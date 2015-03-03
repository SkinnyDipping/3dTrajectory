#include "pointcloudpreview.h"

PointCloudPreview::PointCloudPreview(QWidget *parent)
{
    wheelAngle = 0;
    cloudPreviewOn = false;

    rotationAngleX = 0.0f;
    rotationAngleY = 0.0f;
    currentRotationAngleX = 0.0f;
    currentRotationAngleY = 0.0f;
}

PointCloudPreview::~PointCloudPreview()
{
    points_buffer.destroy();
}

void PointCloudPreview::showCloud(PointCloud &cloud) {
    //TODO implement
}

void PointCloudPreview::clearWindow() {
    qDebug()<<"clearWindow";
}

void PointCloudPreview::mousePressEvent(QMouseEvent *e) {
    pressed_point = e->pos();
}

void PointCloudPreview::mouseReleaseEvent(QMouseEvent *e) {
    release_point = e->pos();
    currentRotationAngleX = rotationAngleX;
    currentRotationAngleY = rotationAngleY;
}

void PointCloudPreview::mouseDoubleClickEvent(QMouseEvent *e) {
    rotationAngleX = 0;
    rotationAngleY = 0;

    update();
}

void PointCloudPreview::mouseMoveEvent(QMouseEvent *e) {

    Qt::MouseButtons buttons = e->buttons();
    if (buttons == Qt::LeftButton){
    rotationAngleX = currentRotationAngleX - (e->x() - pressed_point.x());
    rotationAngleY = currentRotationAngleY - (e->y() - pressed_point.y());
    }
    if (buttons == Qt::RightButton) {
//TODO implemet translation
    }

    update();
}

void PointCloudPreview::wheelEvent(QWheelEvent *e) {
    wheelAngle += e->delta()/8;
    update();
}

void PointCloudPreview::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0.4f, 1);

    loadCloudShaders();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

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
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //cloud centroid (passed to vertex shader)
    QVector4D vecCentroid =QVector4D(DataContainer::instance().getCloudCentroid().x,
                                     DataContainer::instance().getCloudCentroid().y,
                                     DataContainer::instance().getCloudCentroid().z,
                                     0.0f);

    //Rotation
    QMatrix4x4 rotationMatrix = QMatrix4x4();
    rotationMatrix.rotate(rotationAngleX,0,1,0);
    rotationMatrix.rotate(rotationAngleY,1,0,0);

    //MVP matrix
    QMatrix4x4 proj, view, camera;
    //How camera sees
    proj.perspective(45.0f, 4.0f/3.0f, 0.1f, 1500.0f);
    //From where camera sees (ZOOM HERE)
    view.lookAt(QVector3D(0,0,0+wheelAngle/15.0f),QVector3D(0,0,1+wheelAngle/15.0f),QVector3D(0,1,0));
    camera.setToIdentity();
    QMatrix4x4 mvpMatrix = proj*view*camera;

    program.setUniformValue("u_centroid", vecCentroid);
    program.setUniformValue("mvp_matrix", mvpMatrix);
    program.setUniformValue("u_rotation", rotationMatrix);

    drawPointCloud(&program);
}

void PointCloudPreview::loadCloudShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}

void PointCloudPreview::loadPointCloudBuffer() {
    points_buffer.create();
    points_buffer.bind();
    points_buffer.allocate(&DataContainer::instance().getCloud()[0], DataContainer::instance().getCloud().size()*sizeof(CloudPoint));
}

void PointCloudPreview::drawPointCloud(QOpenGLShaderProgram *program) {
    points_buffer.bind();
    GLint vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    //    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, DataContainer::instance().getCloud().size());
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3);

    glPointSize(1);
    glDrawArrays(GL_POINTS, 0, DataContainer::instance().getCloud().size());

    program->disableAttributeArray(vertexLocation);
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
}
