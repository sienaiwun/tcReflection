#include "glslShader.h"
#ifndef IMG_WARPINGNOPIPE_H
#define IMG_WARPINGNOPIPE_H
class ImgWarpingShaderNoPipe:public glslShader
{
public :
	ImgWarpingShaderNoPipe()
	{
		m_vertexFileName = "Shader/imgWarpingNopipe.vert";
		m_fragmentFileName = "Shader/imgWarpingNopipe.frag";
		m_geometryFileName = "Shader/imgWarpingNopipe.geom";
		
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setClearColor(nv::vec3f color)
	{
		m_bindingClearColor = color;
	}
	inline void setRes(int  x,int y)
	{
		m_res = nv::vec2f(x,y);
	}
	inline void setMvpMatrix(const float* matP)
	{
		m_currentMvpMatrix = matP;
	}
	inline void setRefColorTex(GLuint tex)
	{
		m_refColorTex = tex;
	}
	inline void setRefPosTex(GLuint tex)
	{
		m_refPosTex = tex;
	}
	inline void setRefNormalTex(GLuint tex)
	{
		m_refNorTex = tex;
	}
	inline void setReflectedTex(GLuint tex)
	{
		m_reflectedColorTex = tex;
	}
	inline void setEdgeTex(GLuint tex)
	{
		m_edgeTex = tex;
	}
	inline void setNaive(bool is)
	{
		m_isNaive = is;
	}

private:

	nv::vec2f m_res;
	nv::vec3f m_bindingClearColor;
	const float*  m_currentMvpMatrix;
	GLuint m_matrixSlot,m_worldPosSlot,m_refColorSlot,m_resolutionSlot,m_clearColorSlot,m_resSlot,m_refNormalSlot;
	GLuint m_refColorTex,m_refPosTex,m_refNorTex;
	GLuint m_reflectedColorTex;
	GLuint m_reflectedColorTex_slot;
	GLuint m_isNaiveRendering_slot;
	GLuint m_edgeTexSlot,m_edgeTex;
	bool m_isNaive;
	//GLuint m_additionalTex;
	/*
	glUniform1i(m_refelctTexUniform,0);
	glUniform1i(m_transVecTexUniform,1);
	glUniform1i(m_tranWorPosTexUniform,2);
	glUniform1i(m_tranRePosTexUniform,3);
	*/
};
#endif