#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <QDebug>

class CloudPoint {
public:
    float x, y, z;


    CloudPoint(float x, float y, float z) : x(x),y(y),z(z) {}

    CloudPoint operator+(CloudPoint& p) {
        return CloudPoint(this->x+p.x,this->y+p.y,this->z+p.z);
    }

    friend QDebug operator<<(QDebug dbg, const CloudPoint& p) {
        dbg.nospace() << "["<<p.x<<"; "<<p.y<<"; "<<p.z<<"]";
        return dbg.maybeSpace();
    }
//    std::ostream operator<<(std::ostream os, const CloudPoint& p) {
//        os << "["<<p.x<<"; "<<p.y<<"; "<<p.z<<"]";
//        return os;
//    }

};

class CloudPointRGB {
public:
    float x, y, z;
    int r, g, b;

    CloudPointRGB(float x, float y, float z, int r, int g, int b) : x(x),y(y),z(z),r(r),g(g),b(b) {}
    CloudPointRGB(CloudPoint& p) : x(p.x),y(p.x),z(p.z),r(255),g(255),b(255) {}
};

#endif // DATA_TYPES_H

