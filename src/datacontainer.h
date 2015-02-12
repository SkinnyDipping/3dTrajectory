#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>

#include <QDebug>

#include "data_types.h"

class DataContainer
{
public:
    static DataContainer& instance();

    cv::VideoCapture& getSequence();
    std::vector<CloudPoint>& getCloud();

    void loadSequence(std::string filePath);
    cv::Mat& getNextFrame();

    void setCloud(std::vector<CloudPoint>);
    void setCloud(std::string filePath);

    void debugCloud();
    void showSequence();

private:
    DataContainer();
    DataContainer(const DataContainer &);
    ~DataContainer();

private:
    std::vector<CloudPoint> point_cloud;

    cv::VideoCapture sequence;
    cv::Mat sequenceFrame;
    cv::Mat foregroundFrame;

};

#endif // DATACONTAINER_H
