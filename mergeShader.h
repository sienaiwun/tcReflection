#include "glslShader.h"
#ifndef MERGESHADER_H
#define MERGESHADER_H
class MergeShader:public glslShader
{
public :
	MergeShader()
	{
		m_vertexFileName = "Shader/Merge.vert";
		m_fragmentFileName = "Shader/Merge.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setParemeter(GLuint reflectTex,GLuint worldPosTex,GLuint lastWorld,const float* lastmvp)
	{
		
	}
	
private:
	GLuint m_optixTex;
	GLuint m_computeTex;

//Trans shader Uniform
	GLuint m_frameCount;
	GLuint m_bindingNewWoldPos,m_bindingReflectTex,m_bindginLastWorldTex;
	/*
	glUniform1i(m_refelctTexUniform,0);
	glUniform1i(m_transVecTexUniform,1);
	glUniform1i(m_tranWorPosTexUniform,2);
	glUniform1i(m_tranRePosTexUniform,3);
	*/
};
#endif