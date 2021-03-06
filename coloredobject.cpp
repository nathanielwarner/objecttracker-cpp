#include "coloredobject.hpp"

using namespace std;
using namespace cv;

ColoredObject::ColoredObject(int ID, int x, int y, Mat* frameHSV, int hTol, int sTol, int vTol)
{
	this->ID = ID;
	directory = "obj" + to_string(ID) + "/";
	string command = "mkdir " + directory;
	system(command.c_str());
	locFile = directory + "loc";
	speedFile = directory + "speed";
	accelFile = directory + "accel";
	frame = frameHSV;
	loc[0] = x;
	loc[1] = y;
	Vec3b bcolor = (*frame).at<Vec3b>(y, x);
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

void ColoredObject::terminate()
{
	string command = "rm -r " + directory;
	system(command.c_str());
}

void ColoredObject::calcTolerances(void)
{
	lower0[0] = color[0] - hsvTol[0];
	upper0[0] = color[0] + hsvTol[0];
	if (lower0[0] < 0)
	{
		lowerh1 = 181 + lower0[0];
		upperh1 = 180;
		lower0[0] = 0;
	}
	else if (upper0[0] > 180)
	{
	    upperh1 = upper0[0] - 181;
	    lowerh1 = 0;
	    upper0[0] = 180;
	}

	lower0[1] = color[1] - hsvTol[1];
	upper0[1] = color[1] + hsvTol[1];
	if (lower0[1] < 0)
		lower0[1] = 0;
	else if (upper0[1] > 255)
		upper0[1] = 255;

	lower0[2] = color[2] - hsvTol[2];
	upper0[2] = color[2] + hsvTol[2];
	if (lower0[2] < 0)
		lower0[2] = 0;
	else if (upper0[2] > 255)
		upper0[2] = 255;
}

vector<vector<Point>> ColoredObject::getContours(void)
{
	Mat temp0;
	inRange(*frame, Scalar(lower0[0], lower0[1], lower0[2]), Scalar(upper0[0], upper0[1], upper0[2]), temp0);
	
	Mat temp1;
	inRange(*frame, Scalar(lowerh1, lower0[1], lower0[2]), Scalar(upperh1, upper0[1], upper0[2]), temp1);
	
	Mat threshFrame;
	bitwise_or(temp0, temp1, threshFrame);
	
	morphologyEx(threshFrame, threshFrame, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	morphologyEx(threshFrame, threshFrame, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//imshow("Threshold", threshFrame);
	
	vector<vector<Point>> contours;
	findContours(threshFrame, contours, RETR_LIST, CHAIN_APPROX_NONE);
	return contours;
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

void ColoredObject::updateLoc(void)
{
	lastLoc[0] = loc[0];
	lastLoc[1] = loc[1];
	loc[0] = br.x + br.width / 2;
	loc[1] = br.y + br.height / 2;
	ofstream file;
	file.open(locFile);
	file << loc[0] << " " << loc[1] << endl;
	file.close();
}

void ColoredObject::updateSpeed(void)
{
	lastSpeed[0] = speed[0];
	lastSpeed[1] = speed[1];
	speed[0] = loc[0] - lastLoc[0];
	speed[1] = loc[1] - lastLoc[1];
	ofstream file;
	file.open(speedFile);
	file << speed[0] << " " << speed[1] << endl;
	file.close();
}

void ColoredObject::updateAccel(void)
{
	lastAccel[0] = accel[0];
	lastAccel[1] = accel[1];
	accel[0] = speed[0] - lastSpeed[0];
	accel[1] = speed[1] - lastSpeed[1];
	ofstream file;
	file.open(accelFile);
	file << accel[0] << " " << accel[1] << endl;
	file.close();
}

void ColoredObject::updateDeltaAccel(void)
{
	deltaAccel[0] = accel[0] - lastAccel[0];
	deltaAccel[1] = accel[1] - lastAccel[1];
}

void ColoredObject::predictNextLocation(void)
{
	predictedLoc[0] = loc[0] + speed[0];// + accel[0] + deltaAccel[0];
	predictedLoc[1] = loc[1] + speed[1];// + accel[1] + deltaAccel[1];
}

bool ColoredObject::tick(void)
{
	vector<vector<Point>> contours = getContours();
	if (contours.size() == 0)
	{
		return false;
	}
	else
	{
		br = boundingRect(contours[chooseContour(&contours)]);
		updateLoc();
		updateSpeed();
		updateAccel();
		updateDeltaAccel();
		predictNextLocation();
		return true;
	}
}

Point ColoredObject::getLocation(void)
{
	return Point(loc[0], loc[1]);
}

Rect ColoredObject::getBoundingRect(void)
{
	return br;
}

void ColoredObject::print(void)
{
	printf("Object ID# %i\n", ID);
	printf("Location: (%i, %i)\n", loc[0], loc[1]);
	printf("Speed: (%i, %i)\n", speed[0], speed[1]);
	printf("Acceleration: (%i, %i)\n", accel[0], accel[1]);
	printf("Predicted next location: (%i, %i)\n", predictedLoc[0], predictedLoc[1]);
	printf("Color: H=%i S=%i V=%i\n", color[0], color[1], color[2]);
	printf("Tolerances: H=[%i,%i] S=[%i,%i] V=[%i,%i]\n", lower0[0], upper0[0], lower0[1], upper0[1], lower0[2], upper0[2]);
	printf("Secondary Tolerance for H: [%i,%i]\n\n", lowerh1, upperh1);
}
