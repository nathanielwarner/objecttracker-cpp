#pragma once

#include <iostream>
#include <opencv2/highgui.hpp>

class ColoredObject
{
public:
	ColoredObject(double x, double y); // Construct object from coordinates
	void tick(void); // Call once per frame to update location
};
