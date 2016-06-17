#include "glslShader.h"
#ifndef TRANSHADER_H
#define TRANSSHADER_H
class TranShader:public glslShader
{
public :
	TranShader()
	{
		m_vertexFileName = "Shader/Trans.vert";
		m_geometryFileName  = "Shader/Trans.geom";
		m_fragmentFileName = "Shader/Trans.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setParemeter(GLuint ReflectTex,GLuint TransTex, GLuint TransPosTex,GLuint TransReposTex)
	{
		
		m_bindingReflectTex= ReflectTex;
		m_bindingTransTex = TransTex;
		m_bindingTransPosTex = TransPosTex;
		m_bindingTransReposTex = TransReposTex;
	}
	inline void setClearColor(nv::vec3f color)
	{
		m_bindingClearColor = color;
	}
	inline void setRes(int  x,int y)
	{
		m_res = nv::vec2f(x,y);
	}
	inline void setEdgeTex(int tex)
	{
		m_edgeTex =tex ;
	}
	/*
	inline void setAdditionalTex(int tex)
	{
		m_additionalTex = tex;
	}*/
	
private:

	nv::vec2f m_res;
	GLuint m_resUniform;
	GLuint m_refelctTexUniform;
	GLuint m_transVecTexUniform;
	GLuint m_edgeTex,m_edgeTexSlot;
//Trans shader Uniform
	GLuint m_tranWorPosTexUniform;
	GLuint m_tranRePosTexUniform;
	GLuint m_clearColor;
	GLuint m_bindingReflectTex,m_bindingTransTex,m_bindingTransPosTex,m_bindingTransReposTex;
	GLuint m_additionalTexSlot;
	GLuint m_colorTexSlot;
	nv::vec3f m_bindingClearColor;
	//GLuint m_additionalTex;
	/*
	glUniform1i(m_refelctTexUniform,0);
	glUniform1i(m_transVecTexUniform,1);
	glUniform1i(m_tranWorPosTexUniform,2);
	glUniform1i(m_tranRePosTexUniform,3);
	*/
};
#endif