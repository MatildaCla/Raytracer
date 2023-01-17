#include "directShader.h"
#include "..\core\utils.h"
#include "..\materials\mirror.h"

#include <cmath>

DirectShader::DirectShader(int maxBounces)
{
    hitColor = (Vector3D(1, 0, 0));
    maxRayDepth = maxBounces;
}

DirectShader::DirectShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList, Intersection& its) const
{
    if (Utils::hasIntersection(r, objList))
    {
        Vector3D color = Vector3D(0, 0, 0);
            for (size_t counter = 0; counter < lsList.size(); counter++)
            {
                const PointLightSource light = lsList.at(counter);

                Utils::getClosestIntersection(r, objList, its);
                Vector3D p = its.itsPoint;
                Vector3D intensity = light.getIntensity(p);
                Vector3D wi = light.getPosition() - p;  //Light direction

                //Compute Visibility
                Intersection lightIts;
                Utils::getClosestIntersection((Ray(p, wi.normalized())), objList, lightIts);

                bool isVisible = !Utils::hasIntersection(Ray(p, wi.normalized()), objList) ||   //No intersection OR
                    abs(Vector3D(lightIts.itsPoint - p).length()) > abs(wi.length());       //Intersection is farther away than the light


                //Compute Material attributes
                Vector3D refrectance(0, 0, 0);

                //Phong
                if (its.shape->getMaterial().hasDiffuseOrGlossy())
                    refrectance = its.shape->getMaterial().getReflectance(its.normal.normalized(), (r.o - p).normalized(), wi.normalized()) * isVisible;

                //Mirror
                if (its.shape->getMaterial().hasSpecular())
                {
                    // Compute Reflection ray
                    Vector3D n = its.normal.normalized();   //normal
                    Vector3D wo = (r.o - p).normalized();   //camera ray

                    if (r.depth <= maxRayDepth)
                    {
                        Vector3D wr = Vector3D(2, 2, 2) * dot(wo, n) * n - wo;  //reflection direction
                        Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
                        refrectance = computeColor(reflectionRay, objList, lsList, its);
                    }
                    else
                        refrectance = Vector3D(1, 1, 1);
                }
                
                //Transmissive
                if (its.shape->getMaterial().hasTransmission())
                {
                    Vector3D wo = -r.d.normalized();    //Camera Ray
                    Vector3D n = its.normal.normalized();   //Surface Normal
                    float n1 = 1;   //Current Medium (assumed to be air)
                    float n2 = its.shape->getMaterial().getIndexOfRefraction();     //Object Medium (IOR)

                    // Check if Ray is inside Object and change Medium and Normal accordingly
                    if (r.isInsideObj)
                    {
                        n1 = n2;
                        n2 = 1;
                        n = -n;
                    }
                    //std::cout << n1 << std::endl;
                    float nt = n2 / n1;

                    float cosAlpha = dot(n, wo);
                    float sin2Alpha = 1 - pow(cosAlpha, 2);
                    float rad = 1 - pow(nt, 2) * sin2Alpha;

                    //Total internal reflection
                    if (rad < 0)
                    {
                        if (r.depth <= maxRayDepth)
                        {
                            Vector3D wr = Vector3D(2, 2, 2) * dot(wo, n) * n - wo;  //reflection direction
                            Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1);
                            refrectance = computeColor(reflectionRay, objList, lsList, its);
                        }

                        else
                            refrectance = Vector3D(1, 1, 1);
                    }

                    //Transmission
                    else
                    {
                        if (r.depth <= maxRayDepth)
                        {
                            //Vector3D wt = (Vector3D) - sqrt(rad) + (Vector3D)nt * dot(n, wo) * n - (Vector3D)nt * wo;
                            Vector3D wt = (Vector3D)(-sqrt(1 - pow(nt,2) * sin2Alpha) + nt * dot(wo, n)) * n - (Vector3D) nt * wo;
                            Ray transmissionRay = Ray(its.itsPoint, wt, r.depth + 1, !r.isInsideObj);
                            refrectance = computeColor(transmissionRay, objList, lsList, its);
                        }
                        else
                            refrectance = Vector3D(1, 0, 0);
                    }
                    
                }
                
                color += intensity * refrectance;
            }

          return color;
        }
    
    else
        return bgColor;
}
