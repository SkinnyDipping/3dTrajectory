#include "pointcloudpreview.h"

PointCloudPreview::PointCloudPreview(QWidget *parent)
{
    angle = 0;
    wheelAngle = 0;
}

PointCloudPreview::~PointCloudPreview()
{
    pointsBuffer.destroy();
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

void PointCloudPreview::wheelEvent(QWheelEvent *e) {
    wheelAngle += e->delta()/8;
    qDebug() << wheelAngle;
    update();
}

void PointCloudPreview::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0.4f, 1);

    initShaders();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    loadPointCloudBuffer();
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
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(0.0, 0.0, 0.0);
    matrix.rotate(rotation);
    QMatrix4x4 proj, view, camera;
    proj.perspective(45.0f+wheelAngle/15.0f, 4.0f/3.0f, 0.1f, 100.0f);
    view.lookAt(QVector3D(0,0,5),QVector3D(0,0,0),QVector3D(0,1,0));
    camera.setToIdentity();


//    qDebug()<< projection*matrix;
    program.setUniformValue("mvp_matrix", proj*view*camera);
//    program.setUniformValue("mvp_matrix", matrix);

    drawPointCloud(&program);

}

void PointCloudPreview::initShaders()
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
    pointsBuffer.allocate(&DataContainer::instance().getCloud()[0], DataContainer::instance().getCloud().size() * sizeof(CloudPoint));
}

void PointCloudPreview::drawPointCloud(QOpenGLShaderProgram *program) {
    pointsBuffer.bind();
    GLint vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, DataContainer::instance().getCloud().size());

    glPointSize(10);
    glDrawArrays(GL_TRIANGLES, 0, DataContainer::instance().getCloud().size());
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
