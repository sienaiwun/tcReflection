#include "glslShader.h"
#ifndef Histogram_h
#define Histogram_h
class Histogram:public glslShader
{
public :
	Histogram()
	{
		m_vertexFileName = "Shader/Histogram.vert";
		m_fragmentFileName = "Shader/Histogram.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setReflectTex(int tex)
	{
		m_reflectTex = tex;
	}
	
private:

	GLuint m_reflectTex;
	GLuint m_reflectTexSlot;
	

};
#endif