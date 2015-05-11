#include "skeletonization.h"

Skeletonization::Skeletonization(cv::Size frameResolution)
{
    foreground = cv::Mat::zeros(frameResolution, CV_8U);
    frame = cv::Mat::zeros(frameResolution, CV_8UC3);
    cvMOG2 = cv::createBackgroundSubtractorMOG2();
}

bool Skeletonization::pushFrame(cv::Mat _frame)
{
    frame = _frame;
    bool result = processFrame();
    return result;
}

bool Skeletonization::processFrame()
{
    subtractForeground();
    return true;
}

void Skeletonization::subtractForeground()
{
    cvMOG2->apply(frame, foreground);
}

cv::Mat& Skeletonization::getForeground()
{
    return foreground;
}

Skeletonization::~Skeletonization()
{

}

