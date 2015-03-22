#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QLabel>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include "opencv2/videoio.hpp"

#include "data_types.h"
#include "datacontainer.h"
#include "pointcloudpreview.h"

#define TEST_CLOUD
#define OPENGL_TEST
#define MAIN_WINDOW
#define TEST_DATA

int main(int argc, char *argv[])
{

#ifdef TEST_DATA
    std::vector<Point2D> image;
    std::vector<Point3D> cloud;
    image.push_back(Point2D(454,132));
    image.push_back(Point2D(367,127));
    image.push_back(Point2D(270,199));
    image.push_back(Point2D(55,110));
    image.push_back(Point2D(544,364));
    image.push_back(Point2D(497,230));
    image.push_back(Point2D(367,320));
    image.push_back(Point2D(443,321));
    image.push_back(Point2D(440,196));
    image.push_back(Point2D(441,229));
    image.push_back(Point2D(366,137));
    image.push_back(Point2D(365,202));
    cloud.push_back(Point3D(0.0463,6.5282,133.215897));
    cloud.push_back(Point3D(-0.8515,6.1534,133.221603));
    cloud.push_back(Point3D(-1.775,5.7682,132.409393));
    cloud.push_back(Point3D(-3.6192,4.9853,133.212799));
    cloud.push_back(Point3D(0.8383,5.1496,131.091095));
    cloud.push_back(Point3D(0.556,6.7573,132.14));
    cloud.push_back(Point3D(-0.8112,6.1722,131.1138));
    cloud.push_back(Point3D(-0.0135,6.4959,131.105896));
    cloud.push_back(Point3D(-0.0762,6.4742,132.492706));
    cloud.push_back(Point3D(-0.617,6.4595,132.135));
    cloud.push_back(Point3D(-0.8596,6.1433,133.112503));
    cloud.push_back(Point3D(-0.8529,6.1302,132.419));
    DataContainer::instance().setImageKeypoints(image);
    DataContainer::instance().setCloudKeypoints(cloud);
#endif
    DataContainer::instance().setCloud("/home/michal/3dTrajectory/data/cloud/scan_061.xyz");
    DataContainer::instance().loadSequence("/home/michal/3dTrajectory/data/seq/2.mp4");

    QApplication app(argc, argv);

    app.setApplicationName("3dTrajectory");
//    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    MainWindow widget;
    widget.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}
