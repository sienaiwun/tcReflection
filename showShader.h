#include "glslShader.h"
#ifndef showShader_H
#define showShader_H
class showShader:public glslShader
{
public :
	showShader()
	{
		m_vertexFileName = "Shader/showShader.vert";
		m_fragmentFileName = "Shader/showShader.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setTex1(int tex)
	{
		m_tex1 = tex;
	}
	
	
private:

	GLuint m_tex1,m_tex2;
	GLuint m_tex1_slot,m_tex2_slot;
	

};
#endif