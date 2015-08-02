#include "sequencepreview.h"
#include "ui_sequencepreview.h"

#define DEBUG

SequencePreview::SequencePreview(QWidget *parent)
{
    m_playbackOn = false;
    m_renderJoints = false;
    image = QImage(":/textures/sequence_na.png");
    frame = cv::imread("/home/michal/3dTrajectory/res/nos.png");
}

SequencePreview::~SequencePreview()
{
}

//void SequencePreview::startPlayback(cv::VideoCapture &video, int fps) {
void SequencePreview::startPlayback()
{
    m_playbackOn = true;
    update();
}

void SequencePreview::stopPlayback()
{
    m_playbackOn = false;
}

void SequencePreview::viewFrame(cv::Mat &frame, bool foreground, std::vector<Point2D> points)
{
    m_renderJoints = false;
    if (!(points.empty())){
        m_joints.clear();
        if (points[0] != Point2D(-1, -1)){

            m_renderJoints = true;
            int n = points.size();

            // scaling to [-1..1]
            for (int i=0; i<n;i++) {
                int w = frame.size().width/2;
                int h = frame.size().height/2;
                Point2D p = Point2D((points[i].x - w) / w, -(points[i].y - h) / h);
                m_joints.push_back(p);
            }
        }
    }
    this->frame = frame;
    m_renderForeground = foreground;
    update();
}

void SequencePreview::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.4f, 0.4f, 0.0f, 1.0f);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

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

    initPointsPreview();

    cloudNATexture = new QOpenGLTexture(image);
}

void SequencePreview::resizeGL()
{

}

void SequencePreview::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSequencePreview();

    if(m_renderJoints)
        drawPoints(m_joints);
}

void SequencePreview::drawSequencePreview()
{
    rectangleBuffer.bind();
    program.bind();

    GLint VBOlocation = program.attributeLocation("a_position");
    program.enableAttributeArray(VBOlocation);
    program.setAttributeBuffer(VBOlocation, GL_FLOAT, 0, 3);

    GLint textureCoordinateLocation = program.attributeLocation("a_textCoord");
    program.enableAttributeArray(textureCoordinateLocation);
    program.setAttributeBuffer(textureCoordinateLocation, GL_FLOAT, 0, 3);

    if (m_playbackOn) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     frame.size().width, frame.size().height,
                     0, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
        cloudNATexture->bind();
    }

    program.setUniformValue("texture", 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void SequencePreview::drawPoints(std::vector<Point2D> joints)
{
    int nOfPoints = joints.size();

    float* points = new float[nOfPoints*2];
    for (int i=0; i<nOfPoints; i++) {
        points[2*i+0]=joints[i].x;
        points[2*i+1]=joints[i].y;
    }

    pointsBuffer.bind();
    pointsBuffer.allocate(points, 2*nOfPoints*sizeof(float));

    point_program.bind();

    GLint VBOlocation = point_program.attributeLocation("a_position");
    point_program.enableAttributeArray(VBOlocation);
    point_program.setAttributeBuffer(VBOlocation, GL_FLOAT, 0, 2);

    glPointSize(10);
    glDrawArrays(GL_POINTS, 0, nOfPoints);

    point_program.disableAttributeArray(VBOlocation);

}

void SequencePreview::initShaders()
{
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

void SequencePreview::initPointsPreview()
{
    pointsBuffer.create();
    if (!point_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/v_joints.glsl"))
        this->close();
    if (!point_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/f_joints.glsl"))
        this->close();
    if (!point_program.link())
        this->close();
    if (!point_program.bind())
        this->close();
}
