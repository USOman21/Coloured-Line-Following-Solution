// Include files for required libraries
#include <stdio.h>

#include "opencv_aee.hpp"
#include "main.hpp"     // You can use this file for declaring defined values and functions
#include "pi2c.h"

#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "wiringPiI2C.h"

using namespace std;
using namespace cv;
int getFramecon(Mat frame, int x);
void calculatCorrection(int steering1, int steering2, int x, int intergration);

Pi2c car(0x22); // Configure the I2C interface to the Car as a global variable

void setup(void)
{
    setupCamera(320, 30);  // Enable the camera for OpenCV

}

int main( int argc, char** argv )
{
int intergration;
int x = wiringPiI2CSetup(0x08);
int tracker;
    setup();    // Call a setup function to prepare IO and devices

    cv::namedWindow("Photo");   // Create a GUI window called photo

// int fr = wiringPiI2CSetup(0x08);
    while(1)    // Main loop to perform image processing
    {
        //printf("here1\n");
        Mat frame;

        while(frame.empty())
            frame = captureFrame(); // Capture a frame from the camera and store in a new matrix variable


        cv::imshow("Photo", frame); //Display the image in the window
        Mat newImage = frame;

        if (newImage.data == NULL)

        {
            cout << "No image found! Check path." << endl;
            return 1;
        }

        else

        {
            Mat hsvImage;
            cvtColor(newImage, hsvImage, COLOR_BGR2HSV);

            Mat blueGrayscale, redGrayscale, greenGrayscale, pinkGrayscale;
            Mat redMask1, redMask2;

            inRange(hsvImage, Scalar(90,50,50), Scalar(130,255,255), blueGrayscale);

            inRange(hsvImage, Scalar(0,0,0), Scalar(255,255,60), greenGrayscale);

            inRange(hsvImage, Scalar(138,50,40), Scalar(167,255,255), pinkGrayscale);

            Scalar redLower1 = Scalar(0,100,100);
            Scalar redUpper1 = Scalar(10,255,255);
            Scalar redLower2 = Scalar(150,100,100);
            Scalar redUpper2 = Scalar(180,255,255);
            inRange(hsvImage, redLower1, redUpper1, redMask1);
            inRange(hsvImage, redLower2, redUpper2, redMask2);
            bitwise_or(redMask1, redMask2, redGrayscale);

            int bluePixels = 0;
            bluePixels = countNonZero(blueGrayscale);
            //std::cout << "Blue Pixels = " << bluePixels << std::endl;

            int greenPixels = 0;
            greenPixels = countNonZero(greenGrayscale);
            //std::cout << "Green Pixels = " << greenPixels << std::endl;


            int redPixels = 0;
            redPixels = countNonZero(redGrayscale);
            //std::cout << "Red Pixels = " << redPixels << std::endl;

            int pinkPixels = 0;
            pinkPixels = countNonZero(pinkGrayscale);
/*
            if (pinkPixels > 100)
            {
                resizeCamera(320, 240);
                imshow("BGR image", newImage);
                imshow("HSV image", pinkGrayscale);

               // Mat contours = findContours(pinkGrayscale, RETR_TREE, CHAIN_APPROX_SIMPLE);
              //  cv.drawContours(img,contours,-1,(0,255,0),3);
                /*findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            vector<Point> approx;
            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(contours[i], approx, arcLength(contours[i], true)*0.02, true);
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(approx)) > 1000 &&
                    isContourConvex(approx) )
                {
                    double maxCosine = 0;
                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }
                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                  steering1/55+steering2  if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }

            }

                //resizeCamera(320, 30);

            }
/*
             if (bluePixels > greenPixels && bluePixels > redPixels)

            {
                //printf("This image is blue");
                imshow("BGR image", newImage);
                imshow("HSV image", blueGrayscale);

                int framecon[20];
                int l;
                float len = sizeof(framecon) / sizeof(int);
                int framelength = (int) (320/(float)len);

                for(l=0; l<sizeof(framecon) / sizeof(int); l++)
                {
                 steering1/55+steering2   framecon[l] = getFramecon(blueGrayscale, framelength*(l+1));
                    //printf("%d ",framecon[l]);
                }
                //printf("\n");

            }

            else if (redPixels > bluePixels && redPixels > greenPixels)

            {
                //printf("This image is green");
                imshow("BGR image", newImage);
                imshow("HSV image", greenGrayscale);

                int framecon[21];
                int l;
                float len = sizeof(framecon) / sizeof(int);
                int framelength = (int) (320/(float)len);

                for(l=0; l<sizeof(framecon) / sizeof(int); l++)
                {
                    framecon[l] = getFramecon(redGrayscale, framelength*(l+1));
                    //printf("%d ",framecon[l]);
                }
                //printf("\n");


            }


            */

          //  else if (greenPixels > bluePixels && greenPixels > redPixels)
            //{
                //printf("This image is red");
                imshow("BGR image", newImage);
                imshow("HSV image", greenGrayscale);

                int framecon[20];
                int l;
                float len = sizeof(framecon) / sizeof(int);
                int framelength = (int) (320/(float)len);

                for(l=0; l<sizeof(framecon) / sizeof(int); l++)
                {
                framecon[l] = getFramecon(greenGrayscale, framelength*(l+1));
                    printf("%d  ",framecon[l]);

                }
                printf("\n");

                int outer = (float)sizeof(framecon)/(2*sizeof(int));
                int leftval,leftmag, rightval,rightmag;

                for(l=0;l<outer;l++)
                {
                leftval =0;
                leftmag = 0;
                if(framecon[l]>0)
                {
                int a;
                //printf("%d ",framecon[l]);
                leftval = framecon[l];
                leftmag = outer-(l);
                for(a=l+1;a<outer;a++)
                {

                framecon[a]=1;
                //printf("%d ",framecon[a]);
                }
                //printf("\n");
                break;
                }
                }


                for(l=sizeof(framecon)/sizeof(int)-1;l>sizeof(framecon)/sizeof(int)-outer-1;l--)
                {
                rightval = 0;
                rightmag = 0;

                if(framecon[l]>0)
                {
                rightval = framecon[l];
                rightmag = (l+1-outer);
                //printf("%d ",-framecon[l]);
                int a;
                for(a=l-1;a>outer-1;a--)
                {
                framecon[a]=-1;
                //printf("%d ",framecon[a]);
                }
                //printf("\n");
                break;
                }
                }

        int steering1, steering2;


        //printf("%f\n",steering);
if(tracker == 0)
{
steering1 = (leftval+100*leftmag-(rightval+100*rightmag))/(float)(sizeof(framecon)/sizeof(int));
tracker = 1;
}

sleep(0.001);

if(tracker == 1)
{
steering2 = (leftval+100*leftmag-(rightval+100*rightmag))/(float)(sizeof(framecon)/sizeof(int));
tracker = 2;
}

if(tracker == 2)
{
intergration = intergration + (steering1/55+steering2/55)*0.5*0.001;
calculatCorrection(steering1, steering2, x, intergration);
tracker = 0;
}

else tracker = 0;
        }






        cv::imshow("Photo", frame); //Display the image in the windowi2cs.closeSPI();


//printf("%d %d %d %d\n",leftval,leftmag,rightmag,rightval);




        // break;
    }


    closeCV();  // Disable the camera and close any windows

    return 0;
}



int getFramecon(Mat frame, int x)
{
    int framecon=0;
    int a = 0;
    int b = 0;

    for(a = x-16; a < x ; a++)
    {

        for(b = 0 ; b < 30; b++)
        {

            if(frame.at<unsigned char>(b,a)==255)
            {
                framecon = framecon + 1;

            }

        }
    }

    framecon = ((float)framecon/480)*100;
    return(framecon);
}

void calculatCorrection(int steering1, int steering2,int x,int intergration)
{
int P,I,D;

P=25;
I=12;
D=0;

int correction = 100 + P*(steering2)/55 + I*intergration + D*(steering2/55-steering1/55)/0.001;
wiringPiI2CWrite(x,correction);

}


