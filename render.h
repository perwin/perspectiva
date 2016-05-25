// Code for raytracing and saving images

#ifndef _RENDER_H_
#define _RENDER_H_

#include <string>
#include <vector>

#include "vec3.h"
#include "scene.h"
#include "option_structs.h"



// Vec3f Trace( const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Object *> objects, 
//     		const int depth );

void RenderImage( Scene *theScene, Vec3f *image, const int width, const int height, 
					const traceOptions &options );


#endif   // _RENDER_H_
