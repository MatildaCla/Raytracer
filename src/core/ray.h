#ifndef RAY_H
#define RAY_H

#include <string>
#include <sstream>

#include "vector3d.h"

#define Epsilon (double)1e-4

// Note: the "mutable" keyword allows to change a class instance
// poperty even if the instance is declared as "const"
class Ray
{
public:
    // Constructors
    Ray();
    Ray(const Vector3D &ori, const Vector3D &dir,
        size_t dep = 0, size_t lDep=0, bool isInside = false, double start = Epsilon,
        double end = INFINITY);
    // Member functions
    std::string toString() const;

    // Ray public data
    Vector3D o;          // Ray origin
    Vector3D d;          // Ray direction
    mutable double minT; //
    mutable double maxT; //
    size_t depth;        // Ray depth (or number of bounces)
    size_t lightDepth;      // Ray depth for light bounces (indirect illumination)
    mutable bool isInsideObj;
};

std::ostream &operator<<(std::ostream &out, const Ray &r);

#endif // RAY_H
