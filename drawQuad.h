#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "Cg/Cg.h"
#include "Cg/cgGL.h"
#include "cgShader.h"
#ifndef drawQuad_h
#define drawQuad_h
void getEffectParam( CGeffect effect,
					const char* semantic,
					CGparameter* param );
class DrawQuadShader:public CgShader
{
	CGeffect cgEffect;
	CGtechnique cgTech;
public:
	
	virtual inline CGtechnique getTechnique()
	{
		return cgTech;
	}
	DrawQuadShader()
	{
		CGFX_SCEENPATH = std::string( sutilSamplesDir() ) + "/isgReflections/drawQuad.cgfx";
		//CGFX_SCEENPATH = std::string("drawQuad.cgfx");
	}
	void initCG(CGcontext cgContext);
	void setWorkingTex(GLuint );
private:


	CGparameter cgWorkingTex;
};
#endif