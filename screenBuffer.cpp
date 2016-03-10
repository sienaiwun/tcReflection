#include "screenBuffer.h"
#include "assert.h"
#include "Geometry.h"
static GLenum mybuffers2[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
                           GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT, GL_COLOR_ATTACHMENT6_EXT };
void ScreenBuffer::init()
{
	assert(m_bufferHeight*m_bufferHeight);
	assert(m_windowWidth*m_windowHeight);
	glGenFramebuffersEXT(1, &m_fboId);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);
    glDrawBuffers(1, mybuffers2);

    glGenTextures (1, &m_sampleTex);
    glBindTexture(GL_TEXTURE_2D, m_sampleTex);
 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, initialWindowHeight, initialWindowWidth, 0,	GL_RGBA, GL_FLOAT, 0);
  */
 // gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA32F_ARB,initialWindowHeight, initialWindowWidth, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, m_bufferWidth, m_bufferHeight, 0,	GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 

  //glGenerateMipmapEXT(GL_TEXTURE_2D);

  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_sampleTex, 0);
  glGenRenderbuffersEXT(1,&m_depthTex);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_depthTex);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,m_bufferWidth, m_bufferHeight);
  //将深度缓冲与FBO绑定
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,m_depthTex);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);
   glViewport(0,0,(int) m_bufferWidth, (int)		m_bufferHeight);

  float clear_value = 1.f;
  glClearColor(clear_value, clear_value, clear_value, clear_value);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  MyGeometry::drawQuad(shader);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_2D, m_sampleTex);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  m_texShader.setParemeter(m_sampleTex);
  Fbo::drawScreenBackBuffer(m_windowWidth,m_windowHeight);
  MyGeometry::drawQuad(m_texShader);

}