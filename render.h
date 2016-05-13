// Code for raytracing and saving images

#include <string>
#include <vector>

#include "vec3.h"
#include "scene.h"
#include "trace_options.h"



Vec3f Trace( const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &spheres, 
    		const int &depth );
void SaveImage( Vec3f *image, unsigned width=640, unsigned height=480, 
				std::string imageFilename="untitled" );
void RenderAndSaveImage( Scene &theScene, unsigned width, unsigned height, 
				std::string filename, const traceOptions &options );
