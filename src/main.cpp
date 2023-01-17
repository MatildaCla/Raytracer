#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"

#include "shapes/sphere.h"
#include "shapes/infiniteplan.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/globalShader.h"
#include "shaders/directShader.h"
#include "shaders/depthshader.h"
#include "shaders/normalShader.h"
#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"

#include "materials/phong.h"
#include "materials/mirror.h"
#include "materials/transmissive.h"




void buildSceneCornellBox(Camera*& cam, Film*& film,
    std::vector<Shape*>*& objectsList, std::vector<PointLightSource>*& lightSourceList)
{
    /* **************************** */
    /* Declare and place the camera */
    /* **************************** */
    Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

    /* ********* */
    /* Materials */
    /* ********* */
    Material* redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
    Material* greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
    Material* greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
    Material* blueDiffuse = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0, 0, 0), 100);
    Material* transmissive = new Transmissive(1.1);
    Material* mirror = new Mirror();
    Material* red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);

    /* ******* */
    /* Objects */
    /* ******* */
    objectsList = new std::vector<Shape*>;
    double offset = 3.0;
    Matrix4x4 idTransform;
    // Construct the Cornell Box
    Shape* leftPlan = new InfinitePlan(Vector3D(-offset, 0, 0), Vector3D(1, 0, 0), redDiffuse);
    Shape* rightPlan = new InfinitePlan(Vector3D(offset, 0, 0), Vector3D(-1, 0, 0), greenDiffuse);
    Shape* topPlan = new InfinitePlan(Vector3D(0, offset, 0), Vector3D(0, -1, 0), greyDiffuse);
    Shape* bottomPlan = new InfinitePlan(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
    Shape* backPlan = new InfinitePlan(Vector3D(0, 0, 3 * offset), Vector3D(0, 0, -1), blueDiffuse);
    objectsList->push_back(leftPlan);
    objectsList->push_back(rightPlan);
    objectsList->push_back(topPlan);
    objectsList->push_back(bottomPlan);
    objectsList->push_back(backPlan);

    // Place the Spheres inside the Cornell Box
    Matrix4x4 sphereTransform1;
    double radius = 1;
    sphereTransform1 = Matrix4x4::translate(Vector3D(-offset + radius, -offset + radius, 3.5));
    Shape* s1 = new Sphere(1.5, sphereTransform1, mirror);
    Matrix4x4 sphereTransform2;
    sphereTransform2 = Matrix4x4::translate(Vector3D(1.0, 0.0, 2));
    Shape* s2 = new Sphere(1, sphereTransform2, transmissive);
    Matrix4x4 sphereTransform3;
    radius = 1;
    sphereTransform3 = Matrix4x4::translate(Vector3D(0.3, -offset + radius, 5));
    Shape* s3 = new Sphere(radius, sphereTransform3, red_100);
    objectsList->push_back(s1);
    objectsList->push_back(s2);
    objectsList->push_back(s3);

    /* ****** */
    /* Lights */
    /* ****** */
    lightSourceList = new std::vector<PointLightSource>;
    Vector3D lightPosition1 = Vector3D(0, offset - 1, 2 * offset);
    Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
    Vector3D lightPosition3 = Vector3D(0, offset - 1, offset);
    Vector3D intensity = Vector3D(5, 5, 5); // Radiant intensity (watts/sr)
    PointLightSource pointLS1(lightPosition1, intensity);
    PointLightSource pointLS2(lightPosition2, intensity);
    PointLightSource pointLS3(lightPosition3, intensity);
    lightSourceList->push_back(pointLS1);
    lightSourceList->push_back(pointLS2);
    lightSourceList->push_back(pointLS3);
}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList, Intersection its)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for(size_t lin=0; lin<resY; lin++)
    {
        // Show progression
        if ( lin%(resY/sizeBar) == 0)
            std::cout << ".";

        // Inner loop invariant: we have rendered col columns
        for(size_t col=0; col<resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + 0.5) / resX;
            double y = (double)(lin + 0.5) / resY;

            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);
            cameraRay.isInsideObj = false;
            cameraRay.lightDepth = 0;

            // Compute ray color according to the used shader
            Vector3D pixelColor = shader->computeColor( cameraRay, *objectsList, *lightSourceList, its);


            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }
}


//------------TASK 1---------------------//
void PaintImage(Film* film)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main Image Loop
    for (size_t lin = 0; lin < resY; lin++)
    {
        // Show progression
        if (lin % (resY / sizeBar) == 0)
            std::cout << ".";

        for (size_t col = 0; col < resX; col++)
        {
                       
            //Vector3D random_color = Vector3D((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
            Vector3D aspectCol = Vector3D((double)col / resX, (double)lin / resY, 0);
            film->setPixelValue(col,lin, aspectCol);
           
        }
    }
}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RT-ACG - Ray Tracer for \"Advanced Computer Graphics\"" << separator << std::endl;

    // Create an empty film
    Film *film;
    film = new Film(720, 512);


    // Declare the shader
    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1,0,0);
    Intersection its;
    
    //Shader *shader = new IntersectionShader (intersectionColor, bgColor);
    Shader *shaderI = new depthShader (intersectionColor, bgColor, 7.0f);
    Shader* shaderN = new NormalShader(intersectionColor, bgColor);
    Shader* shaderD = new DirectShader(3);
    Shader* shaderG = new GlobalShader(3, 3, 1);
  

    // Build the scene---------------------------------------------------------
    // 
    // Declare pointers to all the variables which describe the scene
    Camera* cam;
    std::vector<Shape*>* objectsList;
    std::vector<PointLightSource>* lightSourceList;
    //Create Scene Geometry and Illumiantion
    buildSceneCornellBox(cam, film, objectsList, lightSourceList);

    //---------------------------------------------------------------------------

    //Paint Image ONLY TASK 1
    //PaintImage(film);

    // Launch some rays! TASK 2,3,...
    // 
    //Depth Shader
    //raytrace(cam, shaderI, film, objectsList, lightSourceList, its);

    //Normal Shader
    //raytrace(cam, shaderN, film, objectsList, lightSourceList, its);

    //Direct Shader
    //raytrace(cam, shaderD, film, objectsList, lightSourceList, its);
    
    //Global Shader
    raytrace(cam, shaderG, film, objectsList, lightSourceList, its);

    // Save the final result to file
    std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
    film->save();

    std::cout << "\n\n" << std::endl;
    return 0;
}
