#include "datacontainer.h"

DataContainer::DataContainer()
{

}

DataContainer::~DataContainer()
{

}

DataContainer& DataContainer::instance() {
    static DataContainer container;
    return container;
}

std::vector<cv::Mat>& DataContainer::getSequence() {
    return sequence;
}

std::vector<CloudPoint>& DataContainer::getCloud(){
    return point_cloud;
}

void DataContainer::setSequence(std::vector<cv::Mat> s) {
    for (unsigned int i=0; i<s.size(); i++) {
        sequence.push_back(s[i]);
    }
}

void DataContainer::setCloud(std::vector<CloudPoint> pc) {
    for (unsigned int i=0; i<pc.size(); i++) {
        point_cloud.push_back(pc[i]);
    }
}

void DataContainer::showCloud() {
    for (unsigned int i=0; i<point_cloud.size(); i++) {
        qDebug() << point_cloud[i];
    }
}

void DataContainer::showSequence() {

}
