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
    string filename = "data/d6.avi";
    CvCapture * vid1 = cvCaptureFromFile(filename.c_str());
    
    // Error Check Video
    if (!vid1) {
        printf("Could not open video file%s", filename.c_str());
        exit(0);
    }

    // Create a window to display the video
    cvNamedWindow( "mainWin", CV_WINDOW_AUTOSIZE);
    cvMoveWindow( "mainWin", 100, 100 );
    
    // Capture a frame of video
    IplImage * frame = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3 );
    // Image format goes 123123123123123123123
    IplImage * backgroundImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );
    unsigned char * backgroundData = (unsigned char *) backgroundImage->imageData;
    // 3686400
    
    unsigned short background[1280][150];
    unsigned char * data;
    vector<unsigned short> tempVector(150);
    unsigned short pixelValue;

    for (int l = 0; l < 960; l++) {
//        cout << "Processing line " << l << "... " << endl;

        vid1 = cvCaptureFromFile(filename.c_str());
        
        for (int i = 0; i < 198; i++) {
            frame = cvQueryFrame( vid1 );        
            if (i > 47) {
                data = (unsigned char *) frame->imageData;
                for (int x = 1280*l; x < 1280*(l+1); x++) {
                    pixelValue = (unsigned short) data[x*3];
                    background[x % 1280][i-48] = pixelValue;
                }
            }        
        }
        for (int j = 0; j < 1280; j++) {            
            for (int k = 0; k < 150; k++) {            
                tempVector[k] = background[j][k];
            }
            sort(tempVector.begin(), tempVector.end());
            backgroundData[(l)*1280 + j] = (unsigned char) tempVector[74];
        }
        
        cvReleaseCapture(&vid1);
    }
    cout << "Finished Extracting Background! " << endl;
    cvSaveImage("background.jpg", backgroundImage);
    
    cvShowImage( "mainWin", backgroundImage );
    cvWaitKey(0);
        
    // Exit Status
    cout << "Program Completed" << endl;
    
}
