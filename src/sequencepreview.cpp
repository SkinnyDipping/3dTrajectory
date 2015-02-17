#include "sequencepreview.h"
#include "ui_sequencepreview.h"

SequencePreview::SequencePreview(QWidget *parent)
{
    playbackOn = false;
    image = QImage("/home/michal/3dTrajectory/res/cloud_na.png").mirrored();
}

SequencePreview::~SequencePreview()
{
}

void SequencePreview::viewFrame(cv::Mat &frame) {
    playbackOn = true;
//    textureBuffer = new QOpenGLTexture(image);
//    update();

}

void SequencePreview::startPlayback(cv::VideoCapture &video, int fps) {
    playbackOn = true;
}

void SequencePreview::stopPlayback() {
    playbackOn = false;
}

void SequencePreview::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.4f, 0.4f, 0.0f, 1.0f);

    initShaders();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    //loading rectangle buffer
    float rectangle[] = {-1.0, -1.0, 0.0,
                          1.0, -1.0, 0.0,
                          1.0,  1.0, 0.0,
                         -1.0,  1.0, 0.0};
    rectangleBuffer.create();
    rectangleBuffer.bind();
    rectangleBuffer.allocate(rectangle, 4*3*sizeof(float));

    textureBuffer = new QOpenGLTexture(image);
//    textureBuffer = new QOpenGLTexture();

}

void SequencePreview::resizeGL() {

}

void SequencePreview::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (playbackOn)
    drawSequencePreview();
}

void SequencePreview::drawSequencePreview() {
    rectangleBuffer.bind();

    GLint VBOlocation = program.attributeLocation("a_position");
    program.enableAttributeArray(VBOlocation);
    program.setAttributeBuffer(VBOlocation, GL_FLOAT, 0, 3);

    GLint textureCoordinateLocation = program.attributeLocation("a_textCoord");
    program.enableAttributeArray(textureCoordinateLocation);
    program.setAttributeBuffer(textureCoordinateLocation, GL_FLOAT, 0, 3);
    textureBuffer->bind();
    program.setUniformValue("texture", 0);

    glPointSize(20);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void SequencePreview::initShaders() {
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "/home/michal/3dTrajectory/src/vPreview.glsl"))
        this->close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "/home/michal/3dTrajectory/src/fPreview.glsl"))
        this->close();

    // Link shader pipeline
    if (!program.link())
        this->close();

    // Bind shader pipeline for use
    if (!program.bind())
        this->close();
}
