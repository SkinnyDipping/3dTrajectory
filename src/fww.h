/**
  * Fotogrametryczne wcięcie wstecz
  *
  */
#ifndef FWW_H_
#define FWW_H_

#include <vector>

#include "opencv2/core.hpp"

#include "data_types.h"

class FWW {

public:

    FWW();
    ~FWW();

    void cast(std::vector<Point3D>& cloudKeypoints, std::vector<Point2D>& imageKeypoints);

    Point2D getImageCenter();
    Point3D getCenterCast();
    cv::Mat getRotationMatrix();
    double getFs();

private:
    Point2D m_imageCenter;
    Point3D m_centerCast;
    double m_fs;
    cv::Mat m_rotationMatrix;



};

#endif
