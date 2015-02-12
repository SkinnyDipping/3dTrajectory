#include "sequencepreview.h"
#include "ui_sequencepreview.h"

SequencePreview::SequencePreview(QWidget *parent)
{
}

SequencePreview::~SequencePreview()
{
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
    program.setAttributeBuffer(VBOlocation, GL_FLOAT, 0, 4);

    glPointSize(20);
    glDrawArrays(GL_POINTS, 0, 4);
}

void SequencePreview::initShaders() {
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "/home/m.szolucha/Qt_OpenGL/3dTrajectory/src/vPreview.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "/home/m.szolucha/Qt_OpenGL/3dTrajectory/src/fPreview.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
