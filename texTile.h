#include "geometry.h"
#include "Fbo.h"
#include "drawQuad.h"
#ifndef TEXTILE_H
#define TEXTILE_H


class TexTile
{
public:
	TexTile(int w,int h):m_height(h),m_width(w)
	{
	}
	void init();

	void renderToPlace(Fbo& sourceFbo,Fbo& targetFbo, int startw, int startY,int slot = 0,int targetSlot =0);
	Fbo m_tempFbo;
	inline GLuint getFboId()
	{
		return m_tempFbo.getFboId();
	}
	void renderToScreen(int startW,int startY);
	void renderToScreen(Fbo &targetFbo,int startW,int startY);
	void renderToScreen(Fbo & targetFbo,int startw, int startY,int sourceSlot ,int targetSlot);
	void setDrawQuadeShader(DrawQuadShader* shader)
	{
		m_pDrawQuadShader = shader;
	}
private:
	DrawQuadShader * m_pDrawQuadShader;
	void renderMipMapFbo( Fbo &sourceFbo,int texId);
	int m_height,m_width;
};
#endif