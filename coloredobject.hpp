#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class ColoredObject
{
	
public:
	ColoredObject(int ID, int x, int y, Mat* frameHSV, int hTol=10, int sTol=20, int vTol=20); // Construct object from coordinates. Optionally specify HSV tolerances
	bool tick(void); // Call once per frame to update location. Returns whether the object was found in the image
	Point getLocation(void);
	Rect getBoundingRect(void);
	void print(void);
	void terminate(void);
	
private:
	int ID;
	Mat* frame;
	string directory;
	
	array<int,2> lastLoc;
	array<int,2> loc;
	string locFile;
	
	array<int,2> lastSpeed;
	array<int,2> speed;
	string speedFile;
	
	array<int,2> lastAccel;
	array<int,2> accel;
	string accelFile;
	
	array<int,2> deltaAccel;
	array<int,2> predictedLoc;
	Rect br; // Bounding rectangle
	array<int,3> color;
	array<int,3> hsvTol;
	array<int,3> lower0;
	array<int,3> upper0;
	int lowerh1;
	int upperh1;
	
	void calcTolerances(void);
	vector<vector<Point>> getContours(void);
	int chooseContour(vector<vector<Point>>* contours);
	
	void updateLoc(void);
	void updateSpeed(void);
	void updateAccel(void);
	void updateDeltaAccel(void);
	void predictNextLocation(void);
	
};
