#include "pointcloudpreview.h"

PointCloudPreview::PointCloudPreview(QWidget *parent)
{
    angle = 0;
    wheelAngle = 0;
    cloudPreviewOn = false;
}

PointCloudPreview::~PointCloudPreview()
{
    pointsBuffer.destroy();
}

void PointCloudPreview::showCloud(PointCloud &cloud) {
    //TODO implement
}

void PointCloudPreview::clearWindow() {
    qDebug()<<"clearWindow";
}

void PointCloudPreview::mouseMoveEvent(QMouseEvent *e) {
    mousePreviousPosition = mouseCurrentPosition;
    mouseCurrentPosition = QVector2D(e->pos());
    QVector2D diff = mouseCurrentPosition - mousePreviousPosition;
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
    angle = diff.length();
    rotationAxis = (rotationAxis*angle+n).normalized();
    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angle)*rotation;
    update();
}

//void PointCloudPreview::mouseMoveEvent(QMouseEvent *e) {

//}

void PointCloudPreview::wheelEvent(QWheelEvent *e) {
    wheelAngle += e->delta()/8;
    qDebug() << wheelAngle;
    update();
}

void PointCloudPreview::initializeGL()
{
    qDebug() << "initializeGL";
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0.4f, 1);

    loadCloudShaders();

    // Enable depth buffer
//    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
//    glEnable(GL_CULL_FACE);

    loadPointCloudBuffer();
//    drawCloudNotAvailable();

}

void PointCloudPreview::resizeGL(int w, int h)
{
    // Calculate aspect ratio
//    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
//    const qreal zNear = 0.01, zFar = 150.0, fov = 45.0;

    // Reset projection
//    projection.setToIdentity();

    // Set perspective projection
//    projection.perspective(fov, aspect, zNear, zFar);
}

void PointCloudPreview::paintGL()
{
    qDebug() << "paintGL";
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.setToIdentity();
//    matrix.translate(0.0, 0.0, 0.0);
    matrix.rotate(rotation);
    QMatrix4x4 proj, view, camera;
    proj.perspective(45.0f+wheelAngle/15.0f, 4.0f/3.0f, 0.1f, 150.0f);
    view.lookAt(QVector3D(0,0,130),QVector3D(0,0,131),QVector3D(0,1,0));
    camera.setToIdentity();


    qDebug()<< proj*matrix;
    program.setUniformValue("mvp_matrix", proj*view*camera);
//    program.setUniformValue("mvp_matrix", matrix);

    drawPointCloud(&program);

}

void PointCloudPreview::loadCloudShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "/home/michal/3dTrajectory/src/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "/home/michal/3dTrajectory/src/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}

void PointCloudPreview::loadPointCloudBuffer() {
    pointsBuffer.create();
    pointsBuffer.bind();
    pointsBuffer.allocate(&DataContainer::instance().getCloud()[0], DataContainer::instance().getCloud().size()*sizeof(CloudPoint));

    float v[] = {-1.0, 1.0, 10.0,
                 -1.0, -1.0, 10.0,
                  1.0, -1.0, 10.0,
                  1.0, 1.0, 10.0};
//    pointsBuffer.allocate(v, 4*3*sizeof(float));
}

void PointCloudPreview::loadPointCloudBuffer(const void *pointCloud, int nOfPoints) {
    pointsBuffer.create();
    pointsBuffer.bind();
    pointsBuffer.allocate(&DataContainer::instance().getCloud()[0], DataContainer::instance().getCloud().size() * sizeof(CloudPoint));
//    pointsBuffer.allocate(pointCloud, nOfPoints * sizeof(CloudPoint));
//    qDebug() << pointsBuffer.size();
}

void PointCloudPreview::drawPointCloud(QOpenGLShaderProgram *program) {
    pointsBuffer.bind();
    GLint vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
//    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, DataContainer::instance().getCloud().size());
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3);

    glPointSize(1);
    glDrawArrays(GL_POINTS, 0, 10);
}

CloudPoint PointCloudPreview::calculateCentroid() {
    CloudPoint centroid = CloudPoint();
    CloudPoint sum = CloudPoint();
    for (int i=0; i<DataContainer::instance().getCloud().size(); i++) {
        sum += DataContainer::instance().getCloud()[i];
    }
    int n=DataContainer::instance().getCloud().size();
    qDebug() << sum;
    centroid.x = sum.x/n;
    centroid.y = sum.y/n;
    centroid.z = sum.z/n;
    return centroid;
}

void PointCloudPreview::drawCloudNotAvailable() {
    const char* vshader =   "#ifdef GL_ES\n"
                            "precision mediump float;\n"
                            "precision mediump int;\n"
                            "#endif\n"
                            "attribute vec2 a_position;\n"
                            "attribute vec2 a_textCoord;\n"
                            "varying vec2 v_textCoord;\n"

                            "void main() {\n"
                                "gl_Position = vec4(a_position, 0.0, 1.0);\n"
                                "v_textCoord = a_textCoord;\n"
                            "}\n";

    const char* fshader =   "#ifdef GL_ES\n"
                            "precision mediump float;\n"
                            "precision mediump int;\n"
                            "#endif\n"
                            "uniform sampler2D texture;\n"
                            "varying vec2 v_textCoord;\n"

                            "void main() {\n"
                                "gl_FragColor = texture2D(texture, v_textCoord);\n"
                            "}\n";

    const float vertices[] = {-1.0, -1.0, 0.0,
                               1.0, -1.0, 0.0,
                               1.0,  1.0, 0.0,
                              -1.0,  1.0, 0.0};

//    QOpenGLBuffer
}
