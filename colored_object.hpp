#pragma once

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class ColoredObject
{
	
public:
	ColoredObject(double x, double y, Mat* frameHSV); // Construct object from coordinates
	void tick(Mat* frameHSV); // Call once per frame to update location
	void print(void); // 
	
private:
	int x, y; // Location
	uchar h, s, v; // Color
	double dx, ddx, dddx; // Speed, acceleration, and change in acceleration

};
