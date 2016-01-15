#ifndef MYMETERIAL_H
#define  MYMETERIAL_H

#include "main.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <GL\glew.h>
#include <gl\gl.h>                    
#include <gl\glu.h>                    
//#include <gl\glaux.h>
#include <crtdbg.h>

#include <olectl.h>              
#include <math.h>  
class MyGeometry;
class MyMeterial
{
public :
	static optix::Context    ms_rtContext;
	MyMeterial();
	MyMeterial(char * jpgName);
	MyMeterial(float3 diffuse);
	optix::TextureSampler getTexture();
	optix::Material getMaterial(); 
	optix::Material getMaterial(MyGeometry * pGeomtry); 
	const int hasTx() const;
	const GLuint getTxId() const;
	const float3 getDiffuse() const;
private:
	char * m_fileName;
	float3 m_diffuseColor;
	int isTex;
	GLuint m_texId;
	std::string ptxpath( const std::string& base )
	{
		return std::string(sutilSamplesPtxDir()) + "/isgReflections_generated_" + base + ".ptx";
	}
};
#endif