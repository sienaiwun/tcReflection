#include "glslShader.h"
#ifndef CompareShader_H
#define CompareShader_H
class CompareShader:public glslShader
{
public :
	CompareShader()
	{
		m_vertexFileName = "Shader/CompareShader.vert";
		m_fragmentFileName = "Shader/CompareShader.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setTex1(int tex)
	{
		m_tex1 = tex;
	}
	inline void setTex2(int tex)
	{
		m_tex2 = tex;
	}
	
private:

	GLuint m_tex1,m_tex2;
	GLuint m_tex1_slot,m_tex2_slot;
	

};
#endif