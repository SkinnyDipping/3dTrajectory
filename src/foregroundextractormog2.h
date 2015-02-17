#ifndef FOREGROUNDEXTRACTORMOG2_H
#define FOREGROUNDEXTRACTORMOG2_H

#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>

class ForegroundExtractorMOG2
{
public:
    ForegroundExtractorMOG2();
    ForegroundExtractorMOG2(const ForegroundExtractorMOG2 &);
    ~ForegroundExtractorMOG2();

    cv::Mat& getForegroundFrame(cv::Mat frame);

private:
    cv::Ptr<cv::BackgroundSubtractorMOG2> MOG2;
//    cv::VideoCapture& video;
};

#endif // FOREGROUNDEXTRACTORMOG2_H
