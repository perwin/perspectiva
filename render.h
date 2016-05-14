// Code for raytracing and saving images

#include <string>
#include <vector>

#include "vec3.h"
#include "scene.h"
#include "option_structs.h"



Vec3f Trace( const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &spheres, 
    		const int &depth );
void RenderAndSaveImage( Scene &theScene, unsigned width, unsigned height, 
				std::string filename, const traceOptions &options );
