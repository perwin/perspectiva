#include <assert.h>
#include <time.h>
#include "sampler.h"
#include "uniform_jitter_sampler.h"
#include "mersenne_twister.h"


UniformJitterSampler::UniformJitterSampler( const int sampleRate )
{
  nSamples1D = sampleRate;
  nSamples2D = nSamples1D*nSamples1D;
  offsetsAllocated = false;

  // seed RNG (replace time call with fixed integer to get repeatable
  // sequence for testing purposes)
  unsigned long t = (unsigned long)time((time_t *)NULL);
  init_genrand(t);
  
  xOffsets = (float *)calloc((size_t)nSamples2D, sizeof(double));
  yOffsets = (float *)calloc((size_t)nSamples2D, sizeof(double));
  offsetsAllocated = true;

  // precompute and store the x and y offsets relative to pixel center
  float oversamplePixFrac = 1.0/nSamples1D;
  pixFrac = oversamplePixFrac;
  int oversampleOffset = nSamples1D / 2 + 1 - nSamples1D;
  int n = 0;
  for (int j = 0; j < nSamples1D; ++j) {
    float yOff = oversamplePixFrac*(j + oversampleOffset);
    for (int i = 0; i < nSamples1D; ++i) {
      float xOff = oversamplePixFrac*(i + oversampleOffset);
      xOffsets[n] = xOff;
      yOffsets[n] = yOff;
      n += 1;
    }
  }
}


UniformJitterSampler::~UniformJitterSampler( )
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
/// In this jittered uniform oversampling version, each time this is called
/// a new randomly offset version of the (x_off, y_off) position is generated
void UniformJitterSampler::GetOffsetCoords( const int n, float *xOffset, float *yOffset ) const
{
  assert ((n >= 0) && (n < nSamples2D) && (offsetsAllocated));
  
  *xOffset = xOffsets[n] + (float)genrand_real1() - 0.5*pixFrac;
  *yOffset = yOffsets[n] + (float)genrand_real1() - 0.5*pixFrac;
}

