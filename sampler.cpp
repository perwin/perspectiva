#include <assert.h>
#include <stdio.h>
#include <new>
#include "sampler.h"

Sampler::Sampler( const int sampleRate )
{
  nSamples1D = sampleRate;
  nSamples2D = nSamples1D*nSamples1D;
  offsetsAllocated = false;

  xOffsets = (float *)calloc((size_t)nSamples2D, sizeof(double));
  yOffsets = (float *)calloc((size_t)nSamples2D, sizeof(double));
  offsetsAllocated = true;

  // precompute and store the x and y offsets relative to pixel center
  float oversamplePixFrac = 1.0/nSamples1D;
  int oversampleOffset = nSamples1D / 2 + 1 - nSamples1D;
  int n = 0;
  for (int j = 0; j < nSamples1D; ++j) {
    float yOff = oversamplePixFrac*(j + oversampleOffset);
    for (int i = 0; i < nSamples1D; ++i) {
      float xOff = oversamplePixFrac*(i + oversampleOffset);
      //printf("j = %d, i = %d, n = %d: (xOff,yOff) = (%.4f,%.4f)\n", j, i, n, xOff, yOff);
      xOffsets[n] = xOff;
      yOffsets[n] = yOff;
      n += 1;
    }
  }
}


/// To be redefined in other classes or subclasses (e.g., for random sampling,
/// we want to compute new values for xOffsets and yOffsets whenever this is
/// called
void Sampler::Reset( )
{
  ;
}


Sampler::~Sampler( )
{
  if (offsetsAllocated) {
    free(xOffsets);
    free(yOffsets);
    offsetsAllocated = false;
  }
}


/// Returns offsets in x and y (--> xOffset, yOffset) for sample number n
/// Usage: assuming x and y are current pixel coords, nTotalSubsamples = nSamples1D**2:
///      for (int n = 0; n < nTotalSubsamples; ++n) {
///        GetOffsetCoords(n, &x_off, &y_off);
///        xx = x + x_off;
///        yy = y + y_off;
///        cameraRayDir = ComputeCameraRay(xx, yy, invWidth, invHeight, tanTheta, aspectRatio);
///        etc.
void Sampler::GetOffsetCoords( const int n, float *xOffset, float *yOffset ) const
{
  assert ((n >= 0) && (n < nSamples2D) && (offsetsAllocated));
  
  *xOffset = xOffsets[n];
  *yOffset = yOffsets[n];
}

