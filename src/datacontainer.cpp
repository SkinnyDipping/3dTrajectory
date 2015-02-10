#include "datacontainer.h"

DataContainer::DataContainer()
{

}

DataContainer::~DataContainer()
{
    sequence.clear();
    point_cloud.clear();
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

//TODO: erase temp line delimiter
void DataContainer::setCloud(std::string filePath) {
    std::ifstream cloudFile(filePath.c_str());
    if (!cloudFile.is_open()) {
        qCritical() << "DataContainer: Error opening cloud file";
        cloudFile.close();
        return;
    }
    std::string line;
    qDebug() << "DataContainer: start reading file...";
    int t=0;
    while(std::getline(cloudFile,line) && t<20000) {
        t++;
        std::istringstream iss(line);
        float a,b,c,d,e,f,g,h;
        iss >>a>>b>>c>>d>>e>>f>>g>>h;
        point_cloud.push_back(CloudPoint(c,d,e));
    }
    qDebug() << "DataContainer: file read";
    cloudFile.close();
}

void DataContainer::debugCloud() {
    for (unsigned int i=0; i<point_cloud.size(); i++) {
        qDebug() << point_cloud[i];
    }
}

void DataContainer::showSequence() {

}
