#include "coloredobject.hpp"

using namespace std;
using namespace cv;

ColoredObject::ColoredObject(double x, double y, Mat* frameHSV, int hTol, int sTol, int vTol)
{
	loc[0] = x;
	loc[1] = y;
	Vec3b bcolor = (*frameHSV).at<Vec3b>(y, x);
	color[0] = bcolor.val[0];
	color[1] = bcolor.val[1];
	color[2] = bcolor.val[2];
	hsvTol[0] = hTol;
	hsvTol[1] = sTol;
	hsvTol[2] = vTol;
	calcTolerances();
	printf("----New object registered----\n");
	print();
}

void ColoredObject::calcTolerances(void)
{
	lowerBound = color;
	lowerBound[0] -= hsvTol[0];
	lowerBound[1] -= hsvTol[1];
	lowerBound[2] -= hsvTol[2];
	upperBound = color;
	upperBound[0] += hsvTol[0];
	upperBound[1] += hsvTol[1];
	upperBound[2] += hsvTol[2];
}

int ColoredObject::chooseContour(vector<vector<Point>>* contours)
{
	array<int,2> largest = {0, 0};
	for (int i = 0; i < (*contours).size(); i++)
	{
		int area = contourArea((*contours)[i]);
		if (area > largest[1])
		{
			largest = {i, area};
		}
	}
	return largest[0];
}

bool ColoredObject::tick(Mat* frameHSV)
{
	Mat threshFrame;
	inRange(*frameHSV, Scalar(lowerBound[0], lowerBound[1], lowerBound[2]), Scalar(upperBound[0], upperBound[1], upperBound[2]), threshFrame);
	morphologyEx(threshFrame, threshFrame, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	morphologyEx(threshFrame, threshFrame, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//imshow("Threshold", threshFrame);
	
	vector<vector<Point>> contours;
	findContours(threshFrame, contours, RETR_LIST, CHAIN_APPROX_NONE);
	if (contours.size() == 0)
	{
		return false;
	}
	else
	{
		br = boundingRect(contours[chooseContour(&contours)]);
		return true;
	}
}

Rect ColoredObject::getBoundingRect()
{
	return br;
}

void ColoredObject::print(void)
{
	printf("Object at (%i, %i): H=%i S=%i V=%i\n", loc[0], loc[1], color[0], color[1], color[2]);
	printf("Tolerances: H=[%i,%i] S=[%i,%i] V=[%i,%i]\n\n", lowerBound[0], upperBound[0], lowerBound[1], upperBound[1], lowerBound[2], upperBound[2]);
}
