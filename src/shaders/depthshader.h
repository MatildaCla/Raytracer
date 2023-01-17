#ifndef DEPTHSHADER_H
#define DEPTHSHADER_H

#include "shader.h"

class depthShader : public Shader
{
public:
    depthShader();
    depthShader(Vector3D hitColor, Vector3D bgColor_, double maxDistance);

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList,
        Intersection& its) const;

    Vector3D hitColor;
    double maxDistance = 7;
};

#endif // DEPTHSHADER_H