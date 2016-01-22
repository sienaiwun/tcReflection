#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>										// Used for our STL string objects
#include <fstream>										// Used for our ifstream object to load text files
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <GL/wglew.h>
#ifndef CH_SHADER_H
#define CH_SHADER_H

void getEffectParam( CGeffect effect,
					const char* semantic,
					CGparameter* param );

 class CgShader
{
public:
	inline virtual CGtechnique getTechnique() = 0;
	virtual void setVertexNumber(int num)
	{

	}
	inline std::string getEffectName()
	{
		return CGFX_SCEENPATH;
	}
	std::string CGFX_SCEENPATH;
};
#endif