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

#include "datacontainer.h"

class PointCloudPreview : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    PointCloudPreview(QWidget *parent = 0);
    ~PointCloudPreview();

signals:


public slots:

protected:
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *e) Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void initShaders();
    void loadPointCloudBuffer();
    void drawPointCloud(QOpenGLShaderProgram *program);

    CloudPoint calculateCentroid();


private:
    QOpenGLShaderProgram program;
    QOpenGLBuffer pointsBuffer;

    QVector2D mouseCurrentPosition, mousePreviousPosition;

    QMatrix4x4 projection;
    qreal angle;
    qint64 wheelAngle;
    QVector3D rotationAxis;
    QQuaternion rotation;


};

#endif // POINTCLOUDPREVIEW_H
