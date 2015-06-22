#include "pointcloudpreview.h"

#include "caster.h"

PointCloudPreview::PointCloudPreview(QWidget *parent)
{
    wheelAngle = 0;

    cloudPreviewOn = false;
    framePreviewOn = false;
    colorizedPreviewOn = false;

    rotationAngleX = 0.0f;
    rotationAngleY = 0.0f;
    currentRotationAngleX = 0.0f;
    currentRotationAngleY = 0.0f;
    translationX = 0.0f;
    translationY = 0.0f;
    currentTranslationX = 0.0f;
    currentTranslationY = 0.0f;

    initialZoom = 100;

    cloudNAimage = QImage(":/textures/cloud_na.png");
}

PointCloudPreview::~PointCloudPreview()
{
    pointcloud_buffer.destroy();
}

void PointCloudPreview::renderCloud(std::vector<Point3D> &selected_points) {
    cloudPreviewOn = true;
    colorizedPreviewOn = false;

    trajectory_buffer.bind();
    trajectory_buffer.allocate(&selected_points[0], selected_points.size()*sizeof(Point3D));

    update();
}

void PointCloudPreview::renderFrame(cv::Mat_<float> matrix) {
    castMatrix = QMatrix4x4(matrix(0,0),matrix(0,1),matrix(0,2),matrix(0,3),
                            matrix(1,0),matrix(1,1),matrix(1,2),matrix(1,3),
                            matrix(2,0),matrix(2,1),matrix(2,2),matrix(2,3),
                            matrix(3,0),matrix(3,1),matrix(3,2),matrix(3,3));

    framePreviewOn = true;
    colorizedPreviewOn = false;

    update();
}

void PointCloudPreview::renderColorizedCloud(cv::Mat_<float> M) {
    QMatrix4x4 matrix = QMatrix4x4(M(0,0),M(0,1),M(0,2),M(0,3),
                                   M(1,0),M(1,1),M(1,2),M(1,3),
                                   M(2,0),M(2,1),M(2,2),M(2,3),
                                   M(3,0),M(3,1),M(3,2),M(3,3));
    renderColorizedCloud(matrix);
}

void PointCloudPreview::renderColorizedCloud(QMatrix4x4 transformationMatrix) {

    // Calculating reference plane
    QVector4D _p1 = QVector4D(10.0,10.0,0.0,1.0);
    QVector4D _p2 = QVector4D(0.0,10.0,0.0,1.0);
    QVector4D _p3 = QVector4D(10.0,0.0,0.0,1.0);
    _p1 = transformationMatrix*_p1;
    _p2 = transformationMatrix*_p2;
    _p3 = transformationMatrix*_p3;
    QVector3D p1 = _p1.toVector3D();
    QVector3D p2 = _p2.toVector3D();
    QVector3D p3 = _p3.toVector3D();
    QVector3D p = QVector3D::crossProduct(p2-p1,p3-p1);
    double d = -1*p.x()*p1.x()-p.y()*p1.y()-p.z()*p1.z();
    referencePlane = QVector4D(p, d);
    castMatrix = transformationMatrix;

    colorizedPreviewOn = true;
    cloudPreviewOn = false;
    framePreviewOn = false;
    update();
}

void PointCloudPreview::clearWindow() {
    qDebug()<<"clearWindow";
}

void PointCloudPreview::mousePressEvent(QMouseEvent *e)
{
    pressed_point = e->pos();
}

void PointCloudPreview::mouseReleaseEvent(QMouseEvent *e)
{
    release_point = e->pos();
    currentRotationAngleX = rotationAngleX;
    currentRotationAngleY = rotationAngleY;
    currentTranslationX = translationX;
    currentTranslationY = translationY;
}

void PointCloudPreview::mouseDoubleClickEvent(QMouseEvent *e)
{
    rotationAngleX = 0;
    rotationAngleY = 0;
    translationX = 0;
    translationY = 0;

    update();
}

void PointCloudPreview::mouseMoveEvent(QMouseEvent *e)
{
    Qt::MouseButtons buttons = e->buttons();
    if (buttons == Qt::LeftButton){
        rotationAngleX = currentRotationAngleX - (e->x() - pressed_point.x());
        rotationAngleY = currentRotationAngleY - (e->y() - pressed_point.y());
    }
    if (buttons == Qt::RightButton) {
        translationX = currentTranslationX - (e->x() - pressed_point.x());
        translationY = currentTranslationY - (e->y() - pressed_point.y());
        //TODO implemet translation
    }

    update();
}

void PointCloudPreview::wheelEvent(QWheelEvent *e)
{
    wheelAngle += e->delta()/8;
    update();
}

