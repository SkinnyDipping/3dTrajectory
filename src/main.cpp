#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QLabel>
#include <QDebug>
#include <QFontDatabase>

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

    app.setApplicationName("3dTrajectory");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Regular.ttf");
#ifndef QT_NO_OPENGL
    MainWindow widget;
    widget.setPalette(app.palette());
    widget.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}
