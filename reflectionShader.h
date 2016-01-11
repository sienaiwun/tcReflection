#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "Cg/Cg.h"
#include "Cg/cgGL.h"
#include "cgShader.h"
#ifndef REFLECTION_H
#define REFLECTION_H
void getEffectParam( CGeffect effect,
					const char* semantic,
					CGparameter* param );
class ReflectionShader:public CgShader
{
	CGeffect cgEffect;
	CGtechnique cgTech;
public:
	
	virtual inline CGtechnique getTechnique()
	{
		return cgTech;
	}
	ReflectionShader()
	{
		CGFX_SCEENPATH = std::string( sutilSamplesDir() ) + "/isgReflections/reflectionShader.cgfx";
		//CGFX_SCEENPATH = std::string("drawQuad.cgfx");
	}
	void initCG(CGcontext cgContext);
	void setWorkingTex(GLuint );
	void setMvpMatrix(float *);
	void setModelViewMatrix(float*);
private:
	CGparameter cgWorldViewProjParam;
	CGparameter cgWorldViewInvParam;
	CGparameter cgWorldViewParam;
	CGparameter cgBlendFactor;
	CGparameter cgCameraMVPParam;
	CGparameter cgCameraModelViewParam;
	CGparameter cgCameraModelViewInvParem;
	CGparameter cgGlossinessParam;
	CGparameter cgReflectionMapParam;
	CGparameter cgWorldPosMapParam;
	CGparameter cgNormalMapParam;
	CGparameter cgDoISGParam;
	CGparameter cgDiffuseTexParam;
	CGparameter cgInvSceneScaleParam;
	CGparameter cgLightPosParam;
	CGparameter cgDoReflectionsParam;
	CGparameter cgIsModelParam;
	CGparameter cgIsWallParam;
	CGparameter cgDiffuseColor;
	CGparameter cgIsTex;
	CGparameter cgShiness;
	CGparameter cgReflectVale;
	CGparameter cgCameraPosition;
	CGparameter cgCudaTransTexParam;
	CGparameter cgModelIdParam;
	CGparameter NewWorldPosTexParam;
	CGparameter LastEffectTexParam;
	CGparameter lastMVPParam;


};
#endif