#include "depthshader.h"
#include "..\core\utils.h"

depthShader::depthShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

depthShader::depthShader(Vector3D hitColor_, Vector3D bgColor_, double maxDistance) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D depthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList, Intersection& its) const
{
    if (Utils::hasIntersection(r, objList))
    {
        Utils::getClosestIntersection(r, objList, its);

        //sqrt: Compute length of the ray to intersection
        double ci = std::max(1 - ((std::sqrt(its.itsPoint.x * its.itsPoint.x + its.itsPoint.y * its.itsPoint.y +
            its.itsPoint.z * its.itsPoint.z) - std::sqrt(r.o.x * r.o.x + r.o.y * r.o.y +
                r.o.z * r.o.z)) / maxDistance), (double)0);

        return Vector3D(0, ci, 0);

    }
    else
        return bgColor;
}