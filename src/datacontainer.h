#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <fstream>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <QDebug>

#include "data_types.h"

class DataContainer
{

public:
    static DataContainer& instance();

    cv::VideoCapture& getSequence();
    PointCloud& getCloud();

    void loadSequence(std::string filePath);
    cv::Mat& getFrame();
    cv::Mat& getNextFrame();
    bool getNextFrame(cv::Mat& newFrame);
    int getSequenceFPS();
    void setCurrentFrameIndex(int value);

    void setCloud(PointCloud);
    void setCloud(std::string filePath);
    CloudPoint getCloudCentroid();

    void debugCloud();
    void showSequence();

private:
    DataContainer();
    DataContainer(const DataContainer &);
    ~DataContainer();

    CloudPoint calculateCentroid();

private:
    PointCloud point_cloud;

    cv::VideoCapture sequence;
    cv::Mat sequenceFrame;
    cv::Mat foregroundFrame;
    std::vector<cv::Mat> framesPoll;
    int sequenceFPS;

    CloudPoint cloud_centroid;

    unsigned int currentFrameIndex;

};

#endif // DATACONTAINER_H
