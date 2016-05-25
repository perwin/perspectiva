// Code for saving (and possibly also reading) images

#ifndef _IMAGE_IO_H_
#define _IMAGE_IO_H_

#include <string>
#include "vec3.h"
#include "color.h"


void SaveImage( Color *image, int width, int height, const std::string outputImageName, 
				int outputImageFormat );

void SaveImagePPM( Color *image, int width=640, int height=480, 
					std::string imageFilename="untitled" );

void SaveImagePNG( Color *image, int width=640, int height=480, 
					std::string imageFilename="untitled" );

void SaveImageOpenEXR( Color *image, int width=640, int height=480, 
						std::string imageFilename="untitled" );

#endif   // _IMAGE_IO_H_
