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
	inline void setEqual(int isEqual)
	{
		m_isSetEqual = isEqual;
	}
	inline void setCompareColor(nv::vec3f color)
	{
		m_compareColor = color;
	}
private:

	GLuint m_reflectTex;
	GLuint m_reflectTexSlot;
	bool m_isSetEqual;
	nv::vec3f m_compareColor;
	GLuint m_equalSlot;
	GLuint m_compareColorSlot;
};
#endif