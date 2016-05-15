// Code for saving (and possibly also reading) images

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "image_io.h"


// Save result to a PPM image (keep these flags if you compile under Windows)
// NOTE (PE): We can conver the ppm image to e.g. png using "convert" (ImageMagick):
//    $ convert untitled.ppm untitled.png
// or using GraphicsMagick "gm convert":
//    $ gm convert untitled.ppm untitled.png
void SaveImagePPM( Vec3f *image, unsigned width, unsigned height, std::string imageFilename )
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


void SaveImagePNG( Vec3f *image, unsigned width, unsigned height, std::string imageFilename )
{
  std::string outputFilename = imageFilename;  // assumed to already end in ".png"
//  unsigned char r, g, b;
  unsigned char *outputImage;
  int imageSize = width*height;

  outputImage = (unsigned char *)malloc(3*imageSize*sizeof(unsigned char));
  
  for (int i = 0; i < width*height; ++i) {
    // PE: min(1, image[i].x) ensures that rgb values are always <= 1.0,
    // max(0, ...) ensures they are always >= 0.0.
    // These are then multiplied by 255 to get into the 0,255 range
    // + 0.5 does slightly nicer quantizing (values from 99.5--100.49 --> 100
    // instead of 99.01--100.0 --> 100
//     r = (unsigned char)(std::max(0.f, std::min(1.f, image[i].x)) * 255 + 0.5);
//     g = (unsigned char)(std::max(0.f, std::min(1.f, image[i].y)) * 255 + 0.5);
//     b = (unsigned char)(std::max(0.f, std::min(1.f, image[i].z)) * 255 + 0.5);
    // r, g, b
    outputImage[3*i] = (unsigned char)(std::max(0.f, std::min(1.f, image[i].x)) * 255 + 0.5);
    outputImage[3*i + 1] = (unsigned char)(std::max(0.f, std::min(1.f, image[i].y)) * 255 + 0.5);
    outputImage[3*i + 2] = (unsigned char)(std::max(0.f, std::min(1.f, image[i].z)) * 255 + 0.5);    
  } 

  int result = stbi_write_png(outputFilename.c_str(), width, height, 3, outputImage, 3*width);
  if (result > 0)
    printf("Saved PNG image file \"%s\"\n", imageFilename.c_str());
  else
    printf("ERROR: failed attempt to save PNG image file \"%s\"\n", imageFilename.c_str());

  free(outputImage);
}
