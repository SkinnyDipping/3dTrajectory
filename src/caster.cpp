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

std::vector<Point3D> Caster::imageOnPlane(Point3D castedCentroid, std::vector<Point2D> imagePoints)
{
    if (A == 1 && B == 3 && C == 3 && D == 7) {
        std::cerr
                << "ABCD = 1337: Lack of initialization probable \n FUNCTION RETURN";
    }
    vector<Point3D> pointsIn3d, points2;

    //Calculating centroid and translating to origin
//	Point2f I_centroid = Utils::calculateCentroid(points);
//	for (int i = 0; i < points.size(); i++) {
//		points[i].x -= I_centroid.x;
//		points[i].y -= I_centroid.y;
//	}

    //Quaternion:
    // <cos phi/2 ; dx sin phi/2 ; dy sin phi/2 ; dz sin phi/2>
    // phi = arcos( C / sqrt(AA+BB+CC) )
    // alpha: Ax + By + D = 0
    // d = [-B, A, 0]

    double phi = acos(C / sqrt(A * A + B * B + C * C));
    double normVec = sqrt(B * B + A * A + C * C);
    PointXYZ vector = PointXYZ(B / normVec, -A / normVec, 0);
    Quaternion q = Quaternion(-phi, vector);

    for (int i = 0; i < points.size(); i++) {
        pointsIn3d.push_back(PointXYZ(points[i].x, points[i].y, 0));
    }
    points2 = Quaternion::rotate(pointsIn3d, q, tangentialPoint);
    return points2;
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

