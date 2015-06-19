#ifndef CASTER_H
#define CASTER_H

#include <vector>
#include <assert.h>

#include "opencv2/core.hpp"

#include "data_types.h"
#include "algebra.h"
#include "quaternion.h"

#ifndef SQ
#define SQ(a) ((a)*(a))
#endif

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

    cv::Mat getTransformationMatrix();

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

    /// Creates transformation matrix based on params set in cast(...)
    cv::Mat generateTransformationMatrix();

// TODO: change to private
public:
    /// Tangential plane coefficients
    double A, B, C, D;

    std::vector<Point3D> virtual_sphere;

    /// Containers for image and cloud keypoints during calculations
    std::vector<Point3D> image, cloud;

    /// Transformation parameters
    Quaternion m_rotation_PiXY, m_rotation_overPi;
    Point3D m_rotation_PiXY_centroid, m_rotation_casted_centroid;
    Point3D m_translation_P, m_translation_O;
    double m_scale;
    Point3D m_image_centroid;
    double currentMSE;
    int currentIdx;

    /// Effect of casting
    cv::Mat m_transformationMatrix;

};

#endif // CASTER_H
