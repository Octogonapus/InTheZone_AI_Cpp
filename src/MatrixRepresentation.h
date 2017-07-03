#pragma once

#include <array>
#include <vector>
#include <opencv2/core.hpp>

class MatrixRepresentation {
public:
    MatrixRepresentation() {}

    virtual ~MatrixRepresentation() {}

    void addRobotToMatrix(const float x, const float y, const float theta);

    void addConesToMatrix(const std::vector<std::vector<cv::Point>> &points);

private:
    static constexpr int m_matSize = 24;
    std::array<std::array<int, m_matSize>, m_matSize> m_mat;
};
