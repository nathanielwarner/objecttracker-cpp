#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include "coloredobject.hpp"

using namespace std;
using namespace cv;

vector<ColoredObject> objects;

int pendingX = -1;
int pendingY = -1;

void mouseCallback(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		pendingX = x;
		pendingY = y;
	}
}

int main(int argc, const char** argv)
{
	VideoCapture cap(0);
	
	if (!cap.isOpened())
	{
		cout << "Cannot open the camera" << endl;
		return -1;
	}
	
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	
	cout << "Frame size: " << dWidth << " x " << dHeight << endl;
	
	namedWindow("Camera", CV_WINDOW_AUTOSIZE);
	setMouseCallback("Camera", mouseCallback, NULL);
	
	Mat frameBGR, frameHSV;
	while (true)
	{
		bool bSuccess = cap.read(frameBGR);
		
		if (!bSuccess)
		{
			cout << "Cannot read from the video stream" << endl;
			break;
		}
		
		GaussianBlur(frameBGR, frameBGR, Size(3, 3), 0, 0);
		cvtColor(frameBGR, frameHSV, CV_BGR2HSV);
		for (ColoredObject obj : objects)
		{
			if (obj.tick(&frameHSV))
			{
				rectangle(frameBGR, obj.getBoundingRect(), Scalar(0, 0, 255));
			}
		}
			
		imshow("Camera", frameBGR);
		
		if (pendingX != -1)
		{
			Vec3b colorBGR = (frameBGR).at<Vec3b>(pendingX, pendingY);
			Vec3b colorHSV = (frameHSV).at<Vec3b>(pendingX, pendingY);
			objects.push_back(ColoredObject(pendingX, pendingY, &frameHSV));
			pendingX = -1;
			pendingY = -1;
		}
		int x = waitKey(10);
		if (x == 27)
		{
			cout << "esc key is pressed" << endl;
			break;
		}
		else if (char(x) == 'p')
		{
			printf("----Printing all objects----\n");
			for (ColoredObject obj : objects)
				obj.print();
		}
	}
	
	return 0;
}
