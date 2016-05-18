#pragma once

#include <iostream>
#include <array>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class ColoredObject
{
	
public:
	ColoredObject(double x, double y, Mat* frameHSV, int hTol=10, int sTol=20, int vTol=20); // Construct object from coordinates. Optionally specify HSV tolerances
	bool tick(); // Call once per frame to update location. Returns whether the object was found in the image
	Rect getBoundingRect();
	void print(void); // 
	
private:
	Mat* frame;
	array<int,2> loc; // Location
	array<int,2> dx, ddx, dddx; // Speed, acceleration, and change in acceleration
	Rect br; // Bounding rectangle
	array<int,3> color;
	array<int,3> hsvTol;
	array<int,3> lower0;
	array<int,3> upper0;
	int lowerh1;
	int upperh1;
	
	void calcTolerances(void);
	int chooseContour(vector<vector<Point>>* contours);
	
};
