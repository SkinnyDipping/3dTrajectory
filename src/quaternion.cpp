/*
 * Quaternion.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: michal
 */

#include "quaternion.h"

Quaternion::Quaternion() {
    this->w = 0;
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Quaternion::~Quaternion() {

}

Quaternion::Quaternion(double angle, Point3D vector) {

    this->w = cos(angle / 2);
    double sinus = sin(angle / 2);
    this->x = vector.x * sinus;
    this->y = vector.y * sinus;
    this->z = vector.z * sinus;
}

Quaternion::Quaternion(double w, double x, double y, double z) {
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

void Quaternion::normalize() {
    double M = sqrt(w * w + x * x + y * y + z * z);
    x = x / M;
    y = y / M;
    z = z / M;
    w = w / M;
}

Quaternion Quaternion::conj() {
    return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::inv() {
    Quaternion temp = this->conj();
    double N = temp.norm();
    temp.w = temp.w / N;
    temp.x = temp.x / N;
    temp.y = temp.y / N;
    temp.z = temp.z / N;
    return temp;
}

double Quaternion::norm() {
    return sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::operator*(Quaternion& q) {
    double newW = w * q.w - x * q.x - y * q.y - z * q.z;
    double newX = w * q.x + x * q.w + y * q.z - z * q.y;
    double newY = w * q.y - x * q.z + y * q.w + z * q.x;
    double newZ = w * q.z + x * q.y - y * q.x + z * q.w;
    return Quaternion(newW, newX, newY, newZ);
}

cv::Mat_<double> Quaternion::toTransformationMatrix() {
    double xx = x * x, yy = y * y, zz = z * z, xy = x * y, xz = x * z, yz = y
            * z, wx = w * x, wy = w * y, wz = w * z;
    cv::Mat_<double> output = cv::Mat(4, 4, CV_64F);

    output(0, 0) = 1 - 2 * (yy + zz);
    output(0, 1) = 2 * (xy - wz);
    output(0, 2) = 2 * (xz + wy);
    output(0, 3) = 0;

    output(1, 0) = 2 * (xy + wz);
    output(1, 1) = 1 - 2 * (xx + zz);
    output(1, 2) = 2 * (yz - wx);
    output(1, 3) = 0;

    output(2, 0) = 2 * (xz - wy);
    output(2, 1) = 2 * (yz + wx);
    output(2, 2) = 1 - 2 * (xx + yy);
    output(2, 3) = 0;

    output(3, 0) = 0;
    output(3, 1) = 0;
    output(3, 2) = 0;
    output(3, 3) = 1;

    return output;
}

Point3D Quaternion::rotate(Point3D point, Quaternion q,
        Point3D finalCentroid) {

    //Translation to center
//	point.x -= rotationCenter.x;
//	point.y -= rotationCenter.y;
//	point.z -= rotationCenter.z;

    //Rotation
    Quaternion v = Quaternion(0, point.x, point.y, point.z);
    Quaternion temp = q.inv();
    Quaternion rotated = (q * v) * temp;

    //Reverse translation
    return Point3D(rotated.x + finalCentroid.x, rotated.y + finalCentroid.y,
            rotated.z + finalCentroid.z);
}

std::vector<Point3D>& Quaternion::rotate(std::vector<Point3D>& points, Quaternion q,
        Point3D finalCentroid) {

    Point3D centroid = Point3D();
    for (int i=0; i<points.size(); i++)
    {
        centroid.x += points[i].x;
        centroid.y += points[i].y;
        centroid.z += points[i].z;
    }
    centroid.x /= points.size();
    centroid.y /= points.size();
    centroid.z /= points.size();

    for (int i = 0; i < points.size(); i++) {
        //Translation to center
        points[i].x -= centroid.x;
        points[i].y -= centroid.y;
        points[i].z -= centroid.z;

        //Rotation
        Quaternion v = Quaternion(0, points[i].x, points[i].y, points[i].z);
        Quaternion temp = q.inv();
        Quaternion rotated = (q * v) * temp;



        points[i].x = rotated.x+finalCentroid.x;
        points[i].y = rotated.y+finalCentroid.y;
        points[i].z = rotated.z+finalCentroid.z;
//        points[i].x = rotated.x;
//        points[i].y = rotated.y;
//        points[i].z = rotated.z;
//        points[i].x += centroid.x;
//        points[i].y += centroid.y;
//        points[i].z += centroid.z;

    }

    return points;
}
