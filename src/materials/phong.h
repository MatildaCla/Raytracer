#pragma once
#ifndef PHONG
#define PHONG
#include "material.h"

class Phong : public Material
{
public:
	Phong(Vector3D diffuseColor, Vector3D specularColor, int specIntensity);

	Vector3D getReflectance(const Vector3D& n, const Vector3D& wo,
		const Vector3D& wi) const;

	Vector3D kd;
	Vector3D ks;
	int sn;

	bool hasSpecular() const;
	bool hasTransmission() const;
	bool hasDiffuseOrGlossy() const;
	double getIndexOfRefraction();

	Vector3D getDiffuseCoefficient() const;


};

#endif // PHONG