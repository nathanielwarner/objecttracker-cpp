#pragma once

#include <iostream>
#include <array>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class ColoredObject
{
	
public:
	ColoredObject(double x, double y, Mat* frameHSV); // Construct object from coordinates
	void tick(Mat* frameHSV); // Call once per frame to update location; returns bounding rectangle around object
	void print(void); // 
	
private:
	const array<int,3> hsvTol = {5, 10, 10};
	array<int,2> loc; // Location
	array<int,3> color; // Color
	array<int,3> lowerBound;
	array<int,3> upperBound;
	int dx, ddx, dddx; // Speed, acceleration, and change in acceleration
	void calcTolerances(void);
	void changeSimpleVal(int *target, int amount, int lower, int upper);
	void changeRadialVal(int *target, int amount, int lower, int upper);
	
};
