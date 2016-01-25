#include "Histogram.h"
void Histogram::init()
{
	/*m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_generateUniform = m_loader.getUniform("generateRefTex");
	m_newRayUniform = m_loader.getUniform("newRefTex");

	GLuint	m_newWorldPosTex = m_loader.getUniform("NewWorldPos");
	GLuint m_reflectTex = m_loader.getUniform("Reflection");
	*/
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());

	
	m_reflectTexSlot = m_loader.getUniform("reflectTex");
	
}
void Histogram::bindParemeter()
{
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_reflectTex);
	glUniform1i(m_reflectTexSlot,0);
}
void Histogram::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void Histogram::end()
{
	m_loader.DisUse();
}