# Raytracer
Raytracer created as part of the Advanced Computer Graphics class at the UPF

Based on the framework provided by the ACG teacher Arnau Colom.

This project consisted of implementing three materials (phong, mirror, transmissive) and a shader that supports global illumination.

## Files created by me:
Materials:
mirror.cpp / mirror.h
phong.cpp /phong.h
transmissive.cpp / transmissive.h

Shader:
directShader.cpp / directShader.h (only direct illumination)
intersectionshader.cpp / intersectionshader.h (basic shader that displays intersections, no light sources or shading)
normalShader.cpp / normalShader.h (displays normal values as colors)
depthShader.cpp / depthShader.cpp (like the intersectionshader but considers the object’s distance to the camera)
globalShader.cpp / globalShader.h (renders both direct and indirect illumination)

## Files edited by me:
ray.cpp / ray.h (isInsideObj bool added)
main.cpp (created new Materials, created new Objects, created new Lights, raytrace function (ray generation), created new Shader, call raytrace with the correct arguments in the main function)


## How to use
The current shader is the global shader. To change to a different shader go to the bottom of the main.cpp file. Uncomment the “raytrace(...)” line below the chosen shader and comment the “raytrace(...)” line below //Global Shader.

To add Objects, Materials, and Shaders edit the respective part of the main.cpp file. 

To change the amount of light bounces and number of samples, change the “shaderG” in the main.cpp file or create a new global shader. The first argument passed to the GlobalShader controls the samples, the third argument controls the light bounces.

# Examples
Global Shader (Direct and Indirect Illumination (3 light bounces and 3 light samples) with phong, mirror, and transmissive material):
![global](https://user-images.githubusercontent.com/50264516/212912890-d412b125-d69d-4037-93d5-641218dd3567.jpg)

Direct Shader (direct illumination with phong material):
![direct1](https://user-images.githubusercontent.com/50264516/212912884-46a684ce-5efb-4b39-81d4-4531747219a0.jpg)
![direct2](https://user-images.githubusercontent.com/50264516/212912886-529f16b3-daa5-409c-823e-e7c0aaf8510b.jpg)

Normal Shader (visualizes surface normals):
![normal](https://user-images.githubusercontent.com/50264516/212912894-b614e2fe-a820-4818-83ec-3a6c51ba9bf8.jpg)

Depth Shader (flat shader, considers distance to camera):
![depth](https://user-images.githubusercontent.com/50264516/212912878-62be8f32-6ca6-4e21-aee3-6e29c724de34.jpg)

Intersection Shader (flat shading):
![flat](https://user-images.githubusercontent.com/50264516/212912888-73b36992-190e-4ac6-b6d2-0f89de437fa7.jpg)
