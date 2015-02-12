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
#define MAIN_WINDOW

int main(int argc, char *argv[])
{

    DataContainer::instance().setCloud("/home/m.szolucha/Qt_OpenGL/3dTrajectory/data/triangle.test");
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("cube");
    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
#ifdef MAIN_WINDOW
    MainWindow widget;
    widget.show();
#else
    PointCloudPreview widget;
    widget.show();
#endif
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}
