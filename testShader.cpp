#include "testShader.h"
void TestShader::initCG(CGcontext cgContext)
{

#ifdef __APPLE__
	const char* cgc_args[] = { "-DCG_FRAGMENT_PROFILE=fp40", "-DCG_VERTEX_PROFILE=vp40", NULL };
#else
	const char* cgc_args[] = { "-DCG_FRAGMENT_PROFILE=gp5fp", "-DCG_VERTEX_PROFILE=gp5vp", NULL };
#endif
	cgEffect = cgCreateEffectFromFile(cgContext, CGFX_SCEENPATH.c_str(), cgc_args);
	if(!cgEffect) {
		printf("CGFX error:\n %s\n", cgGetLastListing(cgContext));
		exit(-1);
	}

	cgTech = cgGetNamedTechnique(cgEffect, "blendPass");
	if(!cgTech) {
		printf("CGFX error:\n %s\n", cgGetLastListing(cgContext));
		exit(-1);
	}
	{
		getEffectParam(cgEffect, "WorldViewProj", &cgWorldViewProjParam);
		getEffectParam(cgEffect, "WorldViewInv", &cgWorldViewInvParam);
		getEffectParam(cgEffect, "WorldView", &cgWorldViewParam);
	
	}
	CHECK_ERRORS();
}
void TestShader::setModelViewMatrix(float * modelView)
{
	cgGLSetMatrixParameterfc(cgWorldViewParam,(float*)modelView);
}
void TestShader::setMvpMatrix(float * mvpFloat)
{
	
	cgGLSetMatrixParameterfc(cgWorldViewProjParam,(float*)mvpFloat);
	
}