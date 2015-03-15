#ifndef CASTER_H
#define CASTER_H

#include <vector>

#include <QDebug>
#include "opencv2/core.hpp"

#include "algebra.h"
#include "data_types.h"

#define SQ(a) a*a

typedef std::pair<Point3D*, int> OUTPUT;

class Caster
{
public:
    Caster();
    ~Caster();

    //TODO: temporary change of return value (debug-proper)
//    cv::Mat castCloudToImage(std::vector<Point3D>& cloudKeypoints, std::vector<Point2D>& imageKeypoints);
    OUTPUT castCloudToImage(std::vector<Point3D>& cloudKeypoints, std::vector<Point2D> imageKeypoints);

//TODO set PRIVATE
public:

    /// Generates virtual sphere with specific resolution (i.e. angle step)
    void generateSphere(Point3D sphereCenter, float sphereRadius, float resolution = 1);

    /// Calculates coefficients of plane tangential to sphere in point [tangentialPoint]
    void inline calculateTangentialPlaneCoeff(Point3D sphereCenter, Point3D tangentialPoint);

    /// MSE Computes Mean Square Error between two sets of points
    double inline MSE(std::vector<Point3D>& set1, std::vector<Point3D>& set2);

    /// Calculates Carthesian distance between two points
    double inline distance(Point3D p1, Point3D p2);

    /// Calculates centroid
    Point3D calculateCentroid(std::vector<Point3D>& points);
    Point2D calculateCentroid(std::vector<Point2D>& points);


//TODO set PRIVATE
public:
    //Tangential plane coefficients:
    //Ax + By + Cz + D = 0
    double A, B, C, D;

    //TODO description
    std::vector<Point3D> virtual_sphere;
};

#endif // CASTER_H
