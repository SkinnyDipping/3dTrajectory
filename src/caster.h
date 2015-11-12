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

typedef std::vector< std::vector< std::vector<Point3D> > > t_3Darray;

class Caster
{
public:
    static Caster& instance() {
        static Caster caster;
        return caster;
    }

    bool cast(PointCloud& point_cloud, std::vector<Point2D>& image_keypoints, std::vector<Point3D>& cloud_keypoints, Point2D img_res);
    std::vector<Point3D> *getPixelCoordinates();
    std::vector<Point3D> &getPoint(Point2D pixel);
    void printPixelCoordinates();

private:
    Caster();
    Caster(const Caster &) {}
    ~Caster() {}

    void performFWW(const std::vector<Point2D>& image_keypoints, const std::vector<Point3D>& cloud_keypoints);
    Point2D calcImagePoint(Point3D &cloud_point);

private:
    // Dynamic lookup table containing points coordinates
    std::vector<Point3D>* m_pixelCoordinates;

    bool m_FWWperformed;
    double m_x0, m_y0, m_fs;
    Point3D m_centerCast;
    cv::Mat m_rotationMatrix;
    Point2D m_resolution;

};

#endif // CASTER_H
