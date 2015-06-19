#include "caster.h"

Caster::Caster() :
    currentIdx(0),
    currentMSE(99999),
    m_transformationMatrix(cv::Mat::eye(4,4,CV_64F))
{
}

Caster::~Caster()
{

}

Caster& Caster::instance() {
    static Caster caster;
    return caster;
}

//TEMP
cv::Mat Caster::cast() {
    cv::Mat_<double> mat = cv::Mat::eye(4,4,CV_64F);
    mat(0,0)=0.009050298138771029;
    mat(0,1)=-0.001227227528054062;
    mat(0,2)=-0.002520708129612326;
    mat(0,3)=-2.6788552141394;
    mat(1,0)=0.002780269151376619;
    mat(1,1)=0.002832484552155603;
    mat(1,2)=0.00860320219387253;
    mat(1,3)=0.7027222807434022;
    mat(2,0)=-0.0003607773914907775;
    mat(2,1)=-0.008957616710773905;
    mat(2,2)=0.003065761865768919;
    mat(2,3)=133.4818346498065;
    
    // point no
    mat(0,0)=-0.01067903676058994;
    mat(0,1)= 0.0002767718521799773;
    mat(0,2)= 0.0003604595485442862;
    mat(0,3)= -4.048845748062263;
    mat(1,0)=-0.0003466368989398938;
    mat(1,1)= 0.0005294062697845262;
    mat(1,2)= -0.01066300482411503;
    mat(1,3)= 0.002605658383593806;
    mat(2,0)=-0.0002945854668057715;
    mat(2,1)= -0.01066504158480218;
    mat(2,2)= -0.0005199507588609634;
    mat(2,3)= -0.9818875510987799;
    return mat;
}
cv::Mat Caster::cast(std::vector<Point3D> &cloudKeypoints, std::vector<Point2D> &imageKeypoints)
{
    assert(cloudKeypoints.size() == imageKeypoints.size());

    // Calculate centroid
    Point3D cloudCentroid = calculateCentroid(cloudKeypoints);
    //    qDebug() << "cloud centroid" << cloudCentroid;

    // Calculate sphere radius
    int sphereRadius = 0;
    for (unsigned int i = 0; i < cloudKeypoints.size(); ++i) {
        double d = distance(cloudCentroid, cloudKeypoints[i]);
        if (d > sphereRadius)
            sphereRadius = ceil(d);
    }
    //    qDebug() << "Sphere radius" << sphereRadius;

    // Generate sphere
    generateSphere(cloudCentroid, sphereRadius);

    //    for (int test_point=0; test_point < virtual_sphere.size(); ++test_point)
    {
        int test_point = 16514;
        Point3D P = virtual_sphere[test_point];
        //        qDebug() << "P: " << P;

        // Calculating tangential plane (Pi)
        calculateTangentialPlaneCoeff(cloudCentroid, P);

        //TODO TEMP
        if (test_point==16514) {
            this->A = 0.0697671;
            this->B = -3.99695;
            this->C = -0.139603;
            this->D = 26.9041;
        }
        qDebug() << "Tang plane coeffs: "<<A<<B<<C<<D;

        // Laying image on XY plane
        image.clear();
        for (int i=0; i<imageKeypoints.size(); i++) {
            Point3D p = Point3D(imageKeypoints[i].x, imageKeypoints[i].y, 0);
            image.push_back(p);
        }

        // Translate img so that centroid = origin
        Point3D imageCentroid = calculateCentroid(image);
        //        qDebug() << "Image centroid: " << imageCentroid;
        m_translation_O.x = -imageCentroid.x;
        m_translation_O.y = -imageCentroid.y;
        m_translation_O.z = -imageCentroid.z;
        for (int i = 0; i < image.size(); i++) {
            image[i].x -= imageCentroid.x;
            image[i].y -= imageCentroid.y;
            image[i].z -= imageCentroid.z;
        }

        // XXX: Vertical flip (?)
        for (int i = 0; i < image.size(); i++) {
            image[i].x *= -1;
        }

        // Calculate angle between Pi and XY
        Point3D normalPi = Point3D(this->A, this->B, this->C);
        Algebra::normalizeVector(normalPi);
        Point3D normalXY = Point3D(0, 0, 1);
        double anglePiXY = acos(
                    Algebra::dotProduct(normalPi, normalXY)
                    / Algebra::length(normalPi)
                    / Algebra::length(normalXY));
//                qDebug() << "Angle Pi | XY: " << anglePiXY;

        // Rotate image to Pi
        m_rotation_PiXY_centroid = calculateCentroid(image);
        Point3D rotationVector = Algebra::crossProduct(normalXY, normalPi);
        Quaternion rotation_PiXY = Quaternion(anglePiXY, rotationVector);
        m_rotation_PiXY = rotation_PiXY;
        Quaternion::rotate(image, rotation_PiXY, P);
        //        qDebug() << "image | Pi rotation vector: " << rotationVector;

        // Translation to P
        imageCentroid = calculateCentroid(image);
        m_translation_P = P;
        for (int i=0; i<image.size(); i++) {
            image[i].x += -imageCentroid.x + P.x;
            image[i].y += -imageCentroid.y + P.y;
            image[i].z += -imageCentroid.z + P.z;
        }

        // Cast cloud on Pi
        cloud.clear();
        for (int i = 0; i < cloudKeypoints.size(); i++) {
            cloud.push_back(
                        castPointOnPlane(cloudKeypoints[i], this->A, this->B,
                                         this->C, this->D));
        }

        // Calculate angle of rotation between sets
        Point3D cloudPoint = Point3D();
        Point3D imagePoint = Point3D();
        cloudPoint.x = cloud[0].x - P.x;
        cloudPoint.y = cloud[0].y - P.y;
        cloudPoint.z = cloud[0].z - P.z;
        imagePoint.x = image[0].x - P.x;
        imagePoint.y = image[0].y - P.y;
        imagePoint.z = image[0].z - P.z;
        double rotationAngle = acos(
                    Algebra::dotProduct(cloudPoint, imagePoint)
                    / Algebra::length(cloudPoint)
                    / Algebra::length(imagePoint));
        //        qDebug() << "Data set rotation angle: " << rotationAngle;

        // Rotate image over Pi
        Point3D rotationVector2 = Algebra::crossProduct(imagePoint, cloudPoint);
        Algebra::normalizeVector(rotationVector2);
        Quaternion rotation_image = Quaternion(rotationAngle, rotationVector2);
        m_rotation_overPi = rotation_image;
        Quaternion::rotate(image, rotation_image, P);

        // Determine scale
        // Scale
        m_scale = Algebra::length(cloudPoint)
                / Algebra::length(imagePoint);
        //        qDebug() << "Scale: " << scale;
        for (int i = 0; i < image.size(); i++) {
            image[i].x -= P.x;
            image[i].y -= P.y;
            image[i].z -= P.z;
            image[i].x *= m_scale;
            image[i].y *= m_scale;
            image[i].z *= m_scale;
            image[i].x += P.x;
            image[i].y += P.y;
            image[i].z += P.z;
        }

        // Calculate MSE
        double mse = MSE(image, cloud);
        if (mse < currentMSE) {
            currentMSE = mse;
            currentIdx = test_point;
        }

    }

    qDebug() << "Final point: " << currentIdx << " with MSE: " << currentMSE;

    return generateTransformationMatrix();
}

