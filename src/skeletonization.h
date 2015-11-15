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

    /**
     * @brief getForeground
     * @return segmented foreground
     */
    cv::Mat& getForeground();

    /**
     * @brief getJoints
     * @return vector of joints (2xfoot, 2xhand, 1xhead)
     */
    std::vector<Point2D> getJoints();

    /**
     * @brief getFeet
     * @return 2 points: feet
     */
    std::pair<Point2D, Point2D> getFeet();

    /**
     * @brief apply apply skeletonization to frame
     * @param frame
     * @return true, if all went well
     */
    bool apply(cv::Mat& frame, int mode);

private:
    /**
     * @brief thresholding apply simple thresholding
     * @param frame frame to be thresholded
     * @param threshold
     * @param highPass true, if values greater or equal shall be preserved. False otherwise
     */
    void thresholding(cv::Mat &frame, int threshold, bool highPass = true);
    
    /**
     * @brief sort Sorting border points
     *
     * Primitive method sorts border points. After operation they are aligned
     * so that next point is a neighbour of previous on border.
     *
     * @param points
     * @return new vector of sorted points
     */
    std::vector<Point2D> sort(std::list<Point2D> &points);

    /**
     * @brief findExtremas find extremas using Persistence1D
     * @param points vector of points
     * @param persistence persistence of points
     */
    void findExtremas(std::vector<Point2D>& points, int persistence);

    /**
     * @brief skeletonization perform skeletonization
     * @param pixels points of human border on image
     * @return true on success
     */
    bool skeletonization(std::list<Point2D> &pixels);

private:
    /// segmented image
    cv::Mat m_foreground;

    /// mixture of gaussians
    cv::Ptr<cv::BackgroundSubtractorMOG2> m_cvMOG2;

    /// erosion/dilatation kernel
    cv::Mat m_morph_kernel;

    /// vector with joints
    /// [0] - skeleton center
    /// other joint are not classified
    std::vector<Point2D> m_joints;

    p1d::Persistence1D p1d_program;

    Point2D centroid;

};

#endif // SKELETONIZATION_H
