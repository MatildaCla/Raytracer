#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H

#include "shader.h"

class GlobalShader : public Shader
{
public:
    GlobalShader(int samples, int maxBounces, int maxLightBounces);
    GlobalShader(Vector3D hitColor, Vector3D bgColor_);

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList,
        Intersection& its) const;

    Vector3D hitColor;
    int maxRayDepth;
    int maxLightDepth;
    int nSamples;
};

#endif // GLOBALSHADER_H