#include "sequencepreview.h"
#include "ui_sequencepreview.h"

SequencePreview::SequencePreview(QWidget *parent)
{
    playbackOn = false;
    image = QImage(":/textures/cloud_na.png").mirrored();
    frame = cv::imread("/home/michal/3dTrajectory/res/nos.png");
}

SequencePreview::~SequencePreview()
{
}

//void SequencePreview::startPlayback(cv::VideoCapture &video, int fps) {
void SequencePreview::startPlayback() {
    playbackOn = true;
    update();
}

void SequencePreview::stopPlayback() {
    playbackOn = false;
}

void SequencePreview::viewFrame(cv::Mat &frame) {
    this->frame = frame;
    update();
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

    cloudNATexture = new QOpenGLTexture(image);
}

void SequencePreview::resizeGL() {

}

void SequencePreview::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    if (playbackOn) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.size().width, frame.size().height, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        qDebug() << textureID;
    } else {
        cloudNATexture->bind();
    }

    program.setUniformValue("texture", 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void SequencePreview::initShaders() {
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/v_sequence.glsl"))
        this->close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/f_sequence.glsl"))
        this->close();

    // Link shader pipeline
    if (!program.link())
        this->close();

    // Bind shader pipeline for use
    if (!program.bind())
        this->close();
}
