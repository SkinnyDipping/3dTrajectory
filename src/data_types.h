#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <QDebug>

#define DEG2RAD 0.0174532925f
#define RAD2DEG 57.2957795f


class Point3D {
public:
    float x, y, z;

    Point3D() : x(0), y(0), z(0) {}

    Point3D(float x, float y, float z) : x(x),y(y),z(z) {}

    Point3D operator+(Point3D& p) {
        return Point3D(this->x+p.x,this->y+p.y,this->z+p.z);
    }

    Point3D operator+=(Point3D& p) {
        this->x += p.x;
        this->y += p.y;
        this->z += p.z;
        return *this;
    }

    friend QDebug operator<<(QDebug dbg, const Point3D& p) {
        dbg.nospace() << "["<<p.x<<"; "<<p.y<<"; "<<p.z<<"]";
        return dbg.maybeSpace();
    }

};

class Point3D_RGB {
public:
    float x, y, z;
    int r, g, b;

    Point3D_RGB(float x, float y, float z, int r, int g, int b) : x(x),y(y),z(z),r(r),g(g),b(b) {}
    Point3D_RGB(Point3D& p) : x(p.x),y(p.x),z(p.z),r(255),g(255),b(255) {}
};

class Point2D {
public:
    float x, y;

    Point2D() : x(0), y(0) {}

    Point2D(float x, float y) : x(x), y(y) {}

    Point2D operator+(Point2D& p) {
        return Point2D(this->x+p.x, this->y+p.y);
    }

    Point2D operator+=(Point2D& p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    }

    friend QDebug operator<<(QDebug dbg, const Point2D& p) {
        dbg.nospace() << "["<<p.x<<"; "<<p.y<<"]";
        return dbg.maybeSpace();
    }
};

typedef std::vector<Point3D> PointCloud;
typedef Point3D_RGB Point3DRGB;
typedef Point3D Vector3D;

#endif // DATA_TYPES_H

