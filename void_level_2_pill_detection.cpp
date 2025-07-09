//import opencv2 imgproc library to do basic image related transformations
#include "opencv2/imgproc.hpp"

//import opencv2 imggui to display the images or videos in windows , and to share the processed image ouput
#include "opencv2/highgui.hpp"

//standard input output file imported for cin and cout
#include<iostream>
#include<stdio.h>

//to avoid multiple "std::" operators 
using namespace std;

//to avoid multiple "cv::"" operators
using namespace cv;

//import image inside
int main()
{

//define path for a color image
string path = "/home/chandru/Documents/opencv/Resources/pills_c.png";

//import image in a Mat format
Mat img = imread(path);

//create image object
Mat img_resize, img_hsv;
//convertcolor to HSV, this is used for easy color tracking, object tracking, environmental monitoring, image filtering, etc.
resize(img,img_resize,Size(round(img.cols/4),round(img.rows/4)));
cvtColor(img_resize,img_hsv,COLOR_BGR2HSV);
imshow("Org",img_resize);
//setting the lower and upper bound of HSV to segment colors foreground
int hue_min, hue_max, sat_min, sat_max, value_min, value_max;
hue_min = 60; //min = 60 for green
hue_max = 180; //max = 180 for green
sat_min = 0;
sat_max = 255;
value_min = 0;
value_max = 255;
Scalar lower_bound_fg(hue_min, sat_min, value_min);
Scalar upper_bound_fg(hue_max, sat_max, value_max);

//setting the lower and upper bound of HSV to segment colors green background
int hue_min2, hue_max2, sat_min2, sat_max2, value_min2, value_max2;
hue_min2 = 0;
hue_max2= 60;
sat_min2 = 0;
sat_max2 = 255;
value_min2 = 0;
value_max2 = 255;
Scalar lower_bound_bg(hue_min2, sat_min2, value_min2);
Scalar upper_bound_bg(hue_max2, sat_max2, value_max2);

/*setting the lower and upper bound of HSV to segment colors white background
int hue_min2, hue_max2, sat_min2, sat_max2, value_min2, value_max2;
hue_min2 = 0;
hue_max2= 179;
sat_min2 = 0;
sat_max2 = 29;
value_min2 = 200;
value_max2 = 255;
Scalar lower_bound_bg(hue_min2, sat_min2, value_min2);
Scalar upper_bound_bg(hue_max2, sat_max2, value_max2);*/

//A Mask to segment the color in that range
Mat mask_fg, mask_bg;

//creates a mask for colors in range of a specific hsv
//inRange(input_hsv_image,lower_bound_hsv_scalar,upper_bound_scalar,cv_8u_type_output_image);
inRange(img_hsv,lower_bound_fg,upper_bound_fg, mask_fg);
inRange(img_hsv,lower_bound_bg,upper_bound_bg, mask_bg);


//Apply mask
Mat seg_fg,seg_bg,img_threshold;
bitwise_and(img_resize,img_resize,seg_fg,mask_fg);
bitwise_and(img_resize,img_resize,seg_bg,mask_bg);
bitwise_not(seg_fg,img_threshold,mask_bg);

//convert to grayscale
cvtColor(img_threshold,img_threshold,COLOR_BGR2GRAY);

imshow("Background",img_threshold);
/*double min, max;
Point min_loc, max_loc;
minMaxLoc(img_threshold,&min,&max,&min_loc,&max_loc);
cout<<"min"<<min;
cout<<"max"<<max;*/

//cvtColor(img_threshold,img_threshold,COLOR_HSV2BGR);
//threshold(img_threshold,img_threshold,130,255,THRESH_BINARY_INV);

//show threshold

//cout<<img_threshold.type();
//img_threshold.convertTo(img_threshold,CV_8UC1);

//do distance transform (to find the shortest distance between the white and closest dark pixels)
Mat dist_trans;
distanceTransform(img_threshold,dist_trans,DIST_L2,3);

//Normalizing the distance in range of 0 to 1 for better visualization
normalize(dist_trans,dist_trans,0,1.0,NORM_MINMAX);
//show distance transform
imshow("Distance Transform", dist_trans);

//get foreground blobs to create markers which is needed for the watershed input
Mat distance_threshold;
//create threshold of sitance transform
threshold(dist_trans,distance_threshold,0.6,255,THRESH_BINARY);
//to find contours, storing in CV_8U form
distance_threshold.convertTo(distance_threshold,CV_8U);
imshow("FOREGROUND",distance_threshold);

//get markers using connected components
Mat labels = Mat::zeros(img_resize.size(),CV_32SC1);


//create a vector for contours
vector<vector<Point>>contours;
findContours(distance_threshold,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
//print(contours);
//subtract the foreground and background
Mat mask;
//cout<<"img"<<img_threshold.type();
//cout<<"distance"<<distance_threshold.type();

subtract(img_threshold,distance_threshold,mask);
imshow("Subtraction of fg and bg",mask);
//connected components check


//Create connected components and -1 is for removing the background from the set
int ncomp = connectedComponents(distance_threshold,labels)-1;

cout<<"Number of components excluding the background: "<<ncomp<<endl;
for (int i=0; i<labels.rows;i++)
{
    for(int j=0;j<labels.cols;j++)
    {
        labels.at<int>(i,j)+= 1;
        if (mask.at<uchar>(i,j)==255)
        {
            labels.at<int>(i,j)=0;
        }    
    }
}
//imshow("Markers",labels);

img_resize.convertTo(img_resize,CV_8UC3);
labels.convertTo(labels,CV_32SC1);
cout<<"l"<<labels.type();
cout<<"i"<<img_resize.type();
if (img_resize.type() != CV_8UC3) {
    // Convert grayscale or other to BGR color
    cvtColor(img_resize, img_resize, COLOR_GRAY2BGR);
}

// Check marker image type
if (labels.type() != CV_32SC1) {
    labels.convertTo(labels, CV_32SC1);
}

watershed(img_resize, labels);
// Draw boundaries (where labels == -1) excluding the first and last rows. the first and last rows segment the 
    for (int i = 1; i <labels.rows -1; ++i) {
        for (int j = 1; j <labels.cols-1; ++j) {
            if (labels.at<int>(i, j) == -1) {
                img_resize.at<Vec3b>(i, j) = Vec3b(0, 0, 255); // Red boundary
            }
        }
    }
int a,b;
a = img_resize.cols;
b = img_resize.rows;
string str;
str = "Number of Pills = " + to_string(ncomp);
putText(img_resize, str,Point(20,20),FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,0,255),1);
imshow("Watershed Boundaries", img_resize);

/*imshow("HSV",img_hsv);
imshow("segmented image_fg",seg_fg);
imshow("segmented image_bg",seg_bg);*/
waitKey(0);
return 0;
}