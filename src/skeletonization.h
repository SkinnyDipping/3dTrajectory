#ifndef SKELETONIZATION_H
#define SKELETONIZATION_H

#include <list>
#include <iterator>

#include <QDebug>
#include "opencv2/opencv.hpp"
#include "persistence1d.hpp"

#include "data_types.h"
#include "algebra.h"

/**
 * @brief The Skeletonization class
 *
 * Class that performes basic "star" skeletonization [Fujiyoshi, Lipton].
 * Apart from this, it performs basic image processing routines,
 * proper for this skeletonization (e.g. Canny, MOG)
 */
class Skeletonization
{
public:
    Skeletonization(cv::Size frameResolution);
    ~Skeletonization();
    cv::Mat& getForeground();
    std::vector<Point2D> getJoints();

    /**
     * @brief apply apply skeletonization to frame
     * @param frame
     * @return true, if all went well
     */
    bool apply(cv::Mat& frame);

private:
    /**
     * @brief thresholding apply simple thresholding
     * @param frame frame to be thresholded
     * @param threshold
     * @param highPass true, if values greater or equal shall be preserved. False otherwise
     */
    void thresholding(cv::Mat &frame, int threshold, bool highPass = true);
    
    std::vector<Point2D> sort(std::list<Point2D> &points);

    std::vector<int> findExtremas(std::vector<Point2D>& points, Point2D centroid, int mask);
    void findExtremas(std::vector<Point2D>& points);

    bool skeletonization(std::list<Point2D> &pixels);

//TODO change to private
public:
    /// currently processed frame TODO(?)

    /// segmented image
    cv::Mat m_foreground;

    /// mixture of gaussians
    cv::Ptr<cv::BackgroundSubtractorMOG2> m_cvMOG2;

    /// erosion/dilatation kernel
    cv::Mat m_morph_kernel;

    /// vector with joints
    /// [0] - skeleton center
    /// [1] - right leg
    /// [2] - left leg
    /// [3] - head
    std::vector<Point2D> m_joints;

    p1d::Persistence1D p1d_program;

    Point2D centroid; //TMEP

};

#endif // SKELETONIZATION_H
