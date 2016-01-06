#ifndef MYGEOMETRY_H
#define  MYGEOMETRY_H

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
#include "cgShader.h"
#include <olectl.h>              
#include <math.h>
#include "MyMeterial .h"
#include "transform.h"
#include "glslShader.h"
typedef struct GeoPara
{
	char * name;
	int  matNumber;
	float shiness;
	float reflectValue;
	int actionNum;
} GeoPara;
extern MyMeterial mat[];
class MyGeometry
{

public:
	static nv::vec3f modelBBMin, modelBBMax;
	static optix::Context    ms_rtContext;
	static CGparameter *msgCgreflectVale,*mspCgisTex,*mspCgshiness,*mspCgIsModel,*mspCgIsWall,*mspCgDiffuseColor,*msgCgDiffuseTexParam;
	MyGeometry();
	MyGeometry(char* s);
	void initGeometry();
	void initGeometry(char*s);
	void initGeometry(GeoPara Gp);
	optix::TextureSampler initMaterial();
	void drawGeometry(glslShader & shader,int change = 0);
	void drawGeometry(CGtechnique& tech,int change = 0);
	static void drawTex(GLuint mapId);
	GLuint m_ModelVb,m_ModelIb,optixVb;
	optix::Geometry getOptixGeometry();
	optix::GeometryInstance getInstance();
	void setMaterial(MyMeterial* m);
	MyMeterial* getMaterial()const;
	nv::Model*  m_model;
	int actionNumber;
	static void drawQuad(glslShader & shader);
	static void drawQrad(CgShader& shader);
private:

	char * filename;
	int m_hasTX;
	float reflectValue,shiness;
	MyMeterial* mp_met;
	std::string ptxpath( const std::string& base )
	{
		return std::string(sutilSamplesPtxDir()) + "/isgReflections_generated_" + base + ".ptx";
	}
	nv::vec3f m_modelBBMin, m_modelBBMax;

};


#endif