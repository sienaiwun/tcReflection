#include "blendShader.h"
void BlendShader::init()
{
	/*m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_generateUniform = m_loader.getUniform("generateRefTex");
	m_newRayUniform = m_loader.getUniform("newRefTex");

	GLuint	m_newWorldPosTex = m_loader.getUniform("NewWorldPos");
	GLuint m_reflectTex = m_loader.getUniform("Reflection");
	*/
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());

	
	m_generateUniform = m_loader.getUniform("generateRefTex");
	m_newRayUniform = m_loader.getUniform("newRefTex");
}
void BlendShader::bindParemeter()
{
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_generateTexId);
	glUniform1i(m_generateUniform,0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,m_newRayTexId);
	glUniform1i(m_newRayUniform,1);


	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_bindingReflectTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,m_bindingTransTex);

	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,m_bindingTransPosTex);


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,m_bindingTransReposTex);

	glUniform1i(m_refelctTexUniform,0);
	glUniform1i(m_transVecTexUniform,1);
	glUniform1i(m_tranWorPosTexUniform,2);
	glUniform1i(m_tranRePosTexUniform,3);
	glUniform3f(m_clearColor,m_bindingClearColor.x,m_bindingClearColor.y,m_bindingClearColor.z);*/
}
void BlendShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void BlendShader::end()
{
	m_loader.DisUse();
}