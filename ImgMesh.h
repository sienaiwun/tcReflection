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

#ifndef IMGMESH_H
#define IMGMESH_H
class glslShader;
class ImgMesh
{
public:
	ImgMesh(int w, int h);
	void init();
	void drawImgMesh();
	void drawImgMesh(glslShader& shader);
private:
	GLuint m_vboId;
	int m_w,  m_h;
};

#endif 
