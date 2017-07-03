#include <opencv/cv.hpp>
#include "MatrixRepresentation.h"

void MatrixRepresentation::addRobotToMatrix(const float x, const float y, const float theta) {
}

void MatrixRepresentation::addConesToMatrix(const std::vector<std::vector<cv::Point>> &points) {
    std::for_each(points.begin(), points.end(), [](std::vector<cv::Point> contour) {
        cv::Rect boundingRect = cv::boundingRect(contour);
    });
}
