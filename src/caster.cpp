#include "caster.h"

void debug(cv::Mat_<double> m, std::string msg){
    qDebug() << msg.c_str();
    qDebug() << m(0,0) << m(0,1) << m(0,2);
    qDebug() << m(1,0) << m(1,1) << m(1,2);
    qDebug() << m(2,0) << m(2,1) << m(2,2);
}


Caster::Caster() :
    m_FWWperformed(false),
    m_x0(-1), m_y0(-1), m_fs(-1),
    m_centerCast(Point3D(-1,-1,-1)),
    m_rotationMatrix(cv::Mat::eye(3,3,CV_64F))
{
}

bool Caster::cast(PointCloud &point_cloud,
                  std::vector<Point2D>& image_keypoints, std::vector<Point3D>& cloud_keypoints,
                  Point2D img_res)
{
    performFWW(image_keypoints, cloud_keypoints);
    qDebug()<<m_x0<<m_y0<<m_fs<<m_centerCast;
    debug(m_rotationMatrix, "rotation matrix");
    qDebug()<<"Point:"<<calcImagePoint(cloud_keypoints[0]);

    m_pixelCoordinates = new Point3D*[img_res.x];
    for(int i=0; i<img_res.x; i++)
        m_pixelCoordinates[i] = new Point3D[img_res.y];

    for (Point3D p : point_cloud)
    {
        Point2D img_pt = calcImagePoint(p);

        img_pt += Point2D(x0, y0);

        if( img_pt.x > img_res.x || img_pt.y > img_res.y ||
                img_pt.x < 0 || img_pt.y < 0)
            continue;

        // Interpolation
        if (img_pt.x - floor(img_pt.x) < 0.5)
            img_pt.x = floor(img_pt.x);
        else
            img_pt.x = ceil(img_pt.x);
        if (img_pt.y - floor(img_pt.y) < 0.5)
            img_pt.y = floor(img_pt.y);
        else
            img_pt.y = ceil(img_pt.y);

        m_pixelCoordinates
    }

    return true;
}

cv::Mat Caster::getPixelCoordinates()
{
    return m_pixelCoordinates;
}

void Caster::performFWW(const std::vector<Point2D>& image_keypoints, const std::vector<Point3D>& cloud_keypoints)
{
    assert( image_keypoints.size() == cloud_keypoints.size());
    int n_pts = cloud_keypoints.size();
    cv::Mat_<double> A = cv::Mat::zeros(2*n_pts, 11, CV_64F);
    cv::Mat_<double> l = cv::Mat::zeros(2*n_pts, 1, CV_64F);
    for (int i=0; i<n_pts; i++)
    {
        A(2*i+0, 0) = cloud_keypoints[i].x;
        A(2*i+0, 1) = cloud_keypoints[i].y;
        A(2*i+0, 2) = cloud_keypoints[i].z;
        A(2*i+0, 3) = 1;
        A(2*i+0, 8) = -image_keypoints[i].x * cloud_keypoints[i].x;
        A(2*i+0, 9) = -image_keypoints[i].x * cloud_keypoints[i].y;
        A(2*i+0, 10)= -image_keypoints[i].x * cloud_keypoints[i].z;

        A(2*i+1, 4) = cloud_keypoints[i].x;
        A(2*i+1, 5) = cloud_keypoints[i].y;
        A(2*i+1, 6) = cloud_keypoints[i].z;
        A(2*i+1, 7) = 1;
        A(2*i+1, 8) = -image_keypoints[i].y * cloud_keypoints[i].x;
        A(2*i+1, 9) = -image_keypoints[i].y * cloud_keypoints[i].y;
        A(2*i+1, 10)= -image_keypoints[i].y * cloud_keypoints[i].z;

        l(2*i+0) = image_keypoints[i].x;
        l(2*i+1) = image_keypoints[i].y;
    }

    cv::Mat_<double> L = (A.t()*A).inv() * A.t() * l;

    m_x0 = ( L(0)*L(8) + L(1)*L(9) + L(2)*L(10) ) / ( L(8)*L(8) + L(9)*L(9) + L(10)*L(10) );
    m_y0 = ( L(4)*L(8) + L(5)*L(9) + L(6)*L(10) ) / ( L(8)*L(8) + L(9)*L(9) + L(10)*L(10) );
    m_fs = sqrt( ( (m_x0*L(8)-L(0))*(m_x0*L(8)-L(0)) + (m_x0*L(9)-L(1))*(m_x0*L(9)-L(1)) +
                   (m_x0*L(10)-L(2))*(m_x0*L(10)-L(2)) + (m_y0*L(8)-L(4))*(m_y0*L(8)-L(4)) +
                   (m_y0*L(9)-L(5))*(m_y0*L(9)-L(5)) + (m_y0*L(10)-L(6))*(m_y0*L(10)-L(6)) ) /
                 ( 2* ( L(8)*L(8) + L(9)*L(9) + L(10)*L(10) ) )
               );

    cv::Mat_<double> _cc1 = cv::Mat::zeros(3,3,CV_64F);
    _cc1(0,0) = L(0); _cc1(0,1) = L(1); _cc1(0,2) = L(2);
    _cc1(1,0) = L(4); _cc1(1,1) = L(5); _cc1(1,2) = L(6);
    _cc1(2,0) = L(8); _cc1(2,1) = L(9); _cc1(2,2) = L(10);
    cv::Mat_<double> _cc2 = cv::Mat::ones(3,1,CV_64F);
    _cc2(0) = L(3); _cc2(1) = L(7);
    cv::Mat_<double> _cc = _cc1.inv() * _cc2;
    m_centerCast = Point3D(_cc(0), _cc(1), _cc(2));

    double _D = -1 / sqrt(L(9)*L(9) + L(10)*L(10) + L(11)*L(11));
    cv::Mat_<double> _r = cv::Mat::zeros(3,3,CV_64F);
    _r(0,0) = (m_x0*L(8)-L(0))/m_fs;
    _r(0,1) = (m_x0*L(9)-L(1))/m_fs;
    _r(0,2) = (m_x0*L(10)-L(2))/m_fs;
    _r(1,0) = (m_y0*L(8)-L(4))/m_fs;
    _r(1,1) = (m_y0*L(9)-L(5))/m_fs;
    _r(1,2) = (m_y0*L(10)-L(6))/m_fs;
    _r(2,0) = L(8); _r(2,1) = L(9); _r(2,2) = L(10);
    m_rotationMatrix = _D * _r;

}

Point2D Caster::calcImagePoint(Point3D& cloud_point)
{

    cv::Mat_<double> rm = m_rotationMatrix.inv();
    Point2D P = Point2D();
    Point3D cc = Point3D(-m_centerCast.x, -m_centerCast.y, -m_centerCast.z);
    Point3D& cp = cloud_point;

    P.x = -m_fs * ( rm(0,0)*(cp.x-cc.x) + rm(1,0)*(cp.y-cc.y) + rm(2,0)*(cp.z-cc.z) );
    P.x /= rm(0,2)*(cp.x-cc.x) + rm(1,2)*(cp.y-cc.y) + rm(2,2)*(cp.z-cc.z);
    P.y = -m_fs * ( rm(0,1)*(cp.x-cc.x) + rm(1,1)*(cp.y-cc.y) + rm(2,1)*(cp.z-cc.z) );
    P.y /= rm(0,2)*(cp.x-cc.x) + rm(1,2)*(cp.y-cc.y) + rm(2,2)*(cp.z-cc.z);

    return P;
}

void Caster::printPixelCoordinates()
{
    qDebug() << "Pixel coordinates:";

}
