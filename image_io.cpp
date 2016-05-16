// Code for saving (and possibly also reading) images

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <OpenEXR/ImfConvert.h>
#include <OpenEXR/ImfRgbaFile.h>

#include "image_io.h"
#include "definitions.h"



void SaveImage( Vec3f *image, int width, int height, const std::string outputImageName, 
				int outputImageFormat )
{
  switch (outputImageFormat) {
    case IMAGE_PPM:
      SaveImagePPM(image, width, height, outputImageName);
      break;
    case IMAGE_PNG:
      SaveImagePNG(image, width, height, outputImageName);
      break;
    case IMAGE_EXR:
      SaveImageOpenEXR(image, width, height, outputImageName);
      break;
    default:
      printf("WARNING: Unrecognized image format in output filename (\"%s\")!\n", 
      		outputImageName.c_str());
  }
}


// Save result to a PPM image (keep these flags if you compile under Windows)
// NOTE (PE): We can conver the ppm image to e.g. png using "convert" (ImageMagick):
//    $ convert untitled.ppm untitled.png
// or using GraphicsMagick "gm convert":
//    $ gm convert untitled.ppm untitled.png
void SaveImagePPM( Vec3f *image, int width, int height, std::string imageFilename )
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


void SaveImagePNG( Vec3f *image, int width, int height, std::string imageFilename )
{
  std::string outputFilename = imageFilename;  // assumed to already end in ".png"
  unsigned char *outputImage;
  int imageSize = width*height;

  outputImage = (unsigned char *)malloc(3*imageSize*sizeof(unsigned char));
  
  for (int i = 0; i < width*height; ++i) {
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



// OpenEXR stuff:
// struct Rgba {
//     half r;  // red
//     half g;  // green
//     half b;  // blue
//     half a;  // alpha (opacity)
// };
// 
// void
// writeRgba1 (const char fileName[], const Rgba *pixels, int width, int height)
// {
//   RgbaOutputFile file (fileName, width, height, WRITE_RGBA); 
//   file.setFrameBuffer (pixels, 1, width);
//   file.writePixels (height);
// }
// 
// 
// #include <OpenEXR/ImfConvert.h>
// img[j][i].r=floatToHalf(*(img_ptr++));
// img[j][i].g=floatToHalf(*(img_ptr++));
// img[j][i].b=floatToHalf(*(img_ptr++));

/// Simplistic function for saving rendered image in OpenEXR format (assumes
/// no rendered alpha channel; output image has alpha = 1 everywhere).
void SaveImageOpenEXR( Vec3f *image, int width, int height, std::string imageFilename )
{
  std::string outputFilename = imageFilename;  // assumed to already end in ".exr"
  int nPixels = width*height;
  Imf::Rgba *pixelsOpenEXR = (Imf::Rgba *)malloc(nPixels * sizeof(Imf::Rgba));
  
  for (int i = 0; i < nPixels; ++i) {
    pixelsOpenEXR[i].r = Imf::floatToHalf(image[i].x);
    pixelsOpenEXR[i].g = Imf::floatToHalf(image[i].y);
    pixelsOpenEXR[i].b = Imf::floatToHalf(image[i].z);
    pixelsOpenEXR[i].a = Imf::floatToHalf(1.0);
  }

  Imf::RgbaOutputFile file(imageFilename.c_str(), width, height, Imf::WRITE_RGBA); 
  file.setFrameBuffer(pixelsOpenEXR, 1, width);
  file.writePixels(height);
  printf("Saved OpenEXR image file \"%s\"\n", imageFilename.c_str());
  
  free(pixelsOpenEXR);
}
