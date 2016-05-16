// Code for raytracing and saving images

#include <string>
#include <vector>

#include "vec3.h"
#include "scene.h"
#include "option_structs.h"



Vec3f Trace( const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &spheres, 
    		const int &depth );
void RenderImage( Scene &theScene, Vec3f *image, int width, int height, 
					const traceOptions &options );
