#include "TexTile.h"
#include "geometry.h"
void TexTile::init()
{
	m_tempFbo.set(1,m_width,m_height);
	m_tempFbo.init();
}
void TexTile::renderMipMapFbo( Fbo &sourceFbo,int texId)
{

	 m_tempFbo.attachId();
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 glViewport(0, 0, m_width, m_height);
	 MyGeometry::drawTex(sourceFbo.getTexture(texId));
	 m_tempFbo.end();
}
void TexTile::renderToPlace(Fbo& sourceFbo,Fbo& targetFbo, int startw, int startY,int sourceSlot ,int targetSlot)
{
	renderMipMapFbo(sourceFbo,sourceSlot);
	m_tempFbo.attachId();
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBindTexture(GL_TEXTURE_2D,targetFbo.getTexture(targetSlot));
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,startw,startY,0,0,m_width,m_height);
	glBindTexture(GL_TEXTURE_2D,0);
	m_tempFbo.end();
}
void TexTile::renderToScreen(Fbo & targetFbo,int startw, int startY,int sourceSlot ,int targetSlot)
{

	m_tempFbo.attachId();
	m_tempFbo.SaveBMP("2.bmp",0);
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBindTexture(GL_TEXTURE_2D,targetFbo.getTexture(targetSlot));
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,startw,startY,0,0,m_width,m_height);
	glBindTexture(GL_TEXTURE_2D,0);
	m_tempFbo.end();
	targetFbo.begin();
	targetFbo.SaveBMP("1.bmp",0);
	targetFbo.end();
}

void TexTile ::renderToScreen(Fbo &targetFbo,int startW,int startY)
{
	CHECK_ERRORS();
	 targetFbo.attachId();  // bind the screen FBO
	 glViewport(startW, 0, m_width , m_height);
	 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   
	 glClear(  GL_DEPTH_BUFFER_BIT);
	 m_pDrawQuadShader->setWorkingTex(m_tempFbo.getTexture(0));
	 MyGeometry::drawQrad(*m_pDrawQuadShader);
	 targetFbo.end();
}
void TexTile::renderToScreen(int startw,int startY)
{
	 CHECK_ERRORS();
	 glBindFramebuffer(GL_FRAMEBUFFER, 0);     // bind the screen FBO
	 glViewport(startw, 0, startw+m_width , m_height);
	 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   

	 glClear(  GL_DEPTH_BUFFER_BIT);
	 m_pDrawQuadShader->setWorkingTex(m_tempFbo.getTexture(0));
	 MyGeometry::drawQrad(*m_pDrawQuadShader);
	 
	/* glMatrixMode(GL_MODELVIEW);
	 glPushMatrix();
	 glLoadIdentity();
	 glMatrixMode(GL_PROJECTION);
	 glPushMatrix();
	 glLoadIdentity();
	 
	 CHECK_ERRORS();
	 glBindFramebuffer(GL_READ_FRAMEBUFFER,m_tempFbo.getFboId());
	 glReadBuffer(GL_COLOR_ATTACHMENT0);
	 glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	 glBlitFramebuffer(0, 0, m_width, m_height, startw, startY, startw+m_width, startY+m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	 glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	 CHECK_ERRORS();

	 glPopMatrix();
	 glMatrixMode(GL_MODELVIEW);
     glPopMatrix();
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,0);*/

}