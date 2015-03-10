#ifndef POINTCLOUDPREVIEW_H
#define POINTCLOUDPREVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <opencv2/core.hpp>

#include "data_types.h"
#include "datacontainer.h"
#include "algebra.h"

class PointCloudPreview : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    PointCloudPreview(QWidget *parent = 0);
    ~PointCloudPreview();

    void showCloud(PointCloud& cloud);
    void clearWindow();

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *e) Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    void loadCloudShaders();
    void loadPointCloudBuffer();
    void loadPointCloudBuffer(const void *pointCloud, int nOfPoints);
    void drawPointCloud(QOpenGLShaderProgram *program);
    void drawFrame(cv::Mat transformationMatrix);
    void drawCloudNotAvailable();

private:
    QOpenGLShaderProgram program;
    QOpenGLBuffer points_buffer;

    //Point in which mouse was pressed and released
    QPoint pressed_point, release_point;

    qint64 wheelAngle;
    float rotationAngleX, rotationAngleY;
    float currentRotationAngleX, currentRotationAngleY;

    PointCloud point_cloud;
    PointCloud frame_points; //debug-proper: frame with points to be casted

    bool cloudPreviewOn;
};

#endif // POINTCLOUDPREVIEW_H
