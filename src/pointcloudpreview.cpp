#include "pointcloudpreview.h"

PointCloudPreview::PointCloudPreview(QWidget *parent)
{
    wheelAngle = 0;

    cloudPreviewOn = false;
    framePreviewOn = false;

    rotationAngleX = 0.0f;
    rotationAngleY = 0.0f;
    currentRotationAngleX = 0.0f;
    currentRotationAngleY = 0.0f;

    initialZoom = 100;
}

PointCloudPreview::~PointCloudPreview()
{
    pointcloud_buffer.destroy();
}

void PointCloudPreview::showCloud() {
    cloudPreviewOn = true;
    update();
}

void PointCloudPreview::renderFrame(cv::Mat_<float> matrix) {
    castMatrix = QMatrix4x4(matrix(0,0),matrix(0,1),matrix(0,2),matrix(0,3),
                            matrix(1,0),matrix(1,1),matrix(1,2),matrix(1,3),
                            matrix(2,0),matrix(2,1),matrix(2,2),matrix(2,3),
                            matrix(3,0),matrix(3,1),matrix(3,2),matrix(3,3));

    framePreviewOn = true;
    update();
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

    glClearColor(0, 0, 0, 1);

    loadCloudShaders();
    loadFrameShaders();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    loadPointCloudBuffer();
    loadFrameBuffer();
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
    view.lookAt(QVector3D(0,0,0+wheelAngle/15.0f+initialZoom),QVector3D(0,0,1+wheelAngle/15.0f+initialZoom),QVector3D(0,1,0));
    camera.setToIdentity();
    QMatrix4x4 mvpMatrix = proj*view*camera;


    // DRAW WITH CLOUD SHADER PROGRAM
    cloudProgram.bind();
    cloudProgram.setUniformValue("u_rotoid", vecCentroid);
    cloudProgram.setUniformValue("mvp_matrix", mvpMatrix);
    cloudProgram.setUniformValue("u_rotation", rotationMatrix);

    if (cloudPreviewOn)
        drawPointCloud(&cloudProgram);
    cloudProgram.release();

    // DRAW WITH FRAME SHADER PROGRAM
    frameProgram.bind();
    frameProgram.setUniformValue("u_rotoid", vecCentroid);
    frameProgram.setUniformValue("mvp_matrix", mvpMatrix);
    frameProgram.setUniformValue("u_rotation", rotationMatrix);
    frameProgram.setUniformValue("u_transformationMatrix", castMatrix);

    if (framePreviewOn)
        drawFrame(&frameProgram);
    frameProgram.release();
}

void PointCloudPreview::loadCloudShaders()
{
    // Compile vertex shader
    if (!cloudProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/v_pointcloud.glsl"))
        close();

    // Compile fragment shader
    if (!cloudProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/f_pointcloud.glsl"))
        close();

    // Link shader pipeline
    if (!cloudProgram.link())
        close();
}

void PointCloudPreview::loadFrameShaders()
{
    if (!frameProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/v_frame.glsl"))
        close();
    if (!frameProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/f_frame.glsl"))
        close();
    if (!frameProgram.link())
        close();
}

void PointCloudPreview::loadPointCloudBuffer() {
    pointcloud_buffer.create();
    pointcloud_buffer.bind();
    pointcloud_buffer.allocate(&DataContainer::instance().getCloud()[0], DataContainer::instance().getCloud().size()*sizeof(Point3D));
}

void PointCloudPreview::loadFrameBuffer() {
    float rectangle[] = {-1.0, -1.0, 0.0,
                         1.0, -1.0, 0.0,
                         1.0,  1.0, 0.0,
                         -1.0,  1.0, 0.0};
    frame_buffer.create();
    frame_buffer.bind();
    frame_buffer.allocate(rectangle, 4*3*sizeof(float));
}

void PointCloudPreview::drawPointCloud(QOpenGLShaderProgram *program) {
    pointcloud_buffer.bind();

    GLint vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3);

    glPointSize(1);
    glDrawArrays(GL_POINTS, 0, DataContainer::instance().getCloud().size());

    program->disableAttributeArray(vertexLocation);

}

void PointCloudPreview::drawFrame(QOpenGLShaderProgram *frameProgram) {
    frame_buffer.bind();

    GLint VBOLocation = frameProgram->attributeLocation("a_position");
    frameProgram->enableAttributeArray(VBOLocation);
    frameProgram->setAttributeBuffer(VBOLocation, GL_FLOAT, 0, 3);

    GLint textureCoordLocation = frameProgram->attributeLocation("a_textureCoord");
    frameProgram->enableAttributeArray(textureCoordLocation);
    frameProgram->setAttributeBuffer(textureCoordLocation, GL_FLOAT, 0, 3);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 DataContainer::instance().getReferenceFrame().size().width,
                 DataContainer::instance().getReferenceFrame().size().height,
                 0, GL_BGR, GL_UNSIGNED_BYTE,
                 DataContainer::instance().getReferenceFrame().ptr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    frameProgram->setUniformValue("texture", 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    frameProgram->disableAttributeArray(VBOLocation);
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
