#ifndef POINTCLOUDPREVIEW_H
#define POINTCLOUDPREVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
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

    /**
     * @brief renderCloud Render point cloud
     *
     * Point cloud is loaded from DataContainer
     */
    void renderCloud();

    /**
     * @brief renderFrame Render frame in point cloud window
     *
     * Render frame in point cloud window. Apply optional transformation
     *
     * @param transformationMatrix
     */
    void renderFrame(cv::Mat_<float> transformationMatrix = cv::Mat::eye(4,4,CV_32F));

    /**
     * @brief renderColorizedCloud Render colorized point cloud
     * @param transformationMatrix transformation between RGB texture and ReferencePlane
     */
    void renderColorizedCloud(cv::Mat_<float> transformationMatrix);
    void renderColorizedCloud(QMatrix4x4 transformationMatrix);

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
    void initPointCloudBuffer();
    void drawPointCloud(QOpenGLShaderProgram *cloudProgram);

    void loadFrameShaders();
    void initFrameBuffer();
    void drawFrame(QOpenGLShaderProgram *frameProgram);

    void loadColorizedShaders();
    void drawColorizedPointCloud(QOpenGLShaderProgram *program);

    void initCloudNotAvailableScreen();
    void drawCloudNotAvailableScreen(QOpenGLShaderProgram *program);

private:
    QOpenGLShaderProgram cloudProgram, frameProgram, colorizedProgram;
    QOpenGLBuffer pointcloud_buffer, frame_buffer;

    GLuint textureID;

    /// Point in which mouse was pressed and released
    QPoint pressed_point, release_point;

    /// Fields that describe user controll of view
    qint64 wheelAngle;
    float rotationAngleX, rotationAngleY;
    float currentRotationAngleX, currentRotationAngleY;
    float translationX, translationY;
    float currentTranslationX, currentTranslationY;
    float initialZoom;  //just to debug easier

    PointCloud point_cloud;
    PointCloud frame_points; //debug-proper: frame with points to be casted

    QMatrix4x4 castMatrix;

    bool cloudPreviewOn, framePreviewOn, colorizedPreviewOn;

    /// For colorized preview
    QVector4D referencePlane;

    /// If cloud is not available:
    QImage cloudNAimage;
    QOpenGLShaderProgram cloudNAProgram;
    QOpenGLBuffer cloudNAbuffer;
    QOpenGLTexture* cloudNAtexture;
    GLuint textureCloudNA_ID;

};

#endif // POINTCLOUDPREVIEW_H
