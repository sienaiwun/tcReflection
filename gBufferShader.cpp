#include "gBufferShader.h"
void GbufferShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_vmpBinding = m_loader.getUniform("MVP");
}
void GbufferShader::bindParemeter()
{
	glUniformMatrix4fv(m_vmpBinding,1,GL_FALSE,m_mvp);

}
void GbufferShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void GbufferShader::end()
{
	m_loader.DisUse();
}
void GbufferShader::setCamera(CCamera *pCamera)
{
	m_mvp = pCamera->getMvpMat();
}