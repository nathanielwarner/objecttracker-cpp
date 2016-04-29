#include <iostream>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

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
	
	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);
	
	while (true)
	{
		Mat frame;
		
		bool bSuccess = cap.read(frame);
		
		if (!bSuccess)
		{
			cout << "Cannot read from the video stream" << endl;
			break;
		}
		
		imshow("Webcam", frame);
		
		if (waitKey(10) == 27)
		{
			cout << "esc key is pressed" << endl;
			break;
		}
	}
	
	return 0;
}
