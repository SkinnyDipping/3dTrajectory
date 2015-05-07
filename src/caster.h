#ifndef CASTER_H
#define CASTER_H

#include <vector>
#include <assert.h>

#include "opencv2/core.hpp"

#include "data_types.h"

#define SQ(a) (a*a)

class Caster
{
public:
    static Caster& instance();

    /**
     * @brief cast Casts image keypoints onto cloud keypoints
     *
     * Points are casted actually onto a plane, that cover the points of capturing image sequence
     * Based on frame with imageKeypoints
     *
     * @param cloudKeypoints
     * @param imageKeypoints
     * @return transformation matrix image -> cloud
     */
    cv::Mat cast(std::vector<Point3D>& cloudKeypoints, std::vector<Point2D>& imageKeypoints);
    //TEMP
    cv::Mat cast();

private:
    Caster();
    Caster(const Caster &);
    ~Caster();

    /// Generates virtual sphere with specific resolution (i.e. angle step)
    void generateSphere(Point3D sphereCenter, double sphereRadius, double resolution = 1);

    /// Calculates coefficients of plane tangential to sphere in point [tangentialPoint]
    void inline calculateTangentialPlaneCoeff(Point3D sphereCenter, Point3D tangentialPoint);

    /// Casts point p = [xp, yp, zp] on plane Ax + By + Cz + D = 0
    static Point3D castPointOnPlane(Point3D point, float A, float B, float C, float D);

    /// MSE Computes Mean Square Error between two sets of points
    double inline MSE(std::vector<Point3D>& set1, std::vector<Point3D>& set2);

    /// Calculates Carthesian distance between two points
    double inline distance(Point3D p1, Point3D p2);

    /// Calculates centroid
    Point3D calculateCentroid(std::vector<Point3D>& points);

private:
    double A, B, C, D;
    std::vector<Point3D> virtual_sphere;
    std::vector<Point3D> image, cloud;

};

#endif // CASTER_H
