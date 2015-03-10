#ifndef CASTER_H
#define CASTER_H

#include <vector>

#include <QQuaternion>

#include "data_types.h"
#include "datacontainer.h"


class Caster
{
public:
    Caster();
    ~Caster();

    /**
     * @brief cloudToImage Casts cloud to image. Points in arguments are corresponding
     * @param cloudKeypoints keypoints in cloud
     * @param imageKeypoints keypoints in image
     * @return transformation matrix between image and cloud
     */
    cv::Mat cloudToImage(std::vector<Point3D> cloudKeypoints, std::vector<Point2D> imageKeypoints);

private:

//    std::vector<Point3D> imageOnPlane(Point3D)

    /**
     * @brief calculateTangentialPlaneCoeff
     *
     * Calculates and assigns (as members) coefficients of tangential plane
     * in tangential_point (which lies on a sphere)
     */
    void inline calculateTangentialPlaneCoeff();

    /**
     * @brief castCloudPoints Casts cloud keypoints on ABCD plane
     * @param cloudKeypoints
     * @return casted points
     */
    std::vector<Point3D> castCloudPoints(std::vector<Point3D> cloudKeypoints);

    /**
     * @brief castImagePoints Casts image keypoints on ABCD plane
     * @param imageKeypoints Points shall have z coordiate = 0
     * @return casted points
     */
    std::vector<Point3D> castImagePoints(std::vector<Point2D> imageKeypoints);

    /**
     * @brief MSE Computes Mean Square Error between two sets of points
     * @param set1
     * @param set2
     * @return MSE
     */
    double MSE(std::vector<Point3D> set1, std::vector<Point3D> set2);

    /**
     * @brief distance Computes Carthesian distance between two points
     * @param p1
     * @param p2
     * @return distance
     */
    double distance(Point3D p1, Point3D p2);

private:

    //Tangential plane coefficients:
    //Ax + By + Cz + D = 0
    double A, B, C, D;

    //Virtual sphere properties: center and radius
    Point3D sphere_center;
    double sphere_radius;

    Point3D tangential_point;
    QQuaternion quaternion;
    Point3D scaleFactor;
    double minMSE;

    bool cloudCasted, imageCasted;

};

#endif // CASTER_H
