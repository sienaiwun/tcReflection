#include "glslShader.h"
#ifndef HYBRIDSHADER_H
#define HYBRIDSHADER_H
class HybridShader:public glslShader
{
public :
	HybridShader()
	{
		m_vertexFileName = "Shader/hybridShader.vert";
		m_fragmentFileName = "Shader/hybridShader.frag"; 
		
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setDiffuseTex(int tex)
	{
		m_diffuseTex = tex;
	}
	
	
	inline void setParemeter(int a,int b)
	{
	}
	inline void setCurrentTime(int time)
	{
		m_currentTime = time;
	}
	inline void setTex1(int time1,GLuint tex1)
	{
		m_time1 = time1;
		m_tex1 = tex1;
	}
	inline void setTex2(int time2,GLuint tex2)
	{
		m_time2 = time2;
		m_tex2 = tex2;
	}
	
private:
	GLuint m_tex1,m_tex2;
	GLuint m_tex1Slot,m_tex2Slot;
	GLuint m_ratio1Slot,m_ratio2Slot;
	GLuint m_diffuseSlot;
	GLuint m_diffuseTex;
	int m_currentTime;
	int m_time1,m_time2;

};
#endif