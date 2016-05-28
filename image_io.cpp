// Code for saving (and possibly also reading) images

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <OpenEXR/ImfConvert.h>
#include <OpenEXR/ImfRgbaFile.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.h"
#include "image_io.h"
#include "definitions.h"


float clamp( float x )
{
  return x<0 ? 0 : x>1 ? 1 : x;
}

// convert linear floating-point light values to byte values, including
// pre-correction for computer monitor gamma = 2.2
unsigned char GammaCorrectToByte( float lightValue )
{
  return (unsigned char)int( pow(clamp(lightValue), 1.0/2.2)*255 + 0.5 );
}



void SaveImage( Color *image, int width, int height, const std::string outputImageName, 
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
// NOTE (PE): We can convert the ppm image to e.g. png using "convert" (ImageMagick):
//    $ convert untitled.ppm untitled.png
// or using GraphicsMagick "gm convert":
//    $ gm convert untitled.ppm untitled.png
// [Or, now, just use SaveImagePNG to save the raw image directly to PNG format.]
void SaveImagePPM( Color *image, int width, int height, std::string imageFilename )
{
  std::string outputFilename = imageFilename + ".ppm";
  unsigned char r, g, b;
  
  std::ofstream ofs(outputFilename.c_str(), std::ios::out | std::ios::binary); 
  ofs << "P6\n" << width << " " << height << "\n255\n"; 
  for (int i = 0; i < width*height; ++i) {
    // PE: min(1, image[i].r) ensures that rgb values are always <= 1.0,
    // max(0, ...) ensures they are always >= 0.0.
    // These are then multiplied by 255 to get into the 0,255 range
    // + 0.5 does slightly nicer quantizing (values from 99.5--100.49 --> 100
    // instead of 99.01--100.0 --> 100
//     r = (unsigned char)(std::max(0.f, std::min(1.f, image[i].r)) * 255 + 0.5);
//     g = (unsigned char)(std::max(0.f, std::min(1.f, image[i].g)) * 255 + 0.5);
//     b = (unsigned char)(std::max(0.f, std::min(1.f, image[i].b)) * 255 + 0.5);
    r = GammaCorrectToByte(image[i].r);
    g = GammaCorrectToByte(image[i].g);
    b = GammaCorrectToByte(image[i].b);
    ofs << r << g << b;
  } 
  ofs.close(); 
  
  printf("Saved image file \"%s\".\n", outputFilename.c_str());
}


void SaveImagePNG( Color *image, int width, int height, std::string imageFilename )
{
  std::string outputFilename = imageFilename;  // assumed to already end in ".png"
  unsigned char *outputImage;
  int imageSize = width*height;

  outputImage = (unsigned char *)malloc(3*imageSize*sizeof(unsigned char));
  
  for (int i = 0; i < width*height; ++i) {
//     outputImage[3*i] = (unsigned char)(std::max(0.f, std::min(1.f, image[i].r)) * 255 + 0.5);
//     outputImage[3*i + 1] = (unsigned char)(std::max(0.f, std::min(1.f, image[i].g)) * 255 + 0.5);
//     outputImage[3*i + 2] = (unsigned char)(std::max(0.f, std::min(1.f, image[i].b)) * 255 + 0.5);    
    outputImage[3*i] = GammaCorrectToByte(image[i].r);
    outputImage[3*i + 1] = GammaCorrectToByte(image[i].g);
    outputImage[3*i + 2] = GammaCorrectToByte(image[i].b);    
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
void SaveImageOpenEXR( Color *image, int width, int height, std::string imageFilename )
{
  std::string outputFilename = imageFilename;  // assumed to already end in ".exr"
  int nPixels = width*height;
  Imf::Rgba *pixelsOpenEXR = (Imf::Rgba *)malloc(nPixels * sizeof(Imf::Rgba));
  
  for (int i = 0; i < nPixels; ++i) {
    pixelsOpenEXR[i].r = Imf::floatToHalf(image[i].r);
    pixelsOpenEXR[i].g = Imf::floatToHalf(image[i].g);
    pixelsOpenEXR[i].b = Imf::floatToHalf(image[i].b);
    pixelsOpenEXR[i].a = Imf::floatToHalf(1.0);
  }

  Imf::RgbaOutputFile file(imageFilename.c_str(), width, height, Imf::WRITE_RGBA); 
  file.setFrameBuffer(pixelsOpenEXR, 1, width);
  file.writePixels(height);
  printf("Saved OpenEXR image file \"%s\"\n", imageFilename.c_str());
  
  free(pixelsOpenEXR);
}
