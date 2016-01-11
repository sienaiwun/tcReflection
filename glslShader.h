#include "glslShaderLoader.h"
#include <string>
#include<nvMath.h>
#ifndef GLSLSHADER_H
#define GLSLSHADER_H
class MyGeometry;
class CCamera;
class scene;
class glslShader
{
public:
	virtual void bindParemeter() = 0;
	virtual void init() = 0;
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void setCamera(CCamera* pCamera){};
	virtual void setGeometry(MyGeometry * pGeometry){};
	virtual void setScene(scene * pScene){};
	virtual void setGeomtryIndex(int index){};
protected:
	std::string m_vertexFileName,m_geometryFileName,m_fragmentFileName;
	glslShaderLoader m_loader;

};
#endif