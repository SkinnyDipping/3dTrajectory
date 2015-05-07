#include "caster.h"

Caster::Caster()
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
    return mat;
}
cv::Mat Caster::cast(std::vector<Point3D> &cloudKeypoints, std::vector<Point2D> &imageKeypoints)
{
#if 0
    assert(cloudKeypoints.size() == imageKeypoints.size());

    // Calculate centroid
    Point3D cloudCentroid = calculateCentroid(cloudKeypoints);

    // Calculate sphere radius
    int sphereRadius = 0;
    for (unsigned int i = 0; i < cloudKeypoints.size(); i++) {
        double d = distance(cloudCentroid, cloudKeypoints[i]);
        if (d > sphereRadius)
            sphereRadius = ceil(d);
    }

    generateSphere(cloudCentroid, sphereRadius);

    for (int test_point=0; test_point<virtual_sphere.size(); test_point++)
    {
        Point3D P = virtual_sphere[test_point];

        // Calculating tangential plane (Pi)
        calculateTangentialPlaneCoeff(cloudCentroid, P);

        // Laying image on XY plane
        image.clear();
        for (int i=0; i<imageKeypoints.size(); i++) {
            Point3D p = Point3D(imageKeypoints.x, imageKeypoints.y, 0);
            image.push_back(p);
        }

        // Casting cloud on Pi
        for (int i=0; i<cloudKeypoints.size(); i++) {
            cloud[i] = castPointOnPlane(image[i], this->A, this->B, this->C, this->D);
        }

        // Least Square Method

    }
#endif

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
