#include <iostream>
#include <d3d9.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <array>
#include <cmath>

#include "grip/ConePipeline.h"
#include "grip/RedMobileGoalPipeline.h"
#include "grip/BlueMobileGoalPipeline.h"
#include "vJoy.h"
#include "heapScanner.h"
#include "guiHelper.h"

using namespace cv;
using namespace grip;

Mat screencap(HWND hwnd) {
    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize; // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom;
    width = windowsize.right;

    src.create(height, width, CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER); //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
    bi.biWidth = width;
    bi.biHeight = -height; //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight,
               SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *) &bi,
              DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}

Mat blankImage = imread("../img/blank.png");

Mat extraProcessingCones(ConePipeline pipeline) {
    Mat image;
    blankImage.copyTo(image);
    drawContours(image, *pipeline.GetFilterContoursOutput(), -1, Scalar(0, 230, 230), FILLED);
    return image;
}

Mat extraProcessingRedMobileGoals(RedMobileGoalPipeline pipeline) {
    SimpleBlobDetector::Params params;
    params.filterByColor = 1;
    params.blobColor = 255;
    params.minThreshold = 10;
    params.maxThreshold = 220;
    params.filterByArea = true;
    params.minArea = 190;
    params.filterByCircularity = true;
    params.minCircularity = 0.25;
    params.maxCircularity = 10000.0;
    params.filterByConvexity = false;
    params.filterByInertia = false;

    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    std::vector<KeyPoint> keypoints;
    medianBlur(*pipeline.GetRgbThresholdOutput(), *pipeline.GetRgbThresholdOutput(), 3);
    detector->detect(*pipeline.GetRgbThresholdOutput(), keypoints);

    //Remove outlying blobs
    for (int i = 0; i < keypoints.size(); i++) {
        Point2f coord = keypoints[i].pt;
        if (coord.x > 730 && coord.y < 25) {
            keypoints[i] = KeyPoint();
        } else if (coord.x < 70 && coord.y > 560) {
            keypoints[i] = KeyPoint();
        }
    }

    Mat image = blankImage.clone();
    drawKeypoints(*pipeline.GetRgbThresholdOutput(), keypoints, image, Scalar(0, 0, 255),
                  DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    return image;
}

Mat extraProcessingBlueMobileGoals(BlueMobileGoalPipeline pipeline) {
    SimpleBlobDetector::Params params;
    params.filterByColor = 1;
    params.blobColor = 255;
    params.minThreshold = 10;
    params.maxThreshold = 220;
    params.filterByArea = true;
    params.minArea = 190;
    params.filterByCircularity = true;
    params.minCircularity = 0.25;
    params.maxCircularity = 10000.0;
    params.filterByConvexity = false;
    params.filterByInertia = false;

    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    std::vector<KeyPoint> keypoints;
    medianBlur(*pipeline.GetRgbThresholdOutput(), *pipeline.GetRgbThresholdOutput(), 3);
    detector->detect(*pipeline.GetRgbThresholdOutput(), keypoints);

    //Remove outlying blobs
    for (std::vector<KeyPoint>::reverse_iterator it = keypoints.rbegin(); it != keypoints.rend(); it++) {
        Point2f coord = it->pt;
        if (coord.x > 730 && coord.y < 25) {
            keypoints.erase(it.base());
        } else if (coord.x < 70 && coord.y > 560) {
            keypoints.erase(it.base());
        }
    }

    Mat image = blankImage.clone();
    drawKeypoints(*pipeline.GetRgbThresholdOutput(), keypoints, image, Scalar(255, 0, 0),
                  DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    return image;
}

int main(int argc, char **argv) {
    //Get HWND for Virtual Worlds
    HWND hFoundWnd = NULL;
    ::EnumWindows(&FindTheDesiredWnd, reinterpret_cast<LPARAM>(&hFoundWnd));
    if (hFoundWnd == NULL) {
        std::cerr << "Could not get HWND for virtual worlds window" << std::endl;
        return 1;
    }

//    SetForegroundWindow(itzHWND);
//    Sleep(1000);
//    placeWaypoints();

    //Memory scanner
    heapScanner hs(itzHWND);
    std::array<byte, 6> cmp1{50, 46, 49, 32, 109, 32}; //2.1 m
    std::array<byte, 6> cmp2{45, 53, 52, 46, 57, 49}; //-54.91
    std::array<byte, 6> cmp3{43, 57, 57, 46, 57, 49}; //+99.91
    DWORD wp1d_addr = hs.scan(cmp1);
    DWORD wp1a_addr = hs.scan(cmp2);
    DWORD wp2a_addr = hs.scan(cmp3);

    //Grab virtual joystick 1
    vJoy vj(1);

    //Pipelines
    ConePipeline conePipeline;
    RedMobileGoalPipeline redMobileGoalPipeline;
    BlueMobileGoalPipeline blueMobileGoalPipeline;

    //Main loop
    Mat temp1, temp2;
    do {
        //Grab frame and convert to 3 channel
        Mat frame = screencap(itzHWND);
        cvtColor(frame, frame, CV_BGRA2BGR);

        //GRIP processing
        conePipeline.Process(frame);
        redMobileGoalPipeline.Process(frame);
        blueMobileGoalPipeline.Process(frame);

        //Extra processing
        std::vector<Mat> images = std::vector<Mat>();
        images.resize(4);
        images[0] = frame;
        images[1] = extraProcessingCones(conePipeline);
        images[2] = extraProcessingRedMobileGoals(redMobileGoalPipeline);
        images[3] = extraProcessingBlueMobileGoals(blueMobileGoalPipeline);

        //Localize
        double r = hs.numFromDistanceString(hs.readMemory(wp1d_addr, 6)); //Already in meters
        double theta2 = hs.numFromAngleString(hs.readMemory(wp2a_addr, 6)) * 0.0174532925; //Convert to radians
        double theta1 = hs.numFromAngleString(hs.readMemory(wp1a_addr, 6)) * 0.0174532925; //Convert to radians
        double x = r * fabs(cos(theta2));
        double y = x * tan(theta2);
//        std::cout << x << "," << y + 1.557599999856 << std::endl;

        //Stitch images for final view
        hconcat(images[0], images[1], temp1);
        hconcat(images[2], images[3], temp2);
        vconcat(temp1, temp2, temp2);
        resize(temp2, temp1, Size(960, 720), 0, 0, INTER_AREA);
        imshow("ITZ_AI_CPP", temp1);
    } while (waitKey(1) != 27); //Exit on esc key

    //Clean up
    destroyAllWindows();

    return 0;
}
