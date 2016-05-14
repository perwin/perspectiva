// Code for saving (and possibly also reading) images

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "image_io.h"


// Save result to a PPM image (keep these flags if you compile under Windows)
// NOTE (PE): We can conver the ppm image to e.g. png using "convert" (ImageMagick):
//    $ convert untitled.ppm untitled.png
// or using GraphicsMagick "gm convert":
//    $ gm convert untitled.ppm untitled.png
void SaveImage( Vec3f *image, unsigned width, unsigned height, std::string imageFilename )
{
  std::string outputFilename = imageFilename + ".ppm";
  unsigned char r, g, b;
  
  std::ofstream ofs(outputFilename.c_str(), std::ios::out | std::ios::binary); 
  ofs << "P6\n" << width << " " << height << "\n255\n"; 
  for (int i = 0; i < width*height; ++i) {
    // PE: min(1, image[i].x) ensures that rgb values are always <= 1.0,
    // max(0, ...) ensures they are always >= 0.0.
    // These are then multiplied by 255 to get into the 0,255 range
    // + 0.5 does slightly nicer quantizing (values from 99.5--100.49 --> 100
    // instead of 99.01--100.0 --> 100
    r = (unsigned char)(std::max(0.f, std::min(1.f, image[i].x)) * 255 + 0.5);
    g = (unsigned char)(std::max(0.f, std::min(1.f, image[i].y)) * 255 + 0.5);
    b = (unsigned char)(std::max(0.f, std::min(1.f, image[i].z)) * 255 + 0.5);
    ofs << r << g << b;
  } 
  ofs.close(); 
  
  printf("Saved image file \"%s\".\n", outputFilename.c_str());
}

