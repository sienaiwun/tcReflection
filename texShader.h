#include "glslShader.h"
#ifndef TEXSHOWER_H
#define TEXSHOWER_H
class TexShader:public glslShader
{
public :
	TexShader()
	{
		m_vertexFileName = "Shader/texShader.vert";
		m_fragmentFileName = "Shader/texShader.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setParemeter(GLuint reflectTex)
	{
		m_texId = reflectTex;
	}
private:
	GLuint m_texId;
	GLuint m_texSlot;
};
#endif