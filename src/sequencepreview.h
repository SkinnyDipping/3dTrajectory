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

#include "datacontainer.h"

namespace Ui {
class SequencePreview;
}

class SequencePreview : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit SequencePreview(QWidget *parent = 0);
    ~SequencePreview();

    /**
     * @brief viewFrame Render frame
     * @param frame Frame to be rendered
     * @param foreground True, if frame contains only foreground
     */
    void viewFrame(cv::Mat &frame, bool foreground = false);

    /**
     * @brief startPlayback Enable frames rendering
     */
    void startPlayback();

    /**
     * @brief stopPlayback Disable frames rendering
     */
    void stopPlayback();

    /**
     * @brief clearView TODO
     */
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
    QOpenGLTexture *textureBuffer, *cloudNATexture;

    //TEMP
    QImage image, img;

    /// True, if only foreground shall be rendered
    bool m_renderForeground;
    bool m_playbackOn;

    cv::Mat frame;
    GLuint textureID;



};

#endif // SEQUENCEPREVIEW_H
