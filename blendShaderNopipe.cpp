#include "BlendShaderNoPipe.h"
void BlendShaderNoPipe::init()
{
	/*m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_generateUniform = m_loader.getUniform("generateRefTex");
	m_newRayUniform = m_loader.getUniform("newRefTex");

	GLuint	m_newWorldPosTex = m_loader.getUniform("NewWorldPos");
	GLuint m_reflectTex = m_loader.getUniform("Reflection");
	*/
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());

	
	m_orginTexSlot = m_loader.getUniform("originColorTex");
	m_additionTexSlot = m_loader.getUniform("additionColorTex");
	independentSlot = m_loader.getUniform("viewIndependColor");
	dependentSlot = m_loader.getUniform("viewDependColor");
	
}
void BlendShaderNoPipe::bindParemeter()
{
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_orginTex);
	glUniform1i(m_orginTexSlot,0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,m_additionTex);
	glUniform1i(m_additionTexSlot,1);

	glUniform3f(dependentSlot,m_viewDependentColor.x,m_viewDependentColor.y,m_viewDependentColor.z);
	glUniform3f(independentSlot,m_viewIndependentColor.x,m_viewIndependentColor.y,m_viewIndependentColor.z);

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
void BlendShaderNoPipe::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void BlendShaderNoPipe::end()
{
	m_loader.DisUse();
}