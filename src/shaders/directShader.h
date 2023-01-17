#ifndef DIRECTSHADER_H
#define DIRECTSHADER_H

#include "shader.h"

class DirectShader : public Shader
{
public:
    DirectShader(int maxBounces);
    DirectShader(Vector3D hitColor, Vector3D bgColor_);

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList,
        Intersection& its) const;

    Vector3D hitColor;
    int maxRayDepth;
};

#endif // DIRECTSHADER_H