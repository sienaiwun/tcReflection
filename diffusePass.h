#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "Cg/Cg.h"
#include "Cg/cgGL.h"
//#include "Material.h"
#include "cgShader.h"
#ifndef DIFFUSE_H
#define DIFFUSE_H

void getEffectParam( CGeffect effect,
					const char* semantic,
					CGparameter* param );
class DiffusePass:public CgShader
{
	
public:
	inline virtual CGtechnique  getTechnique()
	{
		return cgTech;
	}
	//virtual void setCgPara(Material m);
	void isRenderPoint();
	void notRenderPoint();
	DiffusePass()
	{
		CGFX_SCEENPATH = std::string("shader.cgfx");
	}
	void isPlane()
	{
		cgSetParameter1f(cgIsPlane,1);
	}
	void notPlane()
	{
		cgSetParameter1f(cgIsPlane,0);
	}
	void initCG(CGcontext cgContext);
	void setMatrix(float *,float *);
	void setLightPosition(nv::vec3f lightPos);
	CGparameter cgDiffuseParam;
	void setModelViewMatrix(nv::matrix4f,float);
	
private:

	CGeffect cgEffect;
	CGtechnique cgTech;
	CGparameter cgWorldViewProjParam;
	CGparameter cgWorldViewInvParam;
	CGparameter cgWorldViewParam;

	
	CGparameter cgLightPosParam;
	CGparameter cgDiffuseTexParam;
	CGparameter cgIsTex;
	CGparameter cgModelParam;
	CGparameter cgIsPlane;
	CGparameter cgRenderPoint;


};
#endif