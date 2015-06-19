/*
 * Quaternion.h
 *
 *  Created on: Sep 4, 2014
 *      Author: michal
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <ostream>
#include <assert.h>

#include "opencv2/core.hpp"
#include "data_types.h"

class Quaternion {

private:
	double w, x, y, z;

public:
	Quaternion();
	virtual ~Quaternion();
	//	Quaternion(double w, double x, double y, double z) {
	//		this->w = w;
	//		this->x = x;
	//		this->y = y;
	//		this->z = z;
	//	}

	Quaternion(double angle, Point3D vector);
	Quaternion(double w, double x, double y, double z);

	//	Quaternion(double angle, vector<double> vector) {
	//		PointXYZ Pvector = PointXYZ(vector[0], vector[1], vector[2]);
	//		Quaternion(angle, Pvector);
	//	}

	void normalize();
	Quaternion operator*(Quaternion& q);
	Quaternion inv();
	Quaternion conj();
	double norm();

	cv::Mat_<double> toTransformationMatrix();

	static Point3D rotate(Point3D point, Quaternion q, Point3D rotationPoint =
			Point3D());
	static std::vector<Point3D>& rotate(std::vector<Point3D>& point,
			Quaternion q, Point3D rotationPoint = Point3D());

	friend std::ostream& operator<<(std::ostream& os, const Quaternion& p) {
		os << "<" << p.w << "; " << p.x << ", " << p.y << ", " << p.z << ">";
		return os;
	}

    friend QDebug operator<<(QDebug dbg, const Quaternion& q) {
        dbg.nospace() << "<"<<q.w<<"; ["<<q.x<<", "<<q.y<<", "<<q.z<<"]>";
        return dbg.maybeSpace();
    }

};

#endif /* QUATERNION_H_ */
