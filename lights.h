// Code for light classes; based on code from scratchapixel.com

#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <cstdlib> 
#include "vec3.h"
#include "color.h"
#include "definitions.h"

const float FOUR_PI = 12.566370614359172;
const float kInfinity = std::numeric_limits<float>::max(); 


class Light
{
public:
  Light( ) {};

  virtual ~Light() {};

  virtual void illuminate( const Vec3f &P, Vec3f &lightDir, Color &lightIntensity,
  						 float &distance ) const = 0;
  
  int GetType( )
  { 
    return lightType; 
  };

  // data members
  int  lightType;
  Color lightColor;
  float luminosity;
};



class PointLight : public Light 
{ 
public: 
  PointLight( const Color &color, const float lum, const Vec3f &pos )
  {
    lightType = LIGHT_POINT;
    position = pos;
    lightColor = color;
    luminosity = lum;
  }
  
  // P: is the shaded point
  void illuminate( const Vec3f &P, Vec3f &lightDir, Color &lightIntensity, float &distance ) const
  { 
    lightDir = (P - position);   // direction vector from this light to P
    float r2 = lightDir.norm();
    distance = sqrt(r2);
    // normalize the lightDir vector
    lightDir.x /= distance;
    lightDir.y /= distance;
    lightDir.z /= distance;
    // avoid division by 0
    lightIntensity = lightColor * luminosity / (FOUR_PI * r2);
  } 

  // additional data members
  Vec3f position;   // location of light in world space
};



class DistantLight : public Light
{ 
public: 
  DistantLight( const Color &color, const float &lum, const Vec3f &lightDir )
  {
    lightType = LIGHT_DISTANT;
    dir = lightDir;
    dir.normalize();
    lightColor = color;
    luminosity = lum;
  }
  
  void illuminate( const Vec3f &P, Vec3f &lightDir, Color &lightIntensity, float &distance ) const
  { 
    lightDir = dir;
    lightIntensity = lightColor * luminosity;
    distance = kInfinity;
  }

  // additional data members
  Vec3f dir;   // direction of light (world coordinate system)
};


#endif  // _LIGHTS_H_

