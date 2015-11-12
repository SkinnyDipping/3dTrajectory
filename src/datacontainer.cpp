#include "datacontainer.h"

DataContainer::DataContainer() :
    cloud_keypoints(),
    image_keypoints()
{
    currentFrameIndex = 0;
    sequenceAvailable = false;
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

std::vector<Point3D>& DataContainer::getCloud(){
    return point_cloud;
}

PointCloudRGB& DataContainer::getRGBCloud() {
    return point_cloud_RGB;
}

void DataContainer::loadSequence(std::string filePath) {
    framesPoll.clear();
    cv::VideoCapture video(filePath);
    do {
        cv::Mat frame;
        video >> frame;
        if (frame.empty()) break;
        framesPoll.push_back(frame);
    } while (true);
    sequenceFPS=30;
    sequenceAvailable = true;
    setCurrentFrameIndex(0);
    loadSequenceKeypoints(filePath);
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

void DataContainer::setCloud(std::vector<Point3D> pc) {
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
        point_cloud.push_back(Point3D(c,d,e));
        point_cloud_RGB.push_back(Point3DRGB(c,d,e,f,g,h));
    }
    qDebug() << "DataContainer: file read";
    cloudFile.close();

    cloud_centroid = calculateCentroid();

    //Load cloud keypoints
    cloud_keypoints.clear();
    std::string cloudKeypointsPath = "/home/michal/3dTrajectory/data/keypoints/cloud_keypoints.dat";
    std::ifstream keypointsFile(cloudKeypointsPath.c_str());
    if (!keypointsFile.is_open()) {
        qCritical() << "DataContainer: Error opening cloud keypoints file";
        keypointsFile.close();
        return;
    }
    std::string keypointsLine;
    while (std::getline(keypointsFile, keypointsLine)) {
        std::istringstream iss(keypointsLine);
        float a,b,c,d,e,f,g;
        iss >>a>>b>>c>>d>>e>>f>>g;
        cloud_keypoints.push_back(Point3D(b,c,d));
    }
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

Point3D DataContainer::calculateCentroid() {
    Point3D centroid = Point3D();
    Point3D sum = Point3D();
    for (unsigned int i=0; i<point_cloud.size(); i++) {
        sum += point_cloud[i];
    }
    int n=point_cloud.size();
    centroid.x = sum.x/n;
    centroid.y = sum.y/n;
    centroid.z = sum.z/n;
    return centroid;
}

Point3D DataContainer::getCloudCentroid() {
    return cloud_centroid;
}

bool DataContainer::isSequenceAvailable() {
    return sequenceAvailable;
}

PointCloud& DataContainer::getCloudKeypoints() {
    return cloud_keypoints;
}

std::vector<Point2D>& DataContainer::getImageKeypoints() {
    return image_keypoints;
}

Point2D DataContainer::getRGBResolution() {
    Point2D output = Point2D(framesPoll.front().cols, framesPoll.front().rows);
    return output;
}

void DataContainer::loadSequenceKeypoints(std::string sequencePath) {
    image_keypoints.clear();
    std::string sequenceKeypointsFilePath = sequencePath.substr(0, sequencePath.find_last_of("/"));
    std::string seqName = sequencePath.substr(sequencePath.find_last_of("/")+1);
    seqName = seqName.substr(0, seqName.find_last_of("."));
    sequenceKeypointsFilePath = sequenceKeypointsFilePath + "/../keypoints/"+ seqName + ".dat";

    //Load sequence keypoints
    std::ifstream keypointsFile(sequenceKeypointsFilePath.c_str());
    if (!keypointsFile.is_open()) {
        qCritical() << "DataContainer: Error opening sequence keypoints file";
        keypointsFile.close();
        return;
    }
    std::string keypointsLine;
    while (std::getline(keypointsFile, keypointsLine)) {
        std::istringstream iss(keypointsLine);
        int a,b;

        iss >>a>>b;
        image_keypoints.push_back(Point2D(a,b));
    }

}
