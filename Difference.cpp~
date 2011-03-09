#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>
#include <vector>
#include "cv.h"
#include "cvaux.h"
#include "highgui.h"

using namespace std;

/* ****************** *
 *  Global Variables  *
 * ****************** */
static int WIDTH    = 1280;
static int DELAY    = 40;
static int HEIGHT   = 960;

int main(int argc, char * argv[])
{
    // Select A Video
    string filename = "data/d3.avi";
	string filename2 = "data/background1.jpg";
    CvCapture * vid1 = cvCaptureFromFile(filename.c_str());
    
    // Error Check Video
    if (!vid1) {
        printf("Could not open video file%s", filename.c_str());
        exit(0);
    }

    // Create a window to display the video
    cvNamedWindow( "mainWin", CV_WINDOW_AUTOSIZE);
    cvMoveWindow( "mainWin", 100, 100 );
    
    // Differencing Algorithm
    IplImage * frame = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3 );
	IplImage * bwframe = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
    IplImage * difference = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );
	IplImage * bg = cvLoadImage(filename2.c_str(), 0);
	
	vid1 = cvCaptureFromFile(filename.c_str());
        
	for (int i = 0; i < 449; i++) {
		frame = cvQueryFrame( vid1 );        
		if (i > 47) {
			cvCvtColor(frame, bwframe, CV_RGB2GRAY);
			cvAbsDiff(bwframe, bg, difference);
			cvThreshold(difference, difference, 50, 255, CV_THRESH_BINARY);
			cvShowImage( "mainWin", difference);
			cvWaitKey(DELAY);
		}        
	}

    cvWaitKey(0);
        
    // Exit Status
    cout << "Program Completed" << endl;
    
}
