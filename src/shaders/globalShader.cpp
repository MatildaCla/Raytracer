#include "globalShader.h"

#include "..\core\utils.h"
#include "..\materials\mirror.h"
#include "..\core\hemisphericalsampler.h"

#include <cmath>

const double pi = 3.14159265358979323846;

GlobalShader::GlobalShader(int samples, int maxBounces, int maxlightBounces)
{
    hitColor = (Vector3D(1, 0, 0));
    nSamples = samples;
    maxRayDepth = maxBounces;
    maxLightDepth = maxlightBounces;
}

GlobalShader::GlobalShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D GlobalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList, Intersection& its) const
{
    if (Utils::hasIntersection(r, objList))
    {
        Vector3D color = Vector3D(0, 0, 0);
        for (size_t counter = 0; counter < lsList.size(); counter++)
        {
            const PointLightSource light = lsList.at(counter);

            // Get relevant points and vectors
            Utils::getClosestIntersection(r, objList, its);

            Vector3D p = its.itsPoint;  // shading point
            Vector3D intensity = light.getIntensity(p); //Light intensity at shading point

            Vector3D wi = (light.getPosition() - p).normalized();  //Light direction
            Vector3D wo = (r.o - p).normalized();   //camera ray
            Vector3D n = its.normal.normalized();   //Normal direction

            //Compute Visibility
            Intersection lightIts;
            Utils::getClosestIntersection((Ray(p, wi.normalized())), objList, lightIts);

            bool isVisible = !Utils::hasIntersection(Ray(p, wi.normalized()), objList) ||   //No intersection OR
                abs(Vector3D(lightIts.itsPoint - p).length()) > abs(wi.length());       //Intersection is farther away than the light



            //Compute Material attributes
            Vector3D refrectance(0, 0, 0);

            //Phong
            if (its.shape->getMaterial().hasDiffuseOrGlossy())
            {
                // Compute direct illumination
                Vector3D dirIllum = its.shape->getMaterial().getReflectance(its.normal.normalized(), (r.o - p).normalized(), wi.normalized());


                // Compute Indirect Illumination
                Vector3D indIllum = Vector3D(0, 0, 0);
                
                // At max light bounces return ambient Term faked global illumination
                if (r.lightDepth == maxLightDepth)
                {
                    double ambientTerm = 0.2;
                    indIllum = its.shape->getMaterial().getDiffuseCoefficient() * ambientTerm;
                }

                
                // Send rays for Global Illumination
                else if (r.lightDepth == 0)
                { 
                    HemisphericalSampler* hemi = new HemisphericalSampler();
                    
                    Vector3D addedColors = Vector3D(0, 0, 0);

                    // Create nSamples Rays in random directions and sum up their colors
                    for (size_t i = 0; i < nSamples; i++)
                    {
                        Ray secondaryRay = Ray(p, hemi->getSample(n), r.depth, r.lightDepth + 1, false);
                        addedColors += computeColor(secondaryRay, objList, lsList, its);
                    }

                    indIllum = addedColors / (2 * pi * nSamples);

                }
                
                else
                {
                    Vector3D addedColors = Vector3D(0, 0, 0);

                    // Instead of the nSamples amount only use two rays
                    // Create two Rays: one along the normal direction, one along the reflection direction
                    Ray normalRay = Ray(p, n, r.depth, r.lightDepth + 1, false);

                    Vector3D wr = Vector3D(2, 2, 2) * dot(wo, n) * n - wo;  //reflection direction
                    Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1, r.lightDepth + 1, false);

                    addedColors = computeColor(normalRay, objList, lsList, its) + computeColor(reflectionRay, objList, lsList, its);

                    indIllum = addedColors / (4 * pi);
                }
                
                refrectance = dirIllum * isVisible + indIllum;
            }

            //Mirror
            if (its.shape->getMaterial().hasSpecular())
            {
                if (r.depth <= maxRayDepth)
                {
                    Vector3D wr = Vector3D(2, 2, 2) * dot(wo, n) * n - wo;  //reflection direction
                    Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1, r.lightDepth, false);
                    refrectance = computeColor(reflectionRay, objList, lsList, its);
                }
                else
                    refrectance = Vector3D(0, 0, 0);
            }

            //Transmissive
            if (its.shape->getMaterial().hasTransmission())
            {
                float n1 = 1;   //Current Medium (assumed to be air)
                float n2 = its.shape->getMaterial().getIndexOfRefraction();     //Object Medium (IOR)

                // Check if Ray is inside Object and change Medium and Normal accordingly
                if (r.isInsideObj)
                {
                    n1 = n2;
                    n2 = 1;
                    n = -n;
                }
                
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
                        Ray reflectionRay = Ray(its.itsPoint, wr, r.depth + 1, r.lightDepth, false);
                        refrectance = computeColor(reflectionRay, objList, lsList, its);
                    }

                    else
                        refrectance = Vector3D(0, 0, 0);
                }

                //Transmission
                else
                {
                    if (r.depth <= maxRayDepth)
                    {
                        Vector3D wt = (Vector3D)(-sqrt(1 - pow(nt, 2) * sin2Alpha) + nt * dot(wo, n)) * n - (Vector3D)nt * wo;
                        Ray transmissionRay = Ray(its.itsPoint, wt, r.depth + 1, r.lightDepth, !r.isInsideObj);
                        refrectance = computeColor(transmissionRay, objList, lsList, its);
                    }
                    else
                        refrectance = Vector3D(1, 1, 1);
                }

            }

            color += intensity * refrectance;
        }

        return color;
    }

    else
        return bgColor;
}
