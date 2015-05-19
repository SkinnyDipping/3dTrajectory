#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "data_types.h"

#define SQ(a) ((a)*(a))

class Vector
{
public:
    float x, y, z;

    Vector() : x(0), y(0), z(0) {}
    Vector(float x, float y, float z) : x(x), y(y), z(z) {}

    float* getFloat();
};

class Algebra
{
public:
    Algebra();
    ~Algebra();

    /// Computes cross product of vectors
    static Vector crossProduct(Vector a, Vector b);

    /// Computes cross product of vectors: [a[0], a[1], a[2]] x [b[0], b[1], b[2]]
    static Vector crossProduct(float* a, float* b);

    static float distance(Point2D a, Point2D b);
};

#endif // ALGEBRA_H
