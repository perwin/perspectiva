// Code for raytracing and saving images

#ifndef _RENDER_H_
#define _RENDER_H_

#include <string>
#include <vector>

#include "scene.h"
#include "color.h"
#include "option_structs.h"


void RenderImage( Scene *theScene, Color *image, const int width, const int height, 
					const traceOptions &options );


#endif   // _RENDER_H_
