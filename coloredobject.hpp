#pragma once

#include <iostream>
#include <array>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class ColoredObject
{
	
public:
	ColoredObject(double x, double y, Mat* frameHSV, int hTol=10, int sTol=20, int vTol=20); // Construct object from coordinates. Optionally specify HSV tolerances
	bool tick(Mat* frameHSV); // Call once per frame to update location. Returns whether the object was found in the image
	Rect getBoundingRect();
	void print(void); // 
	
private:
	array<int,2> loc; // Location
	int dx, ddx, dddx; // Speed, acceleration, and change in acceleration
	Rect br; // Bounding rectangle
	array<int,3> color;
	array<int,3> hsvTol;
	array<int,3> lowerBound;
	array<int,3> upperBound;
	
	void calcTolerances(void);
	int chooseContour(vector<vector<Point>>* contours);
	
};
