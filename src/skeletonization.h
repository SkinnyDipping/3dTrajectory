#ifndef SKELETONIZATION_H
#define SKELETONIZATION_H

#include "opencv2/opencv.hpp"

/**
 * @brief The Skeletonization class
 *
 * Class that performes basic "star" skeletonization [Fujiyoshi, Lipton].
 * Apart from this, it performs basic image processing routines,
 * proper for this skeletonization (e.g. Canny, MOG)
 */
class Skeletonization
{
public:
    Skeletonization(cv::Size frameResolution);
    ~Skeletonization();
    cv::Mat& getForeground();
    std::vector<cv::Point> getJoints();
    bool pushFrame(cv::Mat _frame);

private:
    bool processFrame();

    void subtractForeground();

private:
    /// frame that is currently processed
    cv::Mat frame;

    /// segmented image
    cv::Mat foreground;

    cv::Ptr<cv::BackgroundSubtractorMOG2> cvMOG2;

    std::vector<cv::Point> joints;

};

#endif // SKELETONIZATION_H
