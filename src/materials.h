// in-development header for material classes
// The eventual goal is for each object to have a pointer to an instance of a
// Material subclass, and to have references to Object->surfaceColor replaced by
// Object->GetSurfaceColor(), which in calls Material->GetSurfaceColor()

// current system in scene file:
//     - sphere:
//         position: [-2.0, -3.0, -25.0]
//         radius: 2.0
//         surface_color: [0.7, 0.7, 0.7]
//         reflectivity: 1.0
//         transparency: 0.0

// possible new version:
//     - sphere:
//         position: [-2.0, -3.0, -25.0]
//         radius: 2.0
//         material:
//             type: base
//             surface_color: [0.7, 0.7, 0.7]
//             reflectivity: 1.0
//             transparency: 0.0

#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#include <stdio.h>
#include <cstdlib>

#include "vec3.h"
#include "color.h"

// This is an abstract base class (bcs we have at least one member function declared
// as pure virtual)
class Material
{
  public:
    // default constructor and destructor
    Material( ) {};
    virtual ~Material( ) {};
  
    virtual Color GetSurfaceColor( ) {return surfaceColor;};

    // pure virtual function: *must* be overridden in derived classes
    virtual Color ComputeObjectColor( Vec3f rayDirection, Vec3f n_hit, Vec3f lightDirection ) = 0;
    
    virtual Color GetReflectionColor( ) {return Color(0);};   // not used yet!
    
    virtual Color GetRefractionColor( ) {return Color(0);};   // not used yet!

    virtual Color GetEmissionColor( ) {return Color(0);};
    
    // the following do not need to be overriden, unless reflectivity and/or transparency
    // are more complicated than one value for the entire object
    virtual float GetReflectivity( )
    {
      return reflectivity;
    };
    
    virtual float GetTransparency( )
    {
      return transparency;
    };
    
    virtual bool HasSpecular( )
    {
      return false;
    };
    
  
    Color surfaceColor;
    Color reflectionColor;
    Color refractionColor;
    Color emissionColor;
    float reflectivity;
    float transparency;

};


// The simplest kind of material: matte surface with single color, no transparency or reflectivity
class MatteMaterial : public Material {

  public:
    MatteMaterial( Color c )
    {
      surfaceColor = c;
    };
    
    ~MatteMaterial( ) {};

//         Color diffuseColor = (intersectedObject->GetSurfaceColor() * 
//     				  fmax(float(0), n_hit.dotProduct(-lightDirection)) 
//     				  * lightIntensity * visibility);

    Color ComputeObjectColor( Vec3f rayDirection, Vec3f n_hit, Vec3f lightDirection )
    {
      // standard Lambert diffuse reflection
      float  diffuseReflect = fmax(float(0), n_hit.dotProduct(-lightDirection));
      return diffuseReflect * surfaceColor;
    };

    Color GetEmissionColor( ) 
    {
      return emissionColor;
    };
    
    Color GetReflectionColor( ) { return Color(0); };
    Color GetRefractionColor( ) { return Color(0); };  
};


// UNFINISHED!
class PhongMaterial : public Material {

  public:
    PhongMaterial( Color surfColor, float K_d, float K_s, float n ) 
    {
      surfaceColor = surfColor;
      Kd = K_d;
      Ks = K_s;
      exponent = n;
    };
    
    ~PhongMaterial( ) {};
  
    // [ ] FIXME: change this to proper Phong diffuse + specular calculation
    Color ComputeObjectColor( Vec3f rayDirection, Vec3f n_hit, Vec3f lightDirection )
    {
      float  diffuseReflect = fmax(float(0), n_hit.dotProduct(-lightDirection));
      return diffuseReflect * surfaceColor;
    };

    Color GetEmissionColor( ) { return Color(0); };
    Color GetReflectionColor( ) { return Color(0); };
    Color GetRefractionColor( ) { return Color(0); };
  
    bool HasSpecular( )
    {
      return true;
    };

  
    float Kd = 0.8;  // Phone diffuse weight 
    float Ks = 0.2;  // Phong specular weight
    float exponent = 10;    // Phong specular exponent
};


// Like MatteMaterial, but also includes simple single-valued reflectivity and refraction,
// plus possibly emission
class SimpleMaterial : public Material {

  public:
    SimpleMaterial( Color surfColor, Color emissColor, Color reflectColor, Color refractColor,
    				float reflect, float transp )
    {
      surfaceColor = surfColor;
      reflectionColor = reflectColor;
      refractionColor = refractColor;
      emissionColor = emissColor;
      reflectivity = reflect;
      transparency = transp;
    };
    
    ~SimpleMaterial( ) {};
  
    Color ComputeObjectColor( Vec3f rayDirection, Vec3f n_hit, Vec3f lightDirection )
    {
      // standard Lambert diffuse reflection
      float  diffuseReflect = fmax(float(0), n_hit.dotProduct(-lightDirection));
      return diffuseReflect * surfaceColor;
    };
    
    Color GetReflectionColor( ) 
    {
      return reflectionColor;
    };
    
    Color GetRefractionColor( ) 
    {
      return refractionColor;
    };

    Color GetEmissionColor( ) 
    {
      return emissionColor;
    };

};

#endif  // _MATERIALS_H_
