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
	m_equalSlot = m_loader.getUniform("isEqual");
	m_compareColorSlot = m_loader.getUniform("compareColor");
	
}
void Histogram::bindParemeter()
{
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_reflectTex);
	glUniform1i(m_reflectTexSlot,0);

	glUniform1i(m_equalSlot,m_isSetEqual);
	glUniform3f(m_compareColorSlot,m_compareColor.x,m_compareColor.y,m_compareColor.z);
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