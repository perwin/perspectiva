// Code for light classes; based on code from scratchapixel.com

#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <cstdlib> 
#include <math.h>
#include <stdio.h>
#include "vec3.h"
#include "color.h"
#include "mersenne_twister.h"
#include "definitions.h"


class Light
{
public:
  Light( ) {};

  virtual ~Light() {};

  // illuminate takes the current shaded point P and returns three things:
  //    1. The vector *from* the light *to* the point P
  //    2. The Color specifying the incoming light to point P
  //    3. The (scalar) distance between the light and P
  virtual void illuminate( const Vec3f &P, Vec3f &lightDir, Color &lightIntensity,
  						 float &distance ) const = 0;
  
  // this is the number of random samples on the light surface that can be provided;
  // for Point and Distant lights this is always 1
  virtual int NSamples( ) const
  {
    return 1;
  }
  
  int GetType( ) const
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
    lightPosition = pos;
    lightColor = color;
    luminosity = lum;
  }
  
  // P: is the shaded point
  void illuminate( const Vec3f &P, Vec3f &lightDir, Color &lightIntensity, float &distance ) const
  { 
    lightDir = P - lightPosition;   // vector from this light to P
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
  Vec3f lightPosition;   // location of light in world space
};



class DistantLight : public Light
{ 
public: 
  DistantLight( const Vec3f &lightDir, const Color &color, const float lum )
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



class SphericalLight : public Light
{ 
public:
  SphericalLight( const Vec3f &P, const float r, const Color &color, const float lum,
  					const int nsamps=1 )
  {
    lightType = LIGHT_SPHERE;
    lightPosition = P;
    radius = r;
    lightColor = color;
    luminosity = lum;
    nSamples = nsamps;
  }
  
  void illuminate( const Vec3f &P, Vec3f &lightDir, Color &lightIntensity, float &distance ) const
  {
    // get random point on surface of sphere (translate to world coordinates)
    Vec3f randomPoint = lightPosition + GetRandomSurfacePoint();
    // direction vector from this point on sphere's surface to P
    lightDir = P - randomPoint;
    float r2 = lightDir.norm();
    distance = sqrt(r2);
    // normalize the lightDir vector
    lightDir.x /= distance;
    lightDir.y /= distance;
    lightDir.z /= distance;
    // correct this by computing dot product of lightDir and normal?
    lightIntensity = lightColor * luminosity / (FOUR_PI * r2);
//     lightDir = P - lightPosition;   // direction vector from this light to P
//     float r2 = lightDir.norm();
//     distance = sqrt(r2);
//     // normalize the lightDir vector
//     lightDir.x /= distance;
//     lightDir.y /= distance;
//     lightDir.z /= distance;
//     // avoid division by 0
//     lightIntensity = lightColor * luminosity / (FOUR_PI * r2);
  }

  // get a random point on the sphere's surface
  Vec3f GetRandomSurfacePoint( ) const
  {
    float x, y, z, u, theta, sqrt_one_minus_u2;
    Vec3f newPoint;
    
    theta = 2*PI*genrand_real2();
    u = 2*genrand_real1() - 1.0;
    sqrt_one_minus_u2 = sqrt(1.0 - u*u);
    x = radius * sqrt_one_minus_u2 * cos(theta);
    y = radius * sqrt_one_minus_u2 * sin(theta);
    z = radius * u;
    newPoint = Vec3f(x,y,z);
    return newPoint;
  }
  
  int NSamples( ) const
  {
    return nSamples;
  }
  
  // additional data members
  Vec3f lightPosition;
  float radius;
  int nSamples;   // number of random samples on surface of sphere to generate
};


#endif  // _LIGHTS_H_
