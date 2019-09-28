// in-development header for material classes

#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#include "geometry.h"
#include "color.h"
#include "render_utils.h"

const float DEFAULT_DIFFUSE_IOR = 1.5;


class Material
{
  public:
    // default constructor and destructor
    Material( ) {};
    virtual ~Material( ) {};
  
    virtual Color GetReflectionColor( ) = 0;
    
    virtual Color GetRefractionColor( ) = 0;

    virtual Color GetEmissionColor( ) = 0;

    virtual Color ComputeDiffuseColor( Vector rayDirection, Vector n_hit, Vector lightDirection ) = 0;

    virtual bool IsReflective( ) = 0;

    virtual bool IsTransparent( ) = 0;

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
    
  
  protected:
    Color diffuseColor = Color(1);
    Color reflectionColor = Color(0);
    Color refractionColor = Color(0);
    Color emissionColor = Color(0);
    float reflectivity = 0.0;
    float transparency = 0.0;
    float IOR = 1.0;
};


// Basic simple material, including simple single-valued reflectivity and refraction,
// plus separate colors for reflection, refraction, and emission
class SimpleMaterial : public Material {

  public:
    SimpleMaterial( Color surfColor, Color reflectColor, Color refractColor,
    				Color emissColor, float reflect, float transp, float ior=1.1 )
    {
      diffuseColor = surfColor;
      reflectionColor = reflectColor;
      refractionColor = refractColor;
      emissionColor = emissColor;
      reflectivity = reflect;
      transparency = transp;
      IOR = ior;
    };
    
    ~SimpleMaterial( ) {};
  
    Color GetReflectionColor( ) {
      return reflectionColor;
    };
    
    Color GetRefractionColor( ) {
      return refractionColor;
    };

    Color GetEmissionColor( ) {
      return emissionColor; 
    };

    bool IsReflective( ) { return (reflectivity > 0.0); };

    bool IsTransparent( ) { return (transparency > 0.0); };
    
    Color ComputeDiffuseColor( Vector rayDirection, Vector n_hit, Vector lightDirection )
    {
      // classic Lambertian diffuse reflection
      return diffuseColor * fmax(float(0), Dot(n_hit, -lightDirection));
    }
};


// In the diffuse case, we have a (nominally) dielectric material where *none* of
// the incident light is specularly reflected; instead, a portion of the light
// that is refracted into the material re-emerges after (very local) subsurface
// scattering. We approximate this with the Lambertian model, where *all* light
// is scattered uniformly, with the diffuseColor attribute controlling how much
// of the incoming light (per RGB channel) is scattered.
class Diffuse : public Material {

  public:
    Diffuse( Color diffuseC )
    {
      diffuseColor = diffuseC;
      reflectionColor = Color(0);
      refractionColor = Color(0);
      emissionColor = Color(0);
      reflectivity = 0.0;
      transparency = 0.0;
      IOR = DEFAULT_DIFFUSE_IOR;
    };

    ~Diffuse( ) {};


    Color GetReflectionColor( ) {
      return reflectionColor;
    };
    
    Color GetRefractionColor( ) {
      return refractionColor;
    };

    Color GetEmissionColor( ) {
      return emissionColor;
    };

    bool IsReflective( ) { return false; };

    bool IsTransparent( ) { return false; };
    
    Color ComputeDiffuseColor( Vector rayDirection, Vector n_hit, Vector lightDirection ) {
      // classic Lambertian diffuse reflection
      return diffuseColor * fmax(float(0), Dot(n_hit, -lightDirection));
    };
};


// In the metal/conductor case, we have a material which obeys the Fresnel equations,
// but where all of the refracted light is immediately absorbed.
// The relevant characteristic is the reflectionColor.
class Metal : public Material {

  public:
    Metal( Color reflectC )
    {
      diffuseColor = Color(0);
      reflectionColor = reflectC;
      refractionColor = Color(0);
      emissionColor = Color(0);
      reflectivity = 1.0;
      transparency = 0.0;
      IOR = DEFAULT_DIFFUSE_IOR;
    };

    ~Metal( ) {};


    Color GetReflectionColor( ) {
      return reflectionColor;
    };
        
    Color GetRefractionColor( ) {
      return refractionColor;
    };

    Color GetEmissionColor( ) {
      return emissionColor;
    };

    bool IsReflective( ) { return true; };

    bool IsTransparent( ) { return false; };
    
    Color ComputeDiffuseColor( Vector rayDirection, Vector n_hit, Vector lightDirection ) {
      return diffuseColor;
    };

};


class Dielectric : public Material {

  public:
    Dielectric( Color reflectC, Color refractC, float reflect, float transp,	float ior )
    {
      diffuseColor = Color(0);
      reflectionColor = reflectC;
      refractionColor = refractC;
      emissionColor = Color(0);
      reflectivity = reflect;
      transparency = transp;
      IOR = DEFAULT_DIFFUSE_IOR;
    };

    ~Dielectric( ) {};


    Color GetReflectionColor( ) {
      return reflectionColor;
    };
        
    Color GetRefractionColor( ) {
      return refractionColor;
    };

    Color GetEmissionColor( ) {
      return emissionColor;
    };

    bool IsReflective( ) { return (reflectivity > 0.0); };

    bool IsTransparent( ) { return (transparency > 0.0); };
    
    Color ComputeDiffuseColor( Vector rayDirection, Vector n_hit, Vector lightDirection ) {
      return diffuseColor;
    };

};



#endif  // _MATERIALS_H_
