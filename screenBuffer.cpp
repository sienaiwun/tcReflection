#include "screenBuffer.h"
#include "assert.h"
#include "Geometry.h"
static GLenum mybuffers2[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
                           GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT, GL_COLOR_ATTACHMENT6_EXT };
void ScreenBuffer::init()
{
	assert(m_bufferHeight*m_bufferHeight);
	assert(m_windowWidth*m_windowHeight);
	m_fbo.init();
	CHECK_ERRORS();
  m_texShader.init();
}
void ScreenBuffer::clear()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   glEnable(GL_DEPTH_TEST);
}
void ScreenBuffer::clearScreen()
{
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   glDrawBuffer(GL_BACK);
  float clear_value = 1.f;
  glClearColor(clear_value, clear_value, clear_value, clear_value);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
void ScreenBuffer::directDrawToScreen(glslShader & shader)
{
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
  float clear_value = 1.f;
  glClearColor(clear_value, clear_value, clear_value, clear_value);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
   glViewport(0,0,(int) 512, (int)		512);
  MyGeometry::drawQuad(shader);
 
}
void ScreenBuffer::drawToScreen(glslShader & shader)
{
	m_fbo.begin();
  MyGeometry::drawQuad(shader);
  m_fbo.end();
/*
	BYTE* pTexture = NULL;

	int width = m_bufferWidth;
	int height =m_bufferHeight;

	pTexture = new BYTE[m_bufferWidth* m_bufferHeight * 3];
	memset(pTexture, 0, m_bufferWidth* m_bufferHeight * 3 * sizeof(BYTE));

	float *pData = NULL;
	pData = new float[width * height * 3];
	memset(pData, 0, width * height * 3 * sizeof(float));

	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	Fbo::SaveBMP("123.bmp", pTexture, width, height);

	int w = width,scale =1,x= 1,y=1;
	int index = y*w+x;
	float r = pTexture[4*index]*scale;
	float g = pTexture[4*index+1]*scale;
	float b = pTexture[4*index+2]*scale;
	float a = pTexture[4*index+3]*scale;

	delete pData;
	delete pTexture;

	*/
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_2D, m_fbo.getTexture(0));
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  m_texShader.setParemeter(m_fbo.getTexture(0));
  Fbo::drawScreenBackBuffer(m_windowWidth,m_windowHeight);
  MyGeometry::drawQuad(m_texShader);

}