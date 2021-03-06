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
* ConePipeline class.
* 
* An OpenCV pipeline generated by GRIP.
*/
    class ConePipeline {
    public:
        ConePipeline();

        void process(cv::Mat &source0);

        cv::Mat getRgbThresholdOutput();

        cv::Mat getCvMedianblurOutput();

        std::vector<std::vector<cv::Point>> getFindContoursOutput();

        std::vector<std::vector<cv::Point>> getFilterContoursOutput();

    private:
        cv::Mat m_rgbThresholdOutput;
        cv::Mat m_cvMedianblurOutput;
        std::vector<std::vector<cv::Point>> m_findContoursOutput;
        std::vector<std::vector<cv::Point>> m_filterContoursOutput;

        void rgbThreshold(cv::Mat &input, double red[], double green[], double blue[], cv::Mat &output);

        void cvMedianblur(cv::Mat &src, double kSize, cv::Mat &dst);

        void findContours(cv::Mat &input, bool externalOnly, std::vector<std::vector<cv::Point> > &contours);

        void filterContours(std::vector<std::vector<cv::Point> > &inputContours, double minArea,
                            double minPerimeter, double minWidth, double maxWidth, double minHeight,
                            double maxHeight, double solidity[], double maxVertexCount, double minVertexCount,
                            double minRatio, double maxRatio, std::vector<std::vector<cv::Point> > &output);
    };
} // end namespace grip


