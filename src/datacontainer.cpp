#include "datacontainer.h"

DataContainer::DataContainer()
{
    currentFrameIndex = 0;
}

DataContainer::~DataContainer()
{
    point_cloud.clear();
}

DataContainer& DataContainer::instance() {
    static DataContainer container;
    return container;
}

cv::VideoCapture& DataContainer::getSequence() {
    return sequence;
}

std::vector<CloudPoint>& DataContainer::getCloud(){
    return point_cloud;
}

void DataContainer::loadSequence(std::string filePath) {
    cv::VideoCapture video(filePath);
    do {
        cv::Mat frame;
        video >> frame;
        if (frame.empty()) break;
        framesPoll.push_back(frame);
    } while (true);
    sequenceFPS=30;
}

cv::Mat& DataContainer::getFrame() {
    return framesPoll[currentFrameIndex];
}

cv::Mat& DataContainer::getNextFrame() {
    if (currentFrameIndex >= framesPoll.size()) {
        cv::Mat* frame = new cv::Mat();
        return *frame;
    }
    return framesPoll[currentFrameIndex++];
}

cv::Mat& DataContainer::getReferenceFrame() {
    return framesPoll[0];
}

bool DataContainer::getNextFrame(cv::Mat& newFrame) {
    sequence >> newFrame;
    if (newFrame.empty())
        return false;
    return true;
}

int DataContainer::getSequenceFPS() {
    return sequenceFPS;

}

void DataContainer::setCurrentFrameIndex(int value) {
    this->currentFrameIndex = value;
}

void DataContainer::setCloud(std::vector<CloudPoint> pc) {
    for (unsigned int i=0; i<pc.size(); i++) {
        point_cloud.push_back(pc[i]);
    }
}

//TODO: erase temp points number delimiter
void DataContainer::setCloud(std::string filePath) {
    std::ifstream cloudFile(filePath.c_str());
    if (!cloudFile.is_open()) {
        qCritical() << "DataContainer: Error opening cloud file";
        cloudFile.close();
        return;
    }
    std::string line;
    qDebug() << "DataContainer: start reading file...";
    qWarning() << "Warning: Data delimiter implemented";
    //    int t=0;
    while(std::getline(cloudFile,line)) {
        //        t++;
        std::istringstream iss(line);
        float a,b,c,d,e,f,g,h;
        iss >>a>>b>>c>>d>>e>>f>>g>>h;
        point_cloud.push_back(CloudPoint(c,d,e));
    }
    qDebug() << "DataContainer: file read";
    cloudFile.close();

    cloud_centroid = calculateCentroid();
}
//BOOST_FUSION_ADAPT_STRUCT(PointCloud, (float, x)(float, y)(float, z))

//void DataContainer::setCloud(std::string filePath) {
//    using namespace boost::spirit::qi;
//    boost::iostreams::mapped_file mmap(filePath, boost::iostreams::mapped_file::readonly);
//    auto file = mmap.const_data();
//    auto length = file + mmap.size();
//    point_cloud.reserve(7000000);
////    bool ok = phrase_parse(file, length, (double_ >> double_ >> double_) % eol, space, point_cloud);
//}

void DataContainer::debugCloud() {
    for (unsigned int i=0; i<point_cloud.size(); i++) {
        qDebug() << point_cloud[i];
    }
}

void DataContainer::showSequence() {

}

CloudPoint DataContainer::calculateCentroid() {
    CloudPoint centroid = CloudPoint();
    CloudPoint sum = CloudPoint();
    for (int i=0; i<point_cloud.size(); i++) {
        sum += point_cloud[i];
    }
    int n=point_cloud.size();
    centroid.x = sum.x/n;
    centroid.y = sum.y/n;
    centroid.z = sum.z/n;
    return centroid;
}

CloudPoint DataContainer::getCloudCentroid() {
    return cloud_centroid;
}
