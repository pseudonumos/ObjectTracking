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
static int DELAY    = 100;
static int HEIGHT   = 960;
static int MAXBOX	= 300;

int main(int argc, char * argv[])
{
    // Select A Video
    string filename = "data/d6.avi";
	string filename2 = "data/background.jpg";
    CvCapture * vid1 = cvCaptureFromFile(filename.c_str());
    
    // Error Check Video
    if (!vid1) {
        printf("Could not open video file%s", filename.c_str());
        exit(0);
    }
	
    // Create a window to display the video
//    cvNamedWindow( "blackWin", CV_WINDOW_AUTOSIZE);
//    cvMoveWindow( "blackWin", 0, 0 );
//    cvNamedWindow( "whiteWin", CV_WINDOW_AUTOSIZE);
//    cvMoveWindow( "whiteWin", 600, 200 );

	
    // Differencing Algorithm
    IplImage * frame = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3 );
	IplImage * bwframe = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
    IplImage * difference = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );
//	IplImage * black = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );
//	IplImage * white = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );
	IplImage * bg = cvLoadImage(filename2.c_str(), 0);
	IplImage * trackFrame = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3 );
	cvMerge(bg, bg, bg, 0, trackFrame);
	
	vid1 = cvCaptureFromFile(filename.c_str());
	
	cvNamedWindow( "Final", CV_WINDOW_AUTOSIZE );
    cvMoveWindow( "Final", 600, 0 );
    
	vector<CvPoint> p_points;
	vector<CvPoint> c_points;

	vector<CvScalar> colors;
	// Maroon
	colors.push_back(cvScalar(0x0,0x0,0x80));
	// Navy
	colors.push_back(cvScalar(0x80,0x0,0x0));
	// Red
	colors.push_back(cvScalar(0xff,0x0,0x0));
	// Aqua
	colors.push_back(cvScalar(0xff,0xff,0x0));
	// Purple
	colors.push_back(cvScalar(0x80,0x0,0x80));
	// Green
	colors.push_back(cvScalar(0x0,0xff,0x0));
	// Yellow
	colors.push_back(cvScalar(0x0, 0xff, 0xff));
	// Blue
	colors.push_back(cvScalar(0x0,0x0,0xff));
	// Teal
	colors.push_back(cvScalar(0x80,0x80,0x0));
	// Olive
	colors.push_back(cvScalar(0x0,0x80,0x80));
	// Fuschia
	colors.push_back(cvScalar(0xff,0x0,0xff));
	// Black
	colors.push_back(cvScalar(0x0,0x0,0x0));

	CvScalar color = colors[0];
	for (int i = 0; i < 225; i++) {
		frame = cvQueryFrame( vid1 );        
		if (i > 47) {
			int begin = clock();
			if (i % 15 == 2) {
				color = colors[(i-47)/15];
			}
			if (i == 50) {
//				cvSaveImage("background_t_50.jpg", frame);
			} else if (i == 125) {
//				cvSaveImage("background_t_125.jpg", frame);
			} else if (i == 211) {
//				cvSaveImage("background_t_211.jpg", frame);
			}
			//differencing and segmenting
			cvCvtColor(frame, bwframe, CV_RGB2GRAY);
			cvAbsDiff(bwframe, bg, difference);
			cvThreshold(difference, difference, 15, 255, CV_THRESH_BINARY);
			cvSmooth(difference, difference, CV_MEDIAN);
//			cvAnd(difference, bwframe, black);
//			cvOr(difference, bwframe, white);

//			Takes 10 milliseconds	
//			cvFlip(black, black);
//			cvFlip(white, white);
//			cvShowImage( "whiteWin", difference);
//			cvShowImage( "blackWin", black);

			cvWaitKey(DELAY);
//			sleep(1);
			
			//motion tracking

			IplImage* dst = cvCreateImage( cvGetSize(frame), 8, 3 );
			CvMemStorage* storage = cvCreateMemStorage(0);
			CvSeq* contour = 0;

			cvFindContours( difference, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
			cvZero( dst );

			c_points.clear();
			for( ; contour != 0; contour = contour->h_next )
			{
				CvRect bb = cvBoundingRect(contour, 0);
				if (bb.height * bb.width > 2500) {
//					cvDrawContours( com, contour, cvScalar(255*rand(), 255*rand(), 255*rand()) , cvScalar(255*rand(), 255*rand(), 255*rand()) , -1, CV_FILLED, 8 );
//					cvNamedWindow("blah", CV_WINDOW_AUTOSIZE);
//					cvShowImage("blah", com);
					
//					CvMoments moments;
//					cvMoments( com, &moments, 0 );
//					CvPoint c_of_mass = cvPoint( moments.m10/moments.m00, moments.m01/moments.m00 );
					
					
//					if (c_of_mass.x > bb.x && c_of_mass.x < bb.x + bb.width && c_of_mass.y > bb.y && c_of_mass.y < bb.y + bb.height) {
//						cvCircle(trackFrame, c_of_mass, 3, cvScalar(0, 255, 0), -1);
//						cvCircle(frame, c_of_mass, 3, cvScalar(255, 0, 0), -1);
//					}
					CvPoint center = cvPoint(bb.x + bb.width/2, bb.y + bb.height/2);
					c_points.push_back(center);
					int minDist = 10000000;
					CvPoint min = cvPoint(0, 0);
					if (p_points.size() != 0) {
						for (int i = 0; i < p_points.size(); i++) {
							int dist = sqrt(pow((p_points[i].x - center.x), 2) + pow((p_points[i].y - center.y), 2));
							cout << "dist: " << dist << endl;
							if (dist < minDist) {
								minDist = dist;
								min = p_points[i];
							}
						}
						if (minDist != 10000000 && minDist < 300) {
							cvRectangle(frame, cvPoint(bb.x, bb.y), cvPoint(bb.x + bb.width, bb.y + bb.height), cvScalar(255, 0, 0));
							cvCircle(frame, center, 3, cvScalar(0, 0, 0), -1);
							//cvLine(trackFrame, min, center, color, 1);
							//cvCircle(trackFrame, center, 3, color, -1);
							cvCircle(trackFrame, center, 3, cvScalar(0 , 0, 0), -1);
						}
					} else {
						//cvRectangle(frame, cvPoint(bb.x, bb.y), cvPoint(bb.x + bb.width, bb.y + bb.height), cvScalar(255, 0, 0));
						//cvCircle(frame, center, 3, cvScalar(255, 255, 255), -1);
						//cvCircle(trackFrame, center, 3, cvScalar(255, 255, 255), -1);
					}
				}
				p_points.swap(c_points);

			}

			cvFlip(frame, frame);

//			cvShowImage( "finalWin", trackFrame );
			cvShowImage( "Final", frame );
//			cvWaitKey(0);
			int end = clock();
			cout << "Time taken: " << (end - begin) << endl;
			cout << "Clocks per tick: " << CLOCKS_PER_SEC << endl;
	
		}        
	}
	cvFlip(trackFrame, trackFrame);
	
	
    cvNamedWindow( "finalWin", CV_WINDOW_AUTOSIZE);
    cvMoveWindow( "finalWin", 0, 400 );
	cvShowImage( "finalWin", trackFrame );
	
    cvWaitKey(0);
	sleep(10000);
        
    // Exit Status
    cout << "Program Completed" << endl;
    
}
