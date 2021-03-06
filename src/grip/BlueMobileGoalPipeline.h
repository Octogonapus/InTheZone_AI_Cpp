#pragma once

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>

namespace grip {
/**
* BlueMobileGoalPipeline class.
* 
* An OpenCV pipeline generated by GRIP.
*/
    class BlueMobileGoalPipeline {
    private:
        cv::Mat m_rgbThresholdOutput;

        void rgbThreshold(cv::Mat &input, double red[], double green[], double blue[], cv::Mat &output);

    public:
        BlueMobileGoalPipeline();

        void process(cv::Mat &source0);

        cv::Mat getRgbThresholdOutput();
    };
} // end namespace grip
