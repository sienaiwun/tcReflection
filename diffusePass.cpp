#include "diffusePass.h"
void DiffusePass::initCG(CGcontext cgContext)
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

	cgTech = cgGetNamedTechnique(cgEffect, "GlossyReflections");
	if(!cgTech) {
		printf("CGFX error:\n %s\n", cgGetLastListing(cgContext));
		exit(-1);
	}
	getEffectParam(cgEffect,"DiffuseColor",&cgDiffuseParam);
	getEffectParam(cgEffect,"LightPos",&cgLightPosParam);
	getEffectParam(cgEffect, "WorldViewProj", &cgWorldViewProjParam);
	getEffectParam(cgEffect, "WorldViewInv", &cgWorldViewInvParam);
	getEffectParam(cgEffect, "WorldView", &cgWorldViewParam);
	

	getEffectParam(cgEffect,"DiffuseTex",&cgDiffuseTexParam);
	getEffectParam(cgEffect,"isTex",&cgIsTex);
	getEffectParam(cgEffect,"ModelView",&cgModelParam);

	getEffectParam(cgEffect,"isPlane",&cgIsPlane);
	getEffectParam(cgEffect,"renderPoint",&cgRenderPoint);
	CHECK_ERRORS();
}
void DiffusePass::isRenderPoint()
{
	cgGLSetParameter1f(cgRenderPoint,1);
}
void DiffusePass::notRenderPoint()
{
	cgGLSetParameter1f(cgRenderPoint,0);
}
void DiffusePass::setModelViewMatrix(nv::matrix4f modelMatrix,float isPlane)
{
	
	cgGLSetMatrixParameterfc(cgModelParam,(float*)modelMatrix.get_value());
	cgGLSetParameter1f(cgIsPlane,isPlane);

}
void DiffusePass::setMatrix(float * m,float *p)
{
	cgGLSetMatrixParameterfc(cgWorldViewParam,m);
	nv::matrix4f mvpMatrix = nv::matrix4f(p)*nv::matrix4f(m);
	cgGLSetMatrixParameterfc(cgWorldViewProjParam,(float*)(mvpMatrix.get_value()));
}
void DiffusePass:: setLightPosition(nv::vec3f lightPos)
{
	cgGLSetParameter3fv(cgLightPosParam,lightPos);
}
/*
  void DiffusePass::setCgPara(Material m)
{
	if(m.hasKd()){
		assert(m.hasKd());
		cgGLSetParameter3fv(cgDiffuseParam,m.getkd());
		cgGLSetParameter1f(cgIsTex,0);
	}
	else if (m.hasKdTex())
	{
		assert(m.kdTexId());
		cgGLSetTextureParameter(cgDiffuseTexParam,m.kdTexId());
		cgGLSetParameter1f(cgIsTex,1);
	}
}*/