void Caster::generateSphere(Point3D sphereCenter, double sphereRadius, double resolution)
{
    virtual_sphere.clear();

    for (double phi = 0; phi < 180; phi += resolution)
        for (double theta = -135; theta < 135; theta += resolution) {
            if (theta > -45 && theta < 45)
                continue;
            Point3D p = Point3D();

            //Spherical to Carthesian
            p.x = sphereRadius * sin(theta * DEG2RAD) * cos(phi * DEG2RAD);
            p.y = sphereRadius * sin(theta * DEG2RAD) * sin(phi * DEG2RAD);
            p.z = sphereRadius * cos(theta * DEG2RAD);

            //Translation to sphere center
            p.x += sphereCenter.x;
            p.y += sphereCenter.y;
            p.z += sphereCenter.z;

            virtual_sphere.push_back(p);
        }
}

void Caster::calculateTangentialPlaneCoeff(Point3D sphereCenter, Point3D tangentialPoint)
{
    /*
     * x0, y0, z0	- center of sphere
     * a, b, c		- tangential point (point on sphere)
     */
    double a = tangentialPoint.x;
    double b = tangentialPoint.y;
    double c = tangentialPoint.z;
    double x0 = sphereCenter.x;
    double y0 = sphereCenter.y;
    double z0 = sphereCenter.z;

    this->A = a - x0;
    this->B = b - y0;
    this->C = c - z0;
    this->D = -SQ(a) - SQ(b) - SQ(c) + a * x0 + b * y0 + c * z0;

}

