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
    PointCloudRGB& getCloud();

    void loadSequence(std::string filePath);

    /**
     * @brief getFrame get frame in current position
     * @return cv::Mat with frame
     */
    cv::Mat& getFrame();

    /**
     * @brief getNextFrame get frame with current position and increment position
     * @return cv::Mat with frame
     */
    cv::Mat& getNextFrame();

    /**
     * @brief getReferenceFrame get frame containing reference view data (to cast)
     * @return
     */
    cv::Mat& getReferenceFrame();
    bool getNextFrame(cv::Mat& newFrame);
    int getSequenceFPS();
    void setCurrentFrameIndex(int value);

    void setCloud(PointCloud);
    void setCloud(std::string filePath);
    Point3D getCloudCentroid();

    void debugCloud();
    void showSequence();

    bool isSequenceAvailable();

private:
    DataContainer();
    DataContainer(const DataContainer &);
    ~DataContainer();

    Point3D calculateCentroid();

private:
    PointCloudRGB point_cloud;

    cv::VideoCapture sequence;
    cv::Mat sequenceFrame;
    cv::Mat foregroundFrame;
    std::vector<cv::Mat> framesPoll;
    int sequenceFPS;

    Point3D cloud_centroid;

    unsigned int currentFrameIndex;

    bool sequenceAvailable;

};

#endif // DATACONTAINER_H
