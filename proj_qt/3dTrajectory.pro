#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T20:11:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3dTrajectory
TEMPLATE = app

SOURCES += ../src/main.cpp \
    ../src/pointcloudpreview.cpp \
    ../src/datacontainer.cpp \
    ../src/sequencepreview.cpp \
    ../src/mainwindow.cpp

HEADERS  += \
    ../src/pointcloudpreview.h \
    ../src/datacontainer.h \
    ../src/data_types.h \
    ../src/sequencepreview.h \
    ../src/mainwindow.h

FORMS    += \
    ../src/sequencepreview.ui \
    ../src/mainwindow.ui
LIBS += -lopencv_core

DISTFILES += \
    ../src/fshader.glsl \
    ../src/vshader.glsl
