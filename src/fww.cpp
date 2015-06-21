#include "fww.h"

FWW::FWW()
{
}

FWW::~FWW()
{
}

Point3D FWW::getCenterCast()
{
    return m_centerCast;
}

Point2D FWW::getImageCenter()
{
    return m_imageCenter;
}

double FWW::getFs()
{
    return m_fs;
}

cv::Mat FWW::getRotationMatrix()
{
    return m_rotationMatrix;
}

void FWW::cast(std::vector<Point3D> &cloudKeypoints, std::vector<Point2D> &imageKeypoints)
{
    int n_points = cloudKeypoints.size();
    cv::Mat_<double> A = cv::Mat::zeros(n_points*2, 11, CV_64F);
    cv::Mat_<double> l = cv::Mat::zeros(n_points*2, 1, CV_64F);
    for (int i=0; i<n_points; i++)
    {
        A(2*i+0,0) = cloudKeypoints[i].x;
        A(2*i+0,1) = cloudKeypoints[i].y;
        A(2*i+0,2) = cloudKeypoints[i].z;
        A(2*i+0,3) = 1;
        A(2*i+0,8) = -imageKeypoints[i].x * cloudKeypoints[i].x;
        A(2*i+0,9) = -imageKeypoints[i].x * cloudKeypoints[i].y;
        A(2*i+0,10)= -imageKeypoints[i].x * cloudKeypoints[i].z;

        A(2*i+1,4) = cloudKeypoints[i].x;
        A(2*i+1,5) = cloudKeypoints[i].y;
        A(2*i+1,6) = cloudKeypoints[i].z;
        A(2*i+1,7) = 1;
        A(2*i+1,8) = -imageKeypoints[i].y * cloudKeypoints[i].x;
        A(2*i+1,9) = -imageKeypoints[i].y * cloudKeypoints[i].y;
        A(2*i+1,10)= -imageKeypoints[i].y * cloudKeypoints[i].z;

        l(2*i+0) = imageKeypoints[i].x;
        l(2*i+1) = imageKeypoints[i].y;
    }

    cv::Mat_<double> L = (A.t() * A).inv() * A.t() * l;

    double x0 = ( L(1)*L(9) + L(2)*L(10) + L(3)*L(11) ) / ( L(9)*L(9) + L(10)*L(10) + L(11)*L(11) );
    double y0 = ( L(5)*L(9) + L(6)*L(10) + L(7)*L(11) ) / ( L(9)*L(9) + L(10)*L(10) + L(11)*L(11) );
    m_imageCenter = Point2D(x0, y0);
    m_fs = sqrt( ( (x0*L(9)-L(1))*(x0*L(9)-L(1)) + (x0*L(10)-L(2))*(x0*L(10)-L(2)) +
                        (x0*L(11)-L(3))*(x0*L(11)-L(3)) + (y0*L(9)-L(5))*(y0*L(9)-L(5)) +
                        (y0*L(10)-L(6))*(y0*L(10)-L(6)) + (y0*L(11)-L(7))*(y0*L(11)-L(7)) ) /
                      ( 2*( L(9)*L(9) + L(10)*L(10) + L(11)*L(11) ) ) );
    double _D = -1 / sqrt( L(9)*L(9) + L(10)*L(10) + L(11)*L(11) );
    _D++;
}
