#ifndef _ENVIRONMENT_MAP_H_
#define _ENVIRONMENT_MAP_H_

#include <cstdlib> 
#include <math.h>
#include <vector>
#include <string>

#include "geometry.h"
#include "color.h"
#include "image_io.h"

using namespace std;

const vector<string> skyboxFaceNames = {"right", "left", "up", "down", "back", "front"};

const int  RIGHT_FACE = 0;
const int  LEFT_FACE = 1;
const int  TOP_FACE = 2;
const int  BOTTOM_FACE = 3;
const int  BACK_FACE = 4;
const int  FRONT_FACE = 5;
// 0 = right, 1 = left, 2 = top, 3 = bottom, 4 = back, 5 = front



void GetUVforCube( Ray theRay, int *imageIndex, float *u, float *v );



class Environment
{
  public:
    
    // Constructors with initializations:
    Environment( ) { baseColor = Color(0); hasMap = false; skyboxAllocated = false; }
    
    Environment( const Color c ) { baseColor = c; hasMap = false; skyboxAllocated = false; }
    
    Environment( const string baseFilename, const string fileExtension ) 
    {
      ReadSkyBox(baseFilename, fileExtension);
      hasMap = true; 
    }

    ~Environment( )
    { 
      if (skyboxAllocated) {
        for (int i = 0; i < 6; i++)
          delete skyboxImages[i];
        skyboxImages.clear();
      }
    }


	void SetEnvironmentColor( Color c) { baseColor = c; }
	
	Color GetEnvironmentColor( Ray theRay )
	{
	  if (hasMap)
	    return GetColorFromSkybox(theRay);  // dummy action; to be replaced with map sampling
	  else
	    return baseColor;
	}

    void AddSkyBox( const string baseFilename, const string fileExtension )
    {
      ReadSkyBox(baseFilename, fileExtension);
      hasMap = true;
    }
    
    void ReadSkyBox( const string baseFilename, const string extension )
    {
      // FIXME: insert checks for file existence!
      for (int i = 0; i < 6; i++) {
        string imageName = baseFilename + skyboxFaceNames[i] + extension;
        printf("Reading %s...\n", imageName.c_str());
        skyboxImages.push_back( ReadImage(imageName, imageWidth, imageHeight) );
      }
      skyboxAllocated = true;
      printf("Skybox image w,h = %d,%d\n", imageWidth, imageHeight);
    }

    Color GetColorFromSkybox( Ray theRay )
    {
      int imageIndex;
      float  u, v;
      int  x, y;
      Color  *image;
      
      GetUVforCube(theRay, &imageIndex, &u, &v);
      x = (int)(u*(imageWidth - 1));
      y = (int)(v*(imageHeight - 1));
      image = skyboxImages[imageIndex];
      return image[y*imageWidth + x];
    }


    // Data members:
	Color  baseColor;
	bool  hasMap;
	bool  skyboxAllocated;
	vector<Color *> skyboxImages;
	int  imageWidth, imageHeight;
};


#endif  // _ENVIRONMENT_MAP_H_
