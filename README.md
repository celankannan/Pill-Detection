**Purpose:** This piece of code is used to detect the number of pills in the image in the input path

**Scope:** The pill detection and counting applies only for any pills which was photographed using a green background. Moreover, the algorithm works only for the flat pills
that do not reflect light like smooth glossy cylindrical pills. The algorithm is not expected to remove shadows and shinings. 

**Algorithm/Pipeline:**
1. Read the image path using opencv
2. Segment the green background in the image using HSV based color segmentation.
3. Implement watershed opencv algorithm to extract the contours/mask of the pills.
4. Use connected component analysis algorithm to convert the count the number of pills.
5. Outputs the image with the pill contours and the number of pills text overlaid.
