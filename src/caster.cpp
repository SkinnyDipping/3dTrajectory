#include "caster.h"

Caster::Caster()
{
    //DEBUG ONLY
}

Caster::Caster(PointCloud &cloud_keypoints, cv::Mat &image)
{
    qDebug() << "Caster created";
    this->cloud_keypoints = cloud_keypoints;
    this->image_keypoints = image;
    this->cloud_centroid = centroid;
}

Caster::~Caster()
{

}

std::vector<Point3D>& Caster::generateSphere(Point3D sphereCenter, float sphereRadius, float resolution)
{
    std::vector<Point3D> sphere;

    for (float phi=-180; phi<180; phi+=resolution)
        for (float theta=-180; theta<180; theta+=resolution)
        {
            Point3D p = Point3D();

            //Spherical to Carthesian
            p.x = sphereRadius * cos(theta) * cos(phi);
            p.y = sphereRadius * cos(theta) * sin(phi);
            p.z = sphereRadius * sin(theta);

            //Translation to sphere center
            p += sphereCenter;

            sphere.push_back(p);
        }

    return sphere;
}
