#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QLabel>
#include <QDebug>

#include "data_types.h"
#include "datacontainer.h"
#include "pointcloudpreview.h"

#define TEST_CLOUD
#define OPENGL_TEST

int main(int argc, char *argv[])
{
#ifndef OPENGL_TEST
#ifdef  TEST_CLOUD
    DataContainer::instance().setCloud("/home/michal/3dTrajectory/data/scan_063.xyz");
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
#else
    DataContainer::instance().setCloud("/home/michal/3dTrajectory/data/chmura.test");
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("cube");
    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    PointCloudPreview widget;
    widget.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
#endif
}
