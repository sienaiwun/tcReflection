#include "glslShaderLoader.h"
#include "Camera.h"
#include <string>
#include<nvMath.h>
#ifndef GLSLSHADER_H
#define GLSLSHADER_H
class MyGeometry;
class glslShader
{
public:
	virtual void bindParemeter() = 0;
	virtual void init() = 0;
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void setCamera(CCamera* pCamera){};
	virtual void setGeometry(MyGeometry * pGeometry){};
protected:
	std::string m_vertexFileName,m_geometryFileName,m_fragmentFileName;
	glslShaderLoader m_loader;
};
#endif