#include "texShader.h"
void TexShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_texSlot = m_loader.getUniform("OptixTex");


}
void TexShader::bindParemeter()
{
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_texId);
	glUniform1i(m_texSlot,0);
}
void TexShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void TexShader::end()
{
	m_loader.DisUse();
}