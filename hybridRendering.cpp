#include "hybridRendering.h"
void HybridShader::init()
{
	
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_diffuseSlot = m_loader.getUniform("diffuseTex");
	m_tex1Slot= m_loader.getUniform("tex1");
	m_tex2Slot= m_loader.getUniform("tex2");;
	m_ratio1Slot= m_loader.getUniform("ratio1");
	m_ratio2Slot= m_loader.getUniform("ratio2");;
	
}
void HybridShader::bindParemeter()
{
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_diffuseTex);
	glUniform1i(m_diffuseSlot,0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,m_tex1);
	glUniform1i(m_tex1Slot,1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,m_tex2);
	glUniform1i(m_tex2Slot,2);

	float radio2 = abs(m_time1-m_currentTime)/(float)abs(m_time2-m_time1);
	float radio1 = abs(m_time2-m_currentTime)/(float)abs(m_time2-m_time1);
	glUniform1f(m_ratio1Slot,radio1);
	glUniform1f(m_ratio2Slot,radio2);
}
void HybridShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void HybridShader::end()
{
	m_loader.DisUse();
}