Point3D Caster::castPointOnPlane(Point3D point, float A, float B, float C, float D)
{
    Point3D output;
    double cx = point.x;
    double cy = point.y;
    double cz = point.z;
    double t = (-A * cx - B * cy - C * cz - D) / (SQ(A) + SQ(B) + SQ(C));
    output = Point3D(cx + A * t, cy + B * t, cz + C * t);
    return output;
}

double Caster::MSE(std::vector<Point3D> &set1, std::vector<Point3D> &set2)
{
    double MSE = 0;
    for (int i = 0; i < set1.size(); ++i)
        MSE += distance(set1[i], set2[i]) * distance(set1[i], set2[i]);
    MSE /= set1.size();
    return MSE;
}

double Caster::distance(Point3D p1, Point3D p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)
                + (p1.z - p2.z) * (p1.z - p2.z));
}

Point3D Caster::calculateCentroid(std::vector<Point3D> &points)
{
    Point3D output = Point3D();
    for (int i = 0; i < points.size(); i++) {
        output.x += points[i].x;
        output.y += points[i].y;
        output.z += points[i].z;
    }
    output.x /= points.size();
    output.y /= points.size();
    output.z /= points.size();
    return output;
}

cv::Mat Caster::generateTransformationMatrix()
{
    cv::Mat_<double> transformationMatrix = cv::Mat::eye(4,4,CV_64F);

    cv::Mat_<double> translation_O = cv::Mat::eye(4,4,CV_64F);
    qDebug()<<"TO"<<m_translation_O;
    translation_O(0,3) = m_translation_O.x;
    translation_O(1,3) = m_translation_O.y;
    translation_O(2,3) = m_translation_O.z;

    cv::Mat_<double> verticalFlip = cv::Mat::eye(4,4,CV_64F);
    verticalFlip(0,0) = -1;

    cv::Mat_<double> rotation_PiXY_translation = cv::Mat::eye(4,4,CV_64F);
    rotation_PiXY_translation(0,3) = -m_rotation_PiXY_centroid.x;
    rotation_PiXY_translation(1,3) = -m_rotation_PiXY_centroid.y;
    rotation_PiXY_translation(2,3) = -m_rotation_PiXY_centroid.z;
    cv::Mat rotation_PiXY = m_rotation_PiXY.toTransformationMatrix() * rotation_PiXY_translation;

    cv::Mat_<double> translation_P = cv::Mat::eye(4,4,CV_64F);
    translation_P(0,3) = m_translation_P.x;
    translation_P(1,3) = m_translation_P.y;
    translation_P(2,3) = m_translation_P.z;

    cv::Mat_<double> rotation_Pi = m_rotation_overPi.toTransformationMatrix();

    cv::Mat_<double> scale = cv::Mat::eye(4,4,CV_64F);
    scale(0,0) = m_scale;
    scale(1,1) = m_scale;
    scale(2,2) = m_scale;

    transformationMatrix = translation_P * scale * translation_P.inv() * translation_P * rotation_Pi * translation_P.inv() * translation_P * rotation_PiXY * verticalFlip * translation_O * transformationMatrix;
    m_transformationMatrix = transformationMatrix;
    qDebug()<<"Transformation Matrix:";
    qDebug()<< transformationMatrix(0,0)<<transformationMatrix(0,1)<<transformationMatrix(0,2)<<transformationMatrix(0,3);
    qDebug()<< transformationMatrix(1,0)<<transformationMatrix(1,1)<<transformationMatrix(1,2)<<transformationMatrix(1,3);
    qDebug()<< transformationMatrix(2,0)<<transformationMatrix(2,1)<<transformationMatrix(2,2)<<transformationMatrix(2,3);
    qDebug()<< transformationMatrix(3,0)<<transformationMatrix(3,1)<<transformationMatrix(3,2)<<transformationMatrix(3,3);
    return transformationMatrix;
}

cv::Mat Caster::getTransformationMatrix()
{
    return m_transformationMatrix;
}
