// Code for saving (and possibly also reading) images

#ifndef _IMAGE_IO_H_
#define _IMAGE_IO_H_

#include <string>
#include "vec3.h"


void SaveImage( Vec3f *image, int width, int height, const std::string outputImageName, 
				int outputImageFormat );

void SaveImagePPM( Vec3f *image, int width=640, int height=480, 
					std::string imageFilename="untitled" );

void SaveImagePNG( Vec3f *image, int width=640, int height=480, 
					std::string imageFilename="untitled" );

void SaveImageOpenEXR( Vec3f *image, int width=640, int height=480, 
						std::string imageFilename="untitled" );

#endif   // _IMAGE_IO_H_
