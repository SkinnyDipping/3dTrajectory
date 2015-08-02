#include "skeletonization.h"

Skeletonization::Skeletonization(cv::Size frameResolution)
{
    m_foreground = cv::Mat::zeros(frameResolution, CV_8U);
    m_cvMOG2 = cv::createBackgroundSubtractorMOG2();
    m_morph_kernel = cv::Mat::ones(3, 3, CV_8U);

//    for (int i=0; i<3; i++)
//        m_joints.push_back(Point2D(-1,-1));
    m_joints.push_back(Point2D(0,0));
}

cv::Mat& Skeletonization::getForeground()
{
    return m_foreground;
}

std::vector<Point2D> Skeletonization::getJoints()
{
    return m_joints;
}

std::pair<Point2D, Point2D> Skeletonization::getFeet()
{
    Point2D _2 = Point2D();
    Point2D _1 = m_joints[0];
    for (Point2D j : m_joints) {
        if (j.y > _1.y)
            _1 = j;
    }
    return std::pair<Point2D, Point2D>(_1, _2);
}

Skeletonization::~Skeletonization()
{

}

bool Skeletonization::apply(cv::Mat &frame)
{
    m_cvMOG2->apply(frame, m_foreground);
    thresholding(m_foreground, 255);
    cv::medianBlur(m_foreground, m_foreground, 3);
    cv::erode(m_foreground, m_foreground, m_morph_kernel, cv::Point(-1,-1), 1);
    cv::dilate(m_foreground, m_foreground, m_morph_kernel, cv::Point(-1, -1), 4);
    cv::Canny(m_foreground, m_foreground, 1, 3);

    std::list<Point2D> pixels;
    cv::MatIterator_<uchar> it, end;
    for (it = m_foreground.begin<uchar>(), end = m_foreground.end<uchar>(); it != end; it++)
        if (*it == (uchar)255) {
            pixels.push_back(Point2D(it.pos().x, it.pos().y));
        }

    bool skl = skeletonization(pixels);

    cv::cvtColor(m_foreground, m_foreground, cv::COLOR_GRAY2BGR);
    return skl;
}

void Skeletonization::thresholding(cv::Mat& frame, int threshold, bool highPass)
{
    cv::MatIterator_<uchar> it, end;
    it = frame.begin<uchar>();
    end = frame.end<uchar>();
    if (highPass) {
        for (; it != end; it++) {
            if (*it < (uchar)threshold)
                *it = (uchar)0;
        }
    } else {
        for(; it != end; it++) {
            if (*it >= threshold)
                *it =  0;
        }
    }
}

bool Skeletonization::skeletonization(std::list<Point2D>& pixels)
{
    if (pixels.size() == 0) {
        m_joints[0] = Point2D(-1, -1);
        return false;
    }

    m_joints.clear();
    //calculate centroid
    Point2D centroid = Point2D();
    for (std::list<Point2D>::iterator it = pixels.begin(); it != pixels.end(); it++)
    {
        centroid.x += it->x;
        centroid.y += it->y;
    }
    centroid.x /= pixels.size();
    centroid.y /= pixels.size();
    m_joints.push_back(centroid);
    this->centroid = centroid;

    std::vector<Point2D> pts_sorted = sort(pixels);
    findExtremas(pts_sorted, 9);

    return true;
}

std::vector<Point2D> Skeletonization::sort(std::list<Point2D>& points)
{
    std::vector<Point2D> pts_sorted;
    typedef std::list<Point2D>::iterator P_it;
    P_it ip_next;
    Point2D p_ref = points.front();
    points.pop_front();
    while(points.size() != 0) {
        float dist = 9999;
        for (P_it it = points.begin(); it != points.end(); it++) {
            float d = Algebra::distance(p_ref, *it);
            if (d < dist) {
                dist = d;
                ip_next = it;
            }
        }
        pts_sorted.push_back(Point2D(*ip_next));
        p_ref = *ip_next;
        points.erase(ip_next);
    }
    return pts_sorted;
}

void Skeletonization::findExtremas(std::vector<Point2D>& points, int persistence)
{
    std::vector<float> distances;
    for (unsigned int i=0; i<points.size(); i++)
        distances.push_back(Algebra::distance(centroid, points[i]));

    if (!p1d_program.RunPersistence(distances))
        qDebug() << "P1D error";
    std::vector<p1d::TPairedExtrema> extremas;
    p1d_program.GetPairedExtrema(extremas, persistence);

    for (unsigned int i=0; i<extremas.size(); i++)
    {
        m_joints.push_back(points[extremas[i].MaxIndex]);
    }

}















