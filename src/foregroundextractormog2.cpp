#include "foregroundextractormog2.h"

ForegroundExtractorMOG2::ForegroundExtractorMOG2()
{
    MOG2 = cv::createBackgroundSubtractorMOG2();
}

ForegroundExtractorMOG2::~ForegroundExtractorMOG2()
{
}

cv::Mat& ForegroundExtractorMOG2::getForegroundFrame(cv::Mat& frame)
{
    cv::Mat foreground = cv::Mat(frame.rows, frame.cols,frame.type());
    MOG2->apply(frame, foreground);
    return foreground;
}
