#ifndef SEQUENCEPREVIEW_H
#define SEQUENCEPREVIEW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QImage>
#include "opencv2/opencv.hpp"

namespace Ui {
class SequencePreview;
}

class SequencePreview : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit SequencePreview(QWidget *parent = 0);
    ~SequencePreview();

    void viewFrame(cv::Mat& frame);
    void startPlayback(cv::VideoCapture &video, int fps);
    void stopPlayback();
    void clearView();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL();
    void paintGL() Q_DECL_OVERRIDE;

private:
    void initShaders();
    void drawSequencePreview();

private:
    QOpenGLShaderProgram program;
    QOpenGLBuffer rectangleBuffer;
    QOpenGLTexture *textureBuffer;

    //TEMP
    QImage image;

    bool playbackOn;


};

#endif // SEQUENCEPREVIEW_H
