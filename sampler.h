// Header file for base class dealing with generating pixel subsamples, in form
// of x and y offsets relative to pixel center.

#ifndef _SAMPLER_H_
#define _SAMPLER_H_


// The idea is to have an object which will precompute the coordinates of each
// subsample (relative to the pixel center), and pass those coordinates out one
// by one.

class Sampler 
{ 
public:

  // default constructor and destructor
  Sampler( const int sampleRate=1 );
  virtual ~Sampler( );

  /// Recompute internal offset vectors (if implemented)
  virtual void Update( );
  
  /// Computes offset in x and y xOffset, yOffset for sample number n
  virtual void GetOffsetCoords( const int n, float *xOffset, float *yOffset ) const;

//protected:
  int nSamples1D, nSamples2D;
  float *xOffsets, *yOffsets;
  bool offsetsAllocated;
}; 


#endif  // _SAMPLER_H_
