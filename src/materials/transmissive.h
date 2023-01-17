#pragma once
#include "material.h"
class Transmissive : public Material
{
public:
	Transmissive(double indexOfRefraction);

	Vector3D getReflectance(const Vector3D& n, const Vector3D& wo,
		const Vector3D& wi) const;

	double ior;

	bool hasSpecular() const;
	bool hasTransmission() const;
	bool hasDiffuseOrGlossy() const;
	double getIndexOfRefraction() const;
};

