#ifndef SEQUENCEPREVIEW_H
#define SEQUENCEPREVIEW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
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

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    void initShaders();
    void drawSequencePreview();

private:
    QOpenGLShaderProgram program;
    QOpenGLBuffer rectangleBuffer;

};

#endif // SEQUENCEPREVIEW_H
