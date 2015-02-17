#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T20:11:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 static

TARGET = 3dTrajectory
TEMPLATE = app

SOURCES += ../src/main.cpp \
    ../src/pointcloudpreview.cpp \
    ../src/datacontainer.cpp \
    ../src/sequencepreview.cpp \
    ../src/mainwindow.cpp \
    ../src/QOpenCVWidget.cpp \
    ../src/foregroundextractormog2.cpp

HEADERS  += \
    ../src/pointcloudpreview.h \
    ../src/datacontainer.h \
    ../src/data_types.h \
    ../src/sequencepreview.h \
    ../src/mainwindow.h \
    ../src/QOpenCVWidget.h \
    ../src/foregroundextractormog2.h

FORMS    += \
    ../src/sequencepreview.ui \
    ../src/mainwindow.ui

INCLUDEPATH += /usr/local/include

LIBS += -L"/usr/local/lib/" \
        -lopencv_core -lopencv_highgui -lopencv_video
LIBS += -L"/usr/lib/x86_64-linux-gnu/"
#        -lboost_iostreams

DISTFILES += \
    ../src/fshader.glsl \
    ../src/vshader.glsl
