#include "BlueMobileGoalPipeline.h"

namespace grip {
    BlueMobileGoalPipeline::BlueMobileGoalPipeline() {}

    /**
    * Runs an iteration of the pipeline and updates outputs.
    */
    void BlueMobileGoalPipeline::process(cv::Mat &source0) {
        //Step RGB_Threshold0:
        //input
        cv::Mat rgbThresholdInput = source0;
        double rgbThresholdRed[] = {0.0, 0.0};
        double rgbThresholdGreen[] = {4.586330935251798, 255.0};
        double rgbThresholdBlue[] = {4.586330935251798, 255.0};
        rgbThreshold(rgbThresholdInput, rgbThresholdRed, rgbThresholdGreen, rgbThresholdBlue,
                     this->m_rgbThresholdOutput);
    }

    /**
     * This method is a generated getter for the output of a RGB_Threshold.
     * @return Mat output from RGB_Threshold.
     */
    cv::Mat BlueMobileGoalPipeline::getRgbThresholdOutput() {
        return this->m_rgbThresholdOutput;
    }

    /**
     * Segment an image based on color ranges.
     *
     * @param input The image on which to perform the RGB threshold.
     * @param red The min and max red.
     * @param green The min and max green.
     * @param blue The min and max blue.
     * @param output The image in which to store the output.
     */
    void BlueMobileGoalPipeline::rgbThreshold(cv::Mat &input, double red[], double green[], double blue[],
                                              cv::Mat &output) {
        cv::cvtColor(input, output, cv::COLOR_BGR2RGB);
        cv::inRange(output, cv::Scalar(red[0], green[0], blue[0]), cv::Scalar(red[1], green[1], blue[1]), output);
    }
} // end grip namespace
