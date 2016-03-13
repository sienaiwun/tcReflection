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
		m_fbo.set(1,w,h);

	}
	void clear();
	void clearScreen();
	void drawToScreen(glslShader& shader);
	void directDrawToScreen(glslShader & shader);
	void init();
	void SaveBMP(const char *fileName, int id)
	{
		m_fbo.SaveBMP(fileName,id);
	}
	void debugPixel(int id, int x,int y,int scale =1 )
	{
		m_fbo.debugPixel( id,  x, y,scale  );
	}
private:
	int m_windowWidth,m_windowHeight;
	int m_bufferWidth,m_bufferHeight;
	TexShader m_texShader;
	Fbo m_fbo;
};
#endif