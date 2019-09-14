// in-development header for material classes

#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#include "geometry.h"
#include "color.h"


class Material
{
  public:
    // default constructor and destructor
    Material( ) {};
    virtual ~Material( ) {};
  
    virtual Color GetSurfaceColor( ) = 0;
    
    virtual Color GetReflectionColor( ) = 0;
    
    virtual Color GetRefractionColor( ) = 0;

    virtual Color GetEmissionColor( ) = 0;

    virtual Color ComputeShapeColor( Vector rayDirection, Vector n_hit, Vector lightDirection ) = 0;

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

    virtual float GetIOR( )
    {
      return IOR;
    };

    virtual bool HasSpecular( )
    {
      return false;
    };
    
  
  protected:
    Color surfaceColor = Color(1);
    Color reflectionColor = Color(0);
    Color refractionColor = Color(0);
    Color emissionColor = Color(0);
    float reflectivity = 0.0;
    float transparency = 0.0;
    float IOR = 1.0;
};


// The simplest kind of material: matte surface with single color, no transparency or reflectivity
class MatteMaterial : public Material {

  public:
    MatteMaterial( Color c )
    {
      surfaceColor = c;
    };
    
    ~MatteMaterial( ) {};
  
    Color GetSurfaceColor( ) 
    {
      return surfaceColor;
    };
    
    Color GetReflectionColor( ) { return Color(0); };
    Color GetRefractionColor( ) { return Color(0); };
    Color GetEmissionColor( ) { return Color(0); };

    Color ComputeShapeColor( Vector rayDirection, Vector n_hit, Vector lightDirection )
    {
      return surfaceColor * fmax(float(0), Dot(n_hit, -lightDirection));
    }
};


// Like MatteMaterial, but also includes simple single-valued reflectivity and refraction,
// plus separate colors for reflection, refraction, and emission
class SimpleMaterial : public Material {

  public:
    SimpleMaterial( Color surfColor, Color reflectColor, Color refractColor,
    				Color emissColor, float reflect, float transp, float ior=1.0 )
    {
      surfaceColor = surfColor;
      reflectionColor = reflectColor;
      refractionColor = refractColor;
      emissionColor = emissColor;
      reflectivity = reflect;
      transparency = transp;
      IOR = ior;
    };
    
    ~SimpleMaterial( ) {};
  
    Color GetSurfaceColor( ) 
    {
      return surfaceColor;
    };
    
    Color GetReflectionColor( ) 
    {
      return reflectionColor;
    };
    
    Color GetRefractionColor( ) 
    {
      return refractionColor;
    };

    Color GetEmissionColor( ) {
      return emissionColor; 
    };

    Color ComputeShapeColor( Vector rayDirection, Vector n_hit, Vector lightDirection )
    {
      // classic Lambertian diffuse reflection
      return surfaceColor * fmax(float(0), Dot(n_hit, -lightDirection));
    }
};


#endif  // _MATERIALS_H_
