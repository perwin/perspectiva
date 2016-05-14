// Code for saving (and possibly also reading) images

#include <string>
#include "vec3.h"

void SaveImage( Vec3f *image, unsigned width=640, unsigned height=480, 
				std::string imageFilename="untitled" );
