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
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setParemeter(GLuint generateTex,GLuint newTex)
	{
		m_generateTexId = generateTex;
		m_newRayTexId = newTex;
	}
private:
	GLuint m_generateTexId;
	GLuint m_newRayTexId;

	GLuint m_generateUniform;
	GLuint m_newRayUniform;


	GLuint m_newWorldPosTex;
	GLuint m_reflectTex;

//Trans shader Uniform
	GLuint m_lastMvp;
	GLuint m_lastWorldPosTex;
	GLuint m_clearColor;
	const float* m_bindingLastMvpFloat;
	GLuint m_bindingNewWoldPos,m_bindingReflectTex,m_bindginLastWorldTex;
	nv::vec3f m_bindingIndependentColor;

};
#endif