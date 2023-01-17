#include "transmissive.h"
#include <iostream>

Transmissive::Transmissive(double indexOfRefraction)
{
    ior = indexOfRefraction;
}

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const
{
    return -1;

}

bool Transmissive::hasSpecular() const { return false; }
bool Transmissive::hasTransmission() const { return true; }
bool Transmissive::hasDiffuseOrGlossy() const { return false; }

double Transmissive::getIndexOfRefraction() const
{
    return ior;
}