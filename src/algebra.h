#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "data_types.h"

#define SQ(a) ((a)*(a))

class Algebra
{
public:
    Algebra();
    ~Algebra();

    /// Computes dor product of vectors
    static double dotProduct(Point3D a, Point3D b);

    /// Computes cross product of vectors
    static Point3D crossProduct(Point3D a, Point3D b);

    static double length(Point3D p);

    static void normalizeVector(Point3D& vector);

    static float distance(Point2D a, Point2D b);
};

#endif // ALGEBRA_H
