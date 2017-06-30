#include <iostream>
#include <d3d9.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "grip/ConePipeline.h"
#include "grip/RedMobileGoalPipeline.h"
#include "grip/BlueMobileGoalPipeline.h"
#include "vJoy.h"

using namespace cv;
using namespace grip;

HWND itzHWND;

BOOL CALLBACK FindTheDesiredWnd(HWND hWnd, LPARAM lParam) {
    *(reinterpret_cast<HWND *>(lParam)) = hWnd;

    int length = ::GetWindowTextLength(hWnd);
    if (length == 0) {
        return TRUE;
    }

    TCHAR *buf = new TCHAR[length + 1];
    memset(buf, 0, (length + 1) * sizeof(TCHAR));
    GetWindowText(hWnd, buf, length + 1);
    std::string text(buf);
    if (text.compare("VEX Robotics Competition  - In The Zone RVW") == 0) {
        itzHWND = hWnd;
        delete buf;
        return FALSE;
    }
    delete buf;
    return TRUE;
}

cv::Mat screencap(HWND hwnd) {
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

Mat blankImage = imread("img/blank.png");

Mat extraProcessingCones(ConePipeline pipeline) {
    Mat image = blankImage.clone();
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
    drawKeypoints(*pipeline.GetRgbThresholdOutput(), keypoints, image, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
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
    drawKeypoints(*pipeline.GetRgbThresholdOutput(), keypoints, image, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    return image;
}

int main(int argc, char **argv) {
    //Try to get hwnd for virtual worlds
    HWND hFoundWnd = NULL;
    ::EnumWindows(&FindTheDesiredWnd, reinterpret_cast<LPARAM>(&hFoundWnd));
    if (hFoundWnd == NULL) {
        std::cerr << "Could not get HWND for virtual worlds window" << std::endl;
        return 1;
    }

    //Grab virtual joystick 1
    vJoy vj(1);

    //Pipelines
    ConePipeline conePipeline;
    RedMobileGoalPipeline redMobileGoalPipeline;
    BlueMobileGoalPipeline blueMobileGoalPipeline;

    //Main loop
    do {
        Mat frame = screencap(itzHWND);

        conePipeline.Process(frame);
        redMobileGoalPipeline.Process(frame);
        blueMobileGoalPipeline.Process(frame);

        std::vector<Mat> images = std::vector<Mat>(6);
        images.push_back(frame);
        images.push_back(extraProcessingCones(conePipeline));
        images.push_back(extraProcessingRedMobileGoals(redMobileGoalPipeline));
        images.push_back(extraProcessingBlueMobileGoals(blueMobileGoalPipeline));

        imshow("ITZ_AI_CPP", screencap(itzHWND));
        vj.setDefaults();
    } while (waitKey(1) != 27);

    //Clean up
    destroyAllWindows();
    delete &vj;

    return 0;
}
