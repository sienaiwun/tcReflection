#include "glslShader.h"
#ifndef GBUFFERSHADER_H
#define GBUFFERSHADER_H
class GbufferShader:public glslShader
{
public :
	GbufferShader()
	{
		m_vertexFileName = "Shader/gBuffer.vert";
		m_fragmentFileName = "Shader/gBuffer.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setParemeter(GLuint reflectTex,GLuint worldPosTex,GLuint lastWorld,const float* lastmvp)
	{
		
	}
	virtual void setCamera(CCamera * pCamera);
	
private:
	GLuint m_vmpBinding;
	float* m_mvp;
	/*
	glUniform1i(m_refelctTexUniform,0);
	glUniform1i(m_transVecTexUniform,1);
	glUniform1i(m_tranWorPosTexUniform,2);
	glUniform1i(m_tranRePosTexUniform,3);
	*/
};
#endif