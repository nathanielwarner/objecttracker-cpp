#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "coloredobject.hpp"

using namespace std;
using namespace cv;

vector<ColoredObject> objects;

int pendingX = -1;
int pendingY = -1;

int pendingID = 0;

boost::posix_time::ptime t;

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
	
	t = boost::posix_time::microsec_clock::local_time();
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
		for (int i=0; i<objects.size(); i++)
		{
			if (objects.at(i).tick())
			{
				rectangle(frameBGR, objects.at(i).getBoundingRect(), Scalar(0, 0, 255));
				//objects.at(i).print();
			}
		}
		
		boost::posix_time::ptime ct = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration diff = ct - t;
		long delta = diff.total_milliseconds();
		
		double fps = 1000.0 / delta;
		string message = to_string(fps) + " fps";
		putText(frameBGR, message, Point(35, 35), FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255, 0, 0));
		t = ct;
		imshow("Camera", frameBGR);
		
		if (pendingX != -1)
		{
			Vec3b colorBGR = (frameBGR).at<Vec3b>(pendingX, pendingY);
			Vec3b colorHSV = (frameHSV).at<Vec3b>(pendingX, pendingY);
			objects.push_back(ColoredObject(pendingID, pendingX, pendingY, &frameHSV));
			pendingID++;
			pendingX = -1;
			pendingY = -1;
		}
		int x = waitKey(1);
		if (x == 27)
		{
			cout << "esc key is pressed" << endl;
			break;
		}
		else if (char(x) == 'p')
		{
			printf("----Printing all objects----\n");
			for (int i=0; i<objects.size(); i++)
				objects.at(i).print();
		}
	}
	
	for (int i=0; i<objects.size(); i++)
	{
		objects.at(i).terminate();
	}
	return 0;
}
