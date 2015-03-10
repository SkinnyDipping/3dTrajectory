#include "caster.h"

Caster::Caster()
{
    A = 1;
    B = 3;
    C = 3;
    D = 7;
}

Caster::~Caster()
{

}

void Caster::calculateTangentialPlaneCoeff()
{
    /*
     * x0, y0, z0   - cetner of sphere
     * a, b, c      - tangential point (point on sphere)
     */

    double a = tangential_point.x;
    double b = tangential_point.y;
    double c = tangential_point.z;
    double x0 = sphere_center.x;
    double y0 = sphere_center.y;
    double z0 = sphere_center.z;

    this->A = a - x0;
    this->B = b - y0;
    this->C = c - z0;
    this->D = -SQ(a) - SQ(b) - SQ(c) + a * x0 + b * y0 + c * z0;
}

std::vector<Point3D> Caster::castCloudPoints(std::vector<Point3D>& cloudKeypoints)
{
    std::vector<Point3D> output;
    for (unsigned int i = 0; i < cloudKeypoints.size(); i++) {
        double cx = cloudKeypoints[i].x;
        double cy = cloudKeypoints[i].y;
        double cz = cloudKeypoints[i].z;
        double t = -1 * (A * cx + B * cy + C * cz + D)
                / (SQ(A) + SQ(B) + SQ(C));
        output.push_back(Point3D(cx + A * t, cy + B * t, cz + C * t));
    }
    cloudCasted = true;
    return output;
}

double Caster::MSE(std::vector<Point3D> set1, std::vector<Point3D> set2)
{
    double MSE = 0;
    for (unsigned int i = 0; i < set1.size(); i++)
        MSE += distance(set1[i], set2[i]) * distance(set1[i], set2[i]);
    MSE /= set1.size();
    return MSE;
}

double Caster::distance(Point3D p1, Point3D p2)
{
    return sqrt(
            (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)
                    + (p1.z - p2.z) * (p1.z - p2.z));
}

