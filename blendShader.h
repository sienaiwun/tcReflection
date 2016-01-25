#include "glslShader.h"
#ifndef BLENDSHADER_H
#define BLENDSHADER_H
class BlendShader:public glslShader
{
public :
	BlendShader()
	{
		m_vertexFileName = "Shader/blendShader.vert";
		m_fragmentFileName = "Shader/blendShader.frag"; 
		m_usingAddition = 0;
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setDiffuseTex(int tex)
	{
		m_diffuseTex = tex;
	}
	inline void setReflectTex(int tex)
	{
		m_reflectTex = tex;
	}
	inline void setNewReflectTex(int tex)
	{
		m_newRelfectTex = tex;
		m_usingAddition = 1;
	}
	inline void setParemeter(int a,int b)
	{
	}
	inline void setUsingAdditional(int m)
	{
		m_usingAddition = m;
	}
private:
	GLuint m_reflectTex;
	GLuint m_diffuseTex;
	GLuint m_reflectTexSlot;
	GLuint m_diffuseTexSlot;
	GLuint m_newRelfectTex;
	GLuint m_newRelfectTexSlot;
	GLuint m_useAdditionalTexSlot;
	int m_usingAddition;
};
#endif