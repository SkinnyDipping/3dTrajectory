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

int main(int argc, char *argv[])
{

    DataContainer::instance().setCloud("/home/michal/3dTrajectory/data/cloud/scan_064.xyz");
    QApplication app(argc, argv);
    DataContainer::instance().loadSequence("/home/michal/3dTrajectory/data/seq/5.mp4");
//    cv::Mat frame;
//    cv::VideoCapture vc("/home/michal/3dTrajectory/data/seq/1.mp4");
//    cv::namedWindow("a");
//    int i=0;
//    vc >> frame;
//    while (!frame.empty())
//    {
//        qDebug() << i++;
//        cv::imshow("a",frame);
//        vc >> frame;
//        cv::waitKey(10);
//    }
//    return 5;
//    cv::Mat frame;
//    cv::Mat foreground;
//    cv::Ptr<cv::BackgroundSubtractorMOG2> cvMOG2;
//    cvMOG2 = cv::createBackgroundSubtractorMOG2();
//    for (int i=0; i<DataContainer::instance().getSequence().get(7); i++)
//    {
//        frame = DataContainer::instance().getNextFrame();
//        cv::Mat foreground;
//        cvMOG2->apply(frame, foreground);
//        cv::imshow("a",frame);
//        cv::imshow("B", foreground);
//        cv::waitKey(33);
//    }
//    return 3;

//    QSurfaceFormat format;
//    format.setDepthBufferSize(24);
//    QSurfaceFormat::setDefaultFormat(format);

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
