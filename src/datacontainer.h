#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <vector>
#include <opencv2/opencv.hpp>

#include <QDebug>

#include "data_types.h"

class DataContainer
{
public:
    static DataContainer& instance();

    std::vector<cv::Mat>& getSequence();
    std::vector<CloudPoint>& getCloud();

    void setSequence(std::vector<cv::Mat>);
    void setCloud(std::vector<CloudPoint>);

    void showCloud();
    void showSequence();

private:
    DataContainer();
    DataContainer(const DataContainer &);
    ~DataContainer();

private:
    std::vector<cv::Mat> sequence;
    std::vector<CloudPoint> point_cloud;

};

#endif // DATACONTAINER_H
