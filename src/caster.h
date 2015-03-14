#ifndef CASTER_H
#define CASTER_H

#include <vector>

#include <QDebug>
#include "opencv2/core.hpp"

#include "algebra.h"
#include "data_types.h"

class Caster
{
public:
    Caster();
    Caster(PointCloud& cloud_keypoints, cv::Mat& image_keypoints);
    ~Caster();

    std::vector<Point3D>& generateSphere(Point3D sphereCenter, float sphereRadius, float resolution = 1);

private:
    //Tangential plane coefficients:
    //Ax + By + Cz + D = 0
    double A, B, C, D;

    PointCloud& cloud_keypoints;
    cv::Mat& image_keypoints;
};

#endif // CASTER_H
