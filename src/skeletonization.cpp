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
    //        qDebug()<<"\nFRAME:";
    //        for (unsigned int i=0; i<pts_sorted.size(); i++)
    //            qDebug() << pts_sorted[i].x<<" "<<pts_sorted[i].y;
    //        qDebug()<<"Extremas: "<<extremas.size();
    //        for (unsigned int i=0; i<extremas.size(); i++)
    //            qDebug()<<extremas[i];

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
    this->centroid = centroid; //tEMP

    std::vector<Point2D> pts_sorted = sort(pixels);
    //    std::vector<int> extremasIdx = findExtremas(pts_sorted, centroid, 6);
    findExtremas(pts_sorted);

    //    for (unsigned int i=0; i<extremasIdx.size(); i++)
    //        m_joints.push_back(pts_sorted[extremasIdx[i]]);

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

void Skeletonization::findExtremas(std::vector<Point2D>& points)
{
    std::vector<float> distances;
    for (unsigned int i=0; i<points.size(); i++)
        distances.push_back(Algebra::distance(centroid, points[i]));

    if (!p1d_program.RunPersistence(distances))
        qDebug() << "P1D error";
    std::vector<p1d::TPairedExtrema> extremas;
    p1d_program.GetPairedExtrema(extremas, 2);
    qDebug()<<extremas.size();

//    for(std::vector<p1d::TPairedExtrema>::iterator it; it != extremas.end(); it++)
//    {
//        m_joints.push_back(points[it->MinIndex]);
//    }

    for (int i=0; i<extremas.size(); i++)
    {
        m_joints.push_back(points[extremas[i].MinIndex]);
    }

}

std::vector<int> Skeletonization::findExtremas(std::vector<Point2D>& points, Point2D centroid, int mask)
{
    // mask shall be even
    if (mask % 2 != 0) mask++;

    std::vector<float> distances;
    for (unsigned int i=0; i<points.size(); i++)
        distances.push_back(Algebra::distance(centroid, points[i]));

    std::vector<int> extremas;
    int mins=0,maxs=0;
    for (unsigned int i=mask/2; i<distances.size()-mask/2; i++)
    {
        int min = 1, max = 1;
        for (int j=1; j<=mask/2; j++)
        {
            if (j<0) continue;

            if(distances[i+j] <= distances[i])
                min *= 0;
            else if (distances[i+j] >= distances[i])
                max *= 0;

            if(distances[i-j] <= distances[i])
                min *= 0;
            else if (distances[i-j] >= distances[i])
                max *= 0;
        }

        if (min || max)
            extremas.push_back(i);
        if(min) mins++;
        if(max) maxs++;
    }

    return extremas;

}
















