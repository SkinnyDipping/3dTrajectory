#include "caster.h"

Caster::Caster()
{
qq=0;
}

Caster::~Caster()
{

}

OUTPUT Caster::castCloudToImage(std::vector<Point3D>& cloudKeypoints, std::vector<Point2D>& imageKeypoints)
{
    Point3D cloudCentroid = calculateCentroid(cloudKeypoints);

    //Calculate sphere radius
    int sphereRadius = 0;
    for (unsigned int i=0; i<cloudKeypoints.size(); i++) {
        double d = distance(cloudCentroid, cloudKeypoints[i]);
        if (d > sphereRadius)
            sphereRadius = ceil(d);
    }

//    cloudCentroid=Point3D();
//    sphereRadius = 10;
    generateSphere(cloudCentroid, sphereRadius);

    Point3D P = virtual_sphere[qq];//test
    qq++;

//    Point3D P = Point3D(15.0669, 0.172247, 132.569);
//    Point3D P = Point3D(5,5,7.0710678);
//    Point3D P = Point3D(7.0710678,7.0710678,0);
//    Point3D P = Point3D(-9.97496,0,0.707222);
//    Point3D P = Point3D(8.138,1.962,5);

    // Calculating tangential plane (Pi)
    calculateTangentialPlaneCoeff(cloudCentroid, P);

    // Laying image on XY plane
    std::vector<Point3D> image;
    for (int i=0; i<imageKeypoints.size(); i++)
        image.push_back(Point3D(imageKeypoints[i].x, imageKeypoints[i].y, 0));

    // Translation to origin
    Point3D imageCentroid = calculateCentroid(image);
    for (int i=0; i<image.size(); i++) {
        image[i].x -= imageCentroid.x;
        image[i].y -= imageCentroid.y;
        image[i].z -= imageCentroid.z;
    }
    // Calculating angle between Pi and XY
    Point3D normalPi = Point3D(this->A, this->B, this->C);
    Algebra::normalizeVector(normalPi);
    Point3D normalXY = Point3D(0, 0, 1);
    double anglePiXY = acos(Algebra::dotProduct(normalPi, normalXY)/Algebra::length(normalPi)/Algebra::length(normalXY));

    // Casting image on Pi
    Point3D rotationVector = Algebra::crossProduct(normalXY, normalPi);
    Quaternion q = Quaternion(anglePiXY, rotationVector);

    qDebug() << q;
    Quaternion::rotate(image, q, P);

//    for (int i=0; i<image.size(); i++)
//        qDebug() << A*image[i].x+B*image[i].y+C*image[i].z+D;
    qDebug() << "Angle: "<<anglePiXY*RAD2DEG<<"Vector: "<<rotationVector;
    qDebug() <<"Tangential plane: "<<A<<B<<C<<D;
    qDebug() << "Centroid: "<<calculateCentroid(image);
    qDebug() << "Point P: "<<P;
//    for (int i=0; i<virtual_sphere.size();i++)
//    qDebug() << virtual_sphere[i];

    virtual_sphere.insert(virtual_sphere.end(), image.begin(), image.end());
    return OUTPUT(&virtual_sphere[0], virtual_sphere.size());
}

void Caster::generateSphere(Point3D sphereCenter, float sphereRadius, float resolution)
{
    virtual_sphere.clear();

    for (float phi=0; phi<180; phi+=resolution)
        for (float theta=-135; theta<135; theta+=resolution)
        {
            if (theta > -45 && theta < 45) continue;
//            if(phi==-180 || phi==180 || phi==0 ||
//               theta==-180 || theta==180 || theta==0)
//                continue;
            Point3D p = Point3D();

            //Spherical to Carthesian
            p.x = sphereRadius * sin(theta*DEG2RAD) * cos(phi*DEG2RAD);
            p.y = sphereRadius * sin(theta*DEG2RAD) * sin(phi*DEG2RAD);
            p.z = sphereRadius * cos(theta*DEG2RAD);

            //Translation to sphere center
            p += sphereCenter;

            virtual_sphere.push_back(p);
        }
}

void Caster::calculateTangentialPlaneCoeff(Point3D sphereCenter, Point3D tangentialPoint) {
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
    this->D = -SQ(a)-SQ(b)-SQ(c)+a * x0+b * y0+c * z0;
}

double Caster::distance(Point3D p1, Point3D p2) {
    return sqrt(
            (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)
                    + (p1.z - p2.z) * (p1.z - p2.z));
}

double Caster::MSE(std::vector<Point3D> &set1, std::vector<Point3D> &set2)
{
    double MSE = 0;
    for (int i = 0; i < set1.size(); i++)
        MSE += distance(set1[i], set2[i]) * distance(set1[i], set2[i]);
    MSE /= set1.size();
    return MSE;
}

Point3D Caster::calculateCentroid(std::vector<Point3D> &points)
{
    Point3D output = Point3D();
    for (int i=0; i<points.size(); i++) {
        output += points[i];
    }
    output.x /= points.size();
    output.y /= points.size();
    output.z /= points.size();
    return output;
}

Point2D Caster::calculateCentroid(std::vector<Point2D> &points)
{

}
