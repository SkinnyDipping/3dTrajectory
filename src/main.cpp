#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QLabel>
#include <QDebug>

#include <opencv2/opencv.hpp>

#include "data_types.h"
#include "datacontainer.h"
#include "pointcloudpreview.h"

#define TEST_CLOUD
#define OPENGL_TEST
#define MAIN_WINDOW

int main(int argc, char *argv[])
{

    DataContainer::instance().setCloud("/home/michal/3dTrajectory/data/scan_063.xyz");
    QApplication app(argc, argv);

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
