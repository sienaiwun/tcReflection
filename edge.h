#include "glslShader.h"
#ifndef EDGESHDER_H
#define EDGESHDER_H
class Fbo;
class CCamera;
class EdgeShader:public glslShader
{
public :
	EdgeShader()
	{
		m_vertexFileName = "Shader/edge.vert";
		m_fragmentFileName = "Shader/edge.frag"; 
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setGbuffer(Fbo * p)
	{
		m_pfbo = p;
	}
	inline void setNormalTex(GLuint tex)
	{
		m_normalTex = tex;
	}
	inline void setPositonTex(GLuint tex)
	{
		m_posTex = tex;
	}
	inline void setColorTex(GLuint tex)
	{
		m_colorTex =  tex;
	}
	inline void setRes(nv::vec2f res)
	{
		m_res = res;
	}
	inline void setOptixTex(GLuint tex)
	{
		m_optixTex = tex;
	}
	inline void setCamera(CCamera * pC)
	{
		m_pCamera = pC;
	}
	
    inline void setParemeter();
private:
	GLuint m_optixTex;
	GLuint m_optixTexSlot;
	GLuint m_normalTex,m_normalTexSlot;
	GLuint m_posTex,m_posTexSlot;
	GLuint m_colorTex,m_colorTexSlot;
	GLuint m_res_slot,m_mvpSlot,m_mvpInv_slot;
	GLuint m_modelView_slot,m_modelViewInv_slot;
	GLuint m_cameraPos_slot;
	GLuint m_bbminSlot,m_bbmax_slot;
	CCamera * m_pCamera;
	Fbo * m_pfbo;
	float* m_mvp;
	float* m_modelView;
	nv::vec2f m_res;
	nv::vec3f m_cameraPos;
};
#endif