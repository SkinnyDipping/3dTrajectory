#include "caster.h"

Caster::Caster()
{

}

Caster::~Caster()
{

}

OUTPUT Caster::castCloudToImage(std::vector<Point3D>& cloudKeypoints, std::vector<Point2D> imageKeypoints)
{
    Point3D cloudCentroid = calculateCentroid(cloudKeypoints);

    //Calculate sphere radius
    int sphereRadius = 0;
    for (unsigned int i=0; i<cloudKeypoints.size(); i++) {
        double d = distance(cloudCentroid, cloudKeypoints[i]);
        if (d > sphereRadius)
            sphereRadius = ceil(d);
    }

    generateSphere(cloudCentroid, sphereRadius);
    Point3D P = virtual_sphere[0];

    calculateTangentialPlaneCoeff(cloudCentroid, P);



    std::vector<Point3D> temp;
    for (int y=-50; y<50; y++)
        for (int z=-1000; z<1000; z++) {
            temp.push_back(Point3D(-(D+B*y+C*z)/A, y, z));
        }
    temp.insert(temp.end(), virtual_sphere.begin(), virtual_sphere.end());
    return OUTPUT(&temp[0], temp.size());
}

void Caster::generateSphere(Point3D sphereCenter, float sphereRadius, float resolution)
{
    virtual_sphere.clear();

    for (float phi=-180; phi<180; phi+=resolution)
        for (float theta=-180; theta<180; theta+=resolution)
        {
            Point3D p = Point3D();

            //Spherical to Carthesian
            p.x = sphereRadius * cos(theta) * cos(phi);
            p.y = sphereRadius * cos(theta) * sin(phi);
            p.z = sphereRadius * sin(theta);

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
    this->D = -SQ(a) - SQ(b) - SQ(c) + a * x0 + b * y0 + c * z0;
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
