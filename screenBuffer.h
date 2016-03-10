#include "Fbo.h"
#include "texShader.h"
#include "glslShader.h"
#ifndef SCREENBUFFER_H
#define SCREENBUFFER_H
class ScreenBuffer
{
public:
	ScreenBuffer(int w,int h):m_windowWidth(w),m_windowHeight(h)
	{
	}
	inline void setBuffersize(int w,int h)
	{
		m_bufferWidth = w;
		m_bufferHeight = h;
	}
	void clear();
	void clearScreen();
	void drawToScreen(glslShader& shader);
	void directDrawToScreen(glslShader & shader);
	void init();
private:
	int m_windowWidth,m_windowHeight;
	int m_bufferWidth,m_bufferHeight;
	GLuint m_sampleFbo;
	GLuint m_sampleTex;
	GLuint m_depthTex;
	GLuint m_fboId;
	TexShader m_texShader;
};
#endif