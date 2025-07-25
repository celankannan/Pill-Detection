## Purpose
This piece of code is used to detect the number of pills in the image in the input path

## Scope
The pill detection and counting applies only for any pills which was photographed using a green background. Moreover, the algorithm works only for the flat pills
that do not reflect light like smooth glossy cylindrical pills. The algorithm is not expected to remove shadows and shinings. 

## Algorithm/Pipeline
1. Read the image path using opencv
2. Segment the green background in the image using HSV based color segmentation.
3. Implement watershed opencv algorithm to extract the contours/mask of the pills.
4. Use connected component analysis algorithm to convert the count the number of pills.
5. Outputs the image with the pill contours and the number of pills overlaid.

<img width="543" height="430" alt="Screenshot from 2025-07-24 23-41-37" src="https://github.com/user-attachments/assets/f63d9fe7-2a1d-4dcd-837c-c7577dc674af" />

## Implementation

**1. Download**

Clone this repository using:
**git clone https://github.com/celankannan/Pill-Detection.git**


**2. Compile and execute c++ code**
Compile your source file using g++ with OpenCV flags obtained through pkg-config. For OpenCV 4.x, the command looks like this:
**g++ void_level_2_pill_detection.cpp -o pill `pkg-config --cflags --libs opencv4`**

Run the executable as follows:
**./pill**
**