void PointCloudPreview::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    loadCloudShaders();
    loadFrameShaders();
    loadColorizedShaders();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    initPointCloudBuffer();
    initFrameBuffer();
    initCloudNotAvailableScreen();
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
    view.lookAt(QVector3D(translationX,translationY,0+wheelAngle/15.0f+initialZoom),  //eye
                QVector3D(0,0,1+wheelAngle/15.0f+initialZoom),  //center
                QVector3D(0,1,0));                              //up
    camera.setToIdentity();
    QMatrix4x4 mvpMatrix = proj*view*camera;


    // DRAW WITH CLOUD SHADER PROGRAM
    if (cloudPreviewOn) {
        cloudProgram.bind();
        cloudProgram.setUniformValue("u_rotoid", vecCentroid);
        cloudProgram.setUniformValue("mvp_matrix", mvpMatrix);
        cloudProgram.setUniformValue("u_rotation", rotationMatrix);

        drawPointCloud(&cloudProgram);
        cloudProgram.release();
    }

    // DRAW WITH FRAME SHADER PROGRAM
    if (framePreviewOn) {
        frameProgram.bind();
        frameProgram.setUniformValue("u_rotoid", vecCentroid);
        frameProgram.setUniformValue("mvp_matrix", mvpMatrix);
        frameProgram.setUniformValue("u_rotation", rotationMatrix);
        frameProgram.setUniformValue("u_transformationMatrix", castMatrix);
        QVector2D resolution = QVector2D((float)DataContainer::instance().getReferenceFrame().size().width,
                                         (float)DataContainer::instance().getReferenceFrame().size().height);
        frameProgram.setUniformValue("u_rgb_resolution", resolution);

        drawFrame(&frameProgram);
        frameProgram.release();
    }

    if(colorizedPreviewOn) {
        colorizedProgram.bind();
        colorizedProgram.setUniformValue("u_rotoid", vecCentroid);
        colorizedProgram.setUniformValue("mvp_matrix", mvpMatrix);
        colorizedProgram.setUniformValue("u_rotation", rotationMatrix);
        colorizedProgram.setUniformValue("u_transformationMatrix", castMatrix);
        colorizedProgram.setUniformValue("u_transformationMatrix_inv", castMatrix.inverted());
        colorizedProgram.setUniformValue("u_ref_plane", referencePlane);
        QVector2D resolution = QVector2D((float)DataContainer::instance().getReferenceFrame().size().width,
                                         (float)DataContainer::instance().getReferenceFrame().size().height);
        colorizedProgram.setUniformValue("u_rgb_resolution", resolution);
        qDebug() << castMatrix;
        qDebug() << castMatrix.inverted();

        drawColorizedPointCloud(&colorizedProgram);
        colorizedProgram.release();
    }

    if (!(cloudPreviewOn || framePreviewOn || colorizedPreviewOn)) {
        drawCloudNotAvailableScreen(&cloudNAProgram);
    }
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
    if (!frameProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/v_iframe.glsl"))
        close();
    if (!frameProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/f_iframe.glsl"))
        close();
    if (!frameProgram.link())
        close();
}

void PointCloudPreview::loadColorizedShaders()
{
    if (!colorizedProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/v_colorized.glsl"))
        close();
    if (!colorizedProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/f_colorized.glsl"))
        close();
    if (!colorizedProgram.link())
        close();
}

void PointCloudPreview::initPointCloudBuffer()
{
    pointcloud_buffer.create();
    pointcloud_buffer.bind();
    pointcloud_buffer.allocate(&DataContainer::instance().getRGBCloud()[0],
            DataContainer::instance().getRGBCloud().size()*sizeof(Point3DRGB));

    trajectory_buffer.create();
}

void PointCloudPreview::initFrameBuffer()
{
    int w = DataContainer::instance().getReferenceFrame().size().width;
    int h = DataContainer::instance().getReferenceFrame().size().height;
    float* frame = new float[h*w*3];
    for (int y=0; y<h; y++)
        for (int x=0; x<w; x++) {
            frame[3*(y*w+x)+0]=(float)(x);
            frame[3*(y*w+x)+1]=(float)(y);
            frame[3*(y*w+x)+2]=0.0;
        }
    frame_buffer.create();
    frame_buffer.bind();
    frame_buffer.allocate(frame, w*h*3*sizeof(float));
    //    im_ptr = frame_buffer.map(QOpenGLBuffer::Access::ReadOnly);
    //    if (im_ptr == nullptr)
    //        qDebug()<<"DAFAQ im";
    //    else {
    //        qDebug()<<"And the image is:";
    //        for (int i=0; i<100; i++)
    //            qDebug()<<((float *)im_ptr)[i];
    //    }
}

void PointCloudPreview::drawPointCloud(QOpenGLShaderProgram *program)
{
    program->bind();

    pointcloud_buffer.bind();

    // Drawing point cloud
    GLint position_location = program->attributeLocation("a_position");
    program->setAttributeBuffer(position_location, GL_FLOAT, 0, 3, sizeof(Point3DRGB));
    program->enableAttributeArray(position_location);

    GLint color_location = program->attributeLocation("a_color_pc");
    program->setAttributeBuffer(color_location, GL_FLOAT, 3*sizeof(float), 3, sizeof(Point3DRGB));
    program->enableAttributeArray(color_location);

    glPointSize(1);
    glDrawArrays(GL_POINTS, 0, pointcloud_buffer.size()/sizeof(Point3DRGB));

    program->disableAttributeArray(position_location);
    program->disableAttributeArray(color_location);

    pointcloud_buffer.release();

    trajectory_buffer.bind();

    // That's it, if there's no points to colour
    if(trajectory_buffer.size() == 0 || !trajectory_buffer.isCreated())
        return;

    // Drawing (selecting) additional points
    program->setAttributeValue(color_location, 47, 255, 7.8);
    program->setAttributeBuffer(position_location, GL_FLOAT, 0, 3);
    program->enableAttributeArray(position_location);

    glPointSize(10);
    glDrawArrays(GL_POINTS, 0, trajectory_buffer.size()/sizeof(Point3D));

    program->disableAttributeArray(position_location);
    trajectory_buffer.release();

}

void PointCloudPreview::drawFrame(QOpenGLShaderProgram *frameProgram)
{
    frame_buffer.bind();
    frameProgram->bind();

    GLint VBOlocation = frameProgram->attributeLocation("a_position");
    frameProgram->enableAttributeArray(VBOlocation);
    frameProgram->setAttributeBuffer(VBOlocation, GL_FLOAT, 0, 3);

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

    glDrawArrays(GL_POINTS, 0,
                 DataContainer::instance().getReferenceFrame().size().width *
                 DataContainer::instance().getReferenceFrame().size().height);

    frameProgram->disableAttributeArray(VBOlocation);
}

void PointCloudPreview::drawColorizedPointCloud(QOpenGLShaderProgram *program)
{
    pointcloud_buffer.bind();
    program->bind();

    GLint location_pointCoordinates = program->attributeLocation("a_position");
    program->enableAttributeArray(location_pointCoordinates);
    program->setAttributeBuffer(location_pointCoordinates, GL_FLOAT, 0, 3);

    frame_buffer.bind();

    GLint location_texCoordinates = program->attributeLocation("a_textureCoors");
    program->enableAttributeArray(location_texCoordinates);
    program->setAttributeArray(location_texCoordinates, GL_FLOAT, 0, 3);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 DataContainer::instance().getReferenceFrame().size().width,
                 DataContainer::instance().getReferenceFrame().size().height,
                 0, GL_BGR, GL_UNSIGNED_BYTE,
                 DataContainer::instance().getReferenceFrame().ptr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    program->setUniformValue("u_texture", 0);

    glPointSize(1);
    glDrawArrays(GL_POINTS, 0, DataContainer::instance().getCloud().size());

    program->disableAttributeArray(location_pointCoordinates);
}

//TODO: implement
void PointCloudPreview::initCloudNotAvailableScreen()
{
    const char* vshader =   "attribute vec2 a_position;\n"
                            "attribute vec2 a_textCoord;\n"
                            "varying vec2 v_textCoord;\n"

                            "void main() {\n"
                            "gl_Position = vec4(a_position, 0.0, 1.0);\n"
                            "v_textCoord = a_textCoord;\n"
                            "}\n";

    const char* fshader =  "uniform sampler2D texture;\n"
                           "varying vec2 v_textCoord;\n"

                           "void main() {\n"
                           "        // Due to glTexImage2D texture shall have [0, 1] coordinates\n"
                           "        // therefore scaling is neccessary\n"
                           "        vec2 textCoord = 0.5*(v_textCoord) - 0.5;\n"
                           "        // Horizontal flip...\n"
                           "        textCoord.y *= -1.0;\n"
                           "        gl_FragColor = texture2D(texture, textCoord);\n"
                           "}\n";

    const float vertices[] = {-1.0, -1.0, 0.0,
                              1.0, -1.0, 0.0,
                              1.0,  1.0, 0.0,
                              -1.0,  1.0, 0.0};

    cloudNAbuffer.create();
    cloudNAbuffer.bind();
    cloudNAbuffer.allocate(vertices, 4*3*sizeof(float));

    cloudNAtexture = new QOpenGLTexture(cloudNAimage);

    if (!cloudNAProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/v_sequence.glsl"))
        this->close();
    if (!cloudNAProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/f_sequence.glsl"))
        this->close();
    if (!cloudNAProgram.link())
        this->close();

}

void PointCloudPreview::drawCloudNotAvailableScreen(QOpenGLShaderProgram *program)
{
    cloudNAbuffer.bind();
    program->bind();

    GLint VBOlocation = program->attributeLocation("a_position");
    program->enableAttributeArray(VBOlocation);
    program->setAttributeBuffer(VBOlocation, GL_FLOAT, 0, 3);

    GLint textureCoordinateLocation = program->attributeLocation("a_textCoord");
    program->enableAttributeArray(textureCoordinateLocation);
    program->setAttributeBuffer(textureCoordinateLocation, GL_FLOAT, 0, 3);

    cloudNAtexture->bind();

    program->setUniformValue("texture", 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    program->disableAttributeArray(VBOlocation);
    program->disableAttributeArray(textureCoordinateLocation);
}
