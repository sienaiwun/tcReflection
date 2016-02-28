#include "glslShader.h"
#ifndef REPROJECT_H
#define REPROJECT_H
class ReprojectShader:public glslShader
{
public :
	ReprojectShader()
	{
		m_vertexFileName = "Shader/Reprojec.vert";
		m_fragmentFileName = "Shader/Reprojec.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setParemeter(GLuint reflectTex,GLuint worldPosTex,GLuint lastWorld,const float* lastmvp)
	{
		m_bindingReflectTex = reflectTex;
		m_bindingNewWoldPos = worldPosTex;
		m_bindginLastWorldTex = lastWorld;
		m_bindingLastMvpFloat = lastmvp;
	}
	inline void setNormalTex(GLuint tex)
	{
		m_lastNormalTex = tex;
	}
	inline void setClearColor(nv::vec3f color)
	{
		m_bindingIndependentColor = color;
	}
	
private:
	GLuint m_newWorldPosTex;
	GLuint m_reflectTex;
	GLuint m_lastNormalTex;
	GLuint m_lastNormalTex_slot;

//Trans shader Uniform
	GLuint m_lastMvp;
	GLuint m_lastWorldPosTex;
	GLuint m_clearColor;
	const float* m_bindingLastMvpFloat;
	GLuint m_bindingNewWoldPos,m_bindingReflectTex,m_bindginLastWorldTex;
	nv::vec3f m_bindingIndependentColor;
	/*
	glUniform1i(m_refelctTexUniform,0);
	glUniform1i(m_transVecTexUniform,1);
	glUniform1i(m_tranWorPosTexUniform,2);
	glUniform1i(m_tranRePosTexUniform,3);
	*/
};
#endif