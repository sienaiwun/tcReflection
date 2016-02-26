#include "CompareShader.h"
void CompareShader::init()
{
	/*m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_generateUniform = m_loader.getUniform("generateRefTex");
	m_newRayUniform = m_loader.getUniform("newRefTex");

	GLuint	m_newWorldPosTex = m_loader.getUniform("NewWorldPos");
	GLuint m_reflectTex = m_loader.getUniform("Reflection");
	*/
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());

	
	m_tex1_slot = m_loader.getUniform("tex1");
	
	m_tex2_slot = m_loader.getUniform("tex2");
}
void CompareShader::bindParemeter()
{
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_tex1);
	glUniform1i(m_tex1_slot,0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,m_tex2);
	glUniform1i(m_tex2_slot,1);
}
void CompareShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void CompareShader::end()
{
	m_loader.DisUse();
}