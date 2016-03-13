#include "glslShader.h"
#ifndef BLENDSHADERNOPIPE_H
#define BLENDSHADERNOPIPE_H
class BlendShaderNoPipe:public glslShader
{
public :
	BlendShaderNoPipe()
	{
		m_vertexFileName = "Shader/blendShaderNopipe.vert";
		m_fragmentFileName = "Shader/blendShaderNopipe.frag"; 
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setOriginTex(int tex)
	{
		m_orginTex = tex;
	}
	inline void setAdditionalTex(int tex)
	{
		m_additionTex = tex;
	}
	inline void setViewDependentColor(nv::vec3f color)
	{
		m_viewDependentColor = color;
	}
	inline void setViewIndependColor(nv::vec3f color)
	{
		m_viewIndependentColor = color;
	}
private:
	nv::vec3f m_viewIndependentColor;
	nv::vec3f m_viewDependentColor;
	GLuint independentSlot;
	GLuint dependentSlot;
	GLuint m_additionTexSlot,m_orginTexSlot;
	GLuint m_additionTex,m_orginTex;
};
#endif