#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "Cg/Cg.h"
#include "Cg/cgGL.h"
#include "cgShader.h"
#ifndef TESTSHADER_H
#define TESTSHADER_H
void getEffectParam( CGeffect effect,
					const char* semantic,
					CGparameter* param );
class TestShader:public CgShader
{
	CGeffect cgEffect;
	CGtechnique cgTech;
public:
	
	virtual inline CGtechnique getTechnique()
	{
		return cgTech;
	}
	TestShader()
	{
		CGFX_SCEENPATH = std::string( sutilSamplesDir() ) + "/isgReflections/testShader.cgfx";
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
};
#endif