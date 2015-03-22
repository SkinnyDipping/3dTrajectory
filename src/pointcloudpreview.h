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

    void showCloud(PointCloud &pc);

    /**
     * @brief renderFrame Render frame in point cloud window
     *
     * Render frame in point cloud window. Apply optional transformation
     *
     * @param transformationMatrix
     */
    void renderFrame(cv::Mat_<float> transformationMatrix = cv::Mat::eye(4,4,CV_32F));

    void renderFrame(Point3D* frame, int width, int height, cv::Mat_<float> transformationMatrix = cv::Mat::eye(4, 4, CV_32F));

    /**
     * @brief renderPoint Render single point into window
     * @param point
     */
//    void renderPoint(Point3DRGB point);

    /**
     * @brief renderPoint Render number of points into window.
     * @param points pointer to points array
     * @param n how many points are to be rendered
     */
    void renderPoint(Point3D *points, int n);

    /**
     * @brief rednerPoint
     * @param points
     * @param n
     */
//    void rednerPoint(Point3DRGB *points, int n);

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
    //Methods for drawing point cloud
    void loadCloudShaders();
    void loadTexturedCloudShaders();
    void loadPointCloudBuffer();
    void drawPointCloud(QOpenGLShaderProgram *cloudProgram);
    void drawTexturedPointCloud(QOpenGLShaderProgram *texturedCloudProgram);

    //Method for drawing single frame
    void loadFrameShaders();
    void loadFrameBuffer();
    void drawFrame(QOpenGLShaderProgram *frameProgram);

    //Methods for drawing single point
    void loadPointsBuffer();
    void drawPoints(QOpenGLShaderProgram *pointsProgram);

    void drawCloudNotAvailable();

private:
    QOpenGLShaderProgram cloudProgram, texturedCloudProgram, frameProgram;
    QOpenGLBuffer pointcloud_buffer, textured_pointcloud_buffer, frame_buffer, points_buffer;

    GLuint textureID;

    //Point in which mouse was pressed and released
    QPoint pressed_point, release_point;

    qint64 wheelAngle;
    float rotationAngleX, rotationAngleY, currentRotationAngleX, currentRotationAngleY;
    float initialZoom;  //just to debug easier
    float translationX, translationY, currentTranslationX, currentTranslationY;

    PointCloud point_cloud;
    PointCloud frame_points; //debug-proper: frame with points to be casted
    PointCloud points;  ///< Regular points to render in Window

    QMatrix4x4 castMatrix;

    bool cloudPreviewOn, framePreviewOn, pointsPreviewOn;
};

#endif // POINTCLOUDPREVIEW_H
