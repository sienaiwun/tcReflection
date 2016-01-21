#include "transShader.h"
void TranShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),m_geometryFileName.c_str(),m_fragmentFileName.c_str());

	m_transVecTexUniform = m_loader.getUniform("TransVecTex");
	m_refelctTexUniform = m_loader.getUniform("RefelctTex");
	m_tranWorPosTexUniform = m_loader.getUniform("WorldPosTex");
	m_tranRePosTexUniform = m_loader.getUniform("RePosTex");
	m_resUniform = m_loader.getUniform("resolution");
	m_clearColor = m_loader.getUniform("ClearColor");
	//m_additionalTexSlot = m_loader.getUniform("IdMap");

}

void TranShader::bindParemeter()
{
	//m_bindingReflectTex,m_bindingTransTex,m_bindingTransPosTex,m_bindingTransReposTex
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_bindingReflectTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,m_bindingTransTex);

	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,m_bindingTransPosTex);


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,m_bindingTransReposTex);

	/*glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D,m_additionalTex);
	glUniform1i(m_additionalTexSlot,4);
	*/
	glUniform1i(m_refelctTexUniform,0);
	glUniform1i(m_transVecTexUniform,1);
	glUniform1i(m_tranWorPosTexUniform,2);
	glUniform1i(m_tranRePosTexUniform,3);
	glUniform3f(m_clearColor,m_bindingClearColor.x,m_bindingClearColor.y,m_bindingClearColor.z);
	glUniform2f(m_resUniform,m_res.x,m_res.y);
}
void TranShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void TranShader::end()
{
	m_loader.DisUse();
}