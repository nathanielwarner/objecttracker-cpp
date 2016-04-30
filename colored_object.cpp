#include "colored_object.hpp"

using namespace std;
using namespace cv;

ColoredObject::ColoredObject(double _x, double _y, Mat* frameHSV)
{
	x = _x;
	y = _y;
	Vec3b color = (*frameHSV).at<Vec3b>(x, y);
	h = color.val[0];
	s = color.val[1];
	v = color.val[2];
}

void ColoredObject::tick(Mat* frameHSV)
{
	
}

void ColoredObject::print(void)
{
	std::cout << x << ", " << y << ": " << (int)h << ", " << (int)s << ", " << (int)v << std::endl;
}
