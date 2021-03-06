#include "main.h"
#include "macro.h"
#include "Fbo.h"
#include "Camera.h"
#include "macro.h"
#ifndef REFFRAME_H
#define REFFRAME_H
class RefFrame
{
public:
	GLuint m_reflectIndex;
	Fbo m_refGbuffer;
	GLuint m_frame;
	CCamera m_camera;
	inline CCamera& getCamera()
	{
		return m_camera;
	}
	inline GLuint& getOptixTex()
	{
		return m_reflectIndex;
	}
	/*
	Gluint m_additionalTex;
	inline GLuint& getAdditionalTex()
	{
		return m_additionalTex;
	}*/
	inline void setFrame(int slot)
	{
		m_frame = slot;
	}
	static RefFrame& getFrameByIndex(int index)
	{
		extern RefFrame g_FrameList[];
		return g_FrameList[index];
	}

    void init()
	{

			glGenTextures(1, &m_reflectIndex);
			glBindTexture(GL_TEXTURE_2D,m_reflectIndex);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, rasterWidth, rasterHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
			m_refGbuffer.set(3,rasterWidth, rasterHeight);
#if defined(DIFFNORMAL) || defined(EDGESHADER)
	        m_refGbuffer.set(4,rasterWidth, rasterHeight);
#endif
			m_refGbuffer.init();


			/*
			glGenTextures(1, &m_additionalTex);
			glBindTexture(GL_TEXTURE_2D,m_additionalTex);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, rasterWidth, rasterHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);*/
	}
	inline Fbo& getGbuffer()
	{
		return m_refGbuffer;
	}

};

#endif