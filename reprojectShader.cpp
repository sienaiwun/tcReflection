#include "reprojectShader.h"
void ReprojectShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_newWorldPosTex = m_loader.getUniform("NewWorldPos");
	m_reflectTex = m_loader.getUniform("Reflection");
	m_lastMvp = m_loader.getUniform("LastMVP");
	m_lastWorldPosTex = m_loader.getUniform("LastWorldPos");

	m_clearColor = m_loader.getUniform("Clearcolor");

}
void ReprojectShader::bindParemeter()
{
	/*
	m_bindingReflectTex = reflectTex;
		m_bindingNewWoldPos = worldPosTex;
		m_bindginLastWorldTex = lastWorld;
		m_bindingLastMvpFloat = lastmvp;
		*/
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D,refGbuffer.getTexture(0));
	glBindTexture(GL_TEXTURE_2D,m_bindingReflectTex);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,m_bindingNewWoldPos);
	//glBindTexture(GL_TEXTURE_2D,refGbuffer.getTexture(0));

	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,m_bindginLastWorldTex);
	
	glUniform1i(m_reflectTex,0);
	glUniform1i(m_newWorldPosTex,1);
	glUniform1i(m_lastWorldPosTex,2);
	glUniform3f(m_clearColor,m_bindingIndependentColor.x,m_bindingIndependentColor.y,m_bindingIndependentColor.z);
	glUniformMatrix4fv(m_lastMvp,1,GL_FALSE,m_bindingLastMvpFloat);


}
void ReprojectShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void ReprojectShader::end()
{
	m_loader.DisUse();
}