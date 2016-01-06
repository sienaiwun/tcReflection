#ifndef MAIN_H
#define MAIN_H

#include <cstdlib>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glew.h>
#  if defined(_WIN32)
#    include <GL/wglew.h>
#  endif
#  include <GL/glut.h>
#endif

#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>

#include <cstdio>
#include <limits>

#include <nvModel.h>
#include <nvTime.h>

#include <framebufferObject.h>
#include <nvGlutManipulators.h>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include <string.h>
#include <limits>

#include "sutil.h"
#include "commonStructs.h"
#include "PPMLoader.h"
//#include <ObjLoader.h>

#define SpeedUp    10
#define DrawPoint 0
#define JianGe   10
#define MerGe    0
#define DEBUG 

using namespace optix;

#define CHECK_ERRORS()         \
	do {                         \
	GLenum err = glGetError(); \
	if (err) {                                                       \
	printf( "GL Error %d at line %d of FILE %s\n", (int)err, __LINE__,__FILE__);       \
	exit(-1);                                                      \
	}                                                                \
	} while(0)
typedef struct posPara 
{
	float3 pos;
	float3 view;
} posPara;
#endif


