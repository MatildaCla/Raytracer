#include "phong.h"

Phong::Phong(Vector3D diffuseColor, Vector3D specularColor, int specIntensity)
{
    kd = diffuseColor;
    ks = specularColor;
    sn = specIntensity;
}

Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const
{
    Vector3D wr = Vector3D(2,2,2) * dot(n, wi) * n - wi;    //reflection Ray

    Vector3D diffuse = kd * std::max(dot(n, wi), (double) 0.0);        //Diffuse Component
    Vector3D specular = ks * std::pow(std::max(dot(wr, wo), (double) 0.0), (double) sn);    //Specular Component
    return diffuse + specular;
    
}

bool Phong::hasSpecular() const { return false; }
bool Phong::hasTransmission() const { return false; }
bool Phong::hasDiffuseOrGlossy() const { return true; }

double Phong::getIndexOfRefraction()
{
    return -1;
}

Vector3D Phong::getDiffuseCoefficient() const
{
    return kd;
}