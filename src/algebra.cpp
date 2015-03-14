#include "algebra.h"

float* Vector::getFloat()
{
    float output[] = {this->x, this->y, this->z};
    return output;
}

Algebra::Algebra()
{

}

Algebra::~Algebra()
{

}

Vector Algebra::crossProduct(Vector a, Vector b)
{
    Vector output = Vector();
    output.x = a.y*b.z-a.z*b.y;
    output.y = a.z*b.x - a.x*b.z;
    output.z = a.x*b.y - b.x*a.y;
    return output;
}

Vector Algebra::crossProduct(float *a, float *b)
{
    Vector output = Vector();
    output.x = a[1]*b[2] - a[2]*b[1];
    output.y = a[2]*b[0] - a[0]*b[2];
    output.z = a[0]*b[1] - b[0]*a[1];
    return output;
}

Vector Algebra::sphericalToCarthesian(float r, float phi, float theta)
{
    Vector output = Vector();
    output.x = r * cos(theta) * cos(phi);
    output.y = r * cos(theta) * sin(phi);
    output.z = r * sin(theta);
    return output;
}
