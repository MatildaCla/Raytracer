#include "normalShader.h"
#include "..\core\utils.h"

NormalShader::NormalShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

NormalShader::NormalShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D NormalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList, Intersection& its) const
{
    if (Utils::hasIntersection(r, objList))
    {
        Utils::getClosestIntersection(r, objList, its);
        return Vector3D((its.normal.x + 1.0) / 2, (its.normal.y + 1.0) / 2, (its.normal.z + 1.0) / 2);
    }
    else
        return bgColor;
}
