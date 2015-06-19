#include "algebra.h"

Algebra::Algebra()
{

}

Algebra::~Algebra()
{

}

float Algebra::distance(Point2D a, Point2D b)
{
    float output;
    output = sqrt(SQ(a.x-b.x) + SQ(a.y-b.y));
    return output;
}

void Algebra::normalizeVector(Point3D &vector) {
    double len = length(vector);
    vector.x /= len;
    vector.y /= len;
    vector.z /= len;
}

double Algebra::length(Point3D p) {
    return sqrt(SQ(p.x) + SQ(p.y) + SQ(p.z));
}

double Algebra::dotProduct(Point3D a, Point3D b)
{
    double output = 0;
    output += a.x*b.x;
    output += a.y*b.y;
    output += a.z*b.z;
    return output;
}

Point3D Algebra::crossProduct(Point3D a, Point3D b)
{
    Point3D output = Point3D();
    output.x = a.y*b.z - a.z*b.y;
    output.y = a.z*b.x - a.x*b.z;
    output.z = a.x*b.y - b.x*a.y;
    return output;
}
