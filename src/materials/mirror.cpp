#include "mirror.h"

Mirror::Mirror()
{
}

Vector3D Mirror::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const
{
    return -1;
}

bool Mirror::hasSpecular() const { return true; }
bool Mirror::hasTransmission() const { return false; }
bool Mirror::hasDiffuseOrGlossy() const { return false; }

double Mirror::getIndexOfRefraction()
{
    return -1;
}