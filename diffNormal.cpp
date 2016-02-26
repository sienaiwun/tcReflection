#include "diffNormal.h"
#include "Camera.h"
#include "refFrame.h"
void DiffNormalShader::init()
{
	/*m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_generateUniform = m_loader.getUniform("generateRefTex");
	m_newRayUniform = m_loader.getUniform("newRefTex");

	GLuint	m_newWorldPosTex = m_loader.getUniform("NewWorldPos");
	GLuint m_reflectTex = m_loader.getUniform("Reflection");
	*/
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());

	m_colorTexSlot = m_loader.getUniform("colorTex");
	m_normalTexSlot = m_loader.getUniform("normalTex");
	m_posTexSlot = m_loader.getUniform("posTex");
	m_res_slot = m_loader.getUniform("resolution");
	m_mvpSlot= m_loader.getUniform("MVP");
	m_mvpInv_slot= m_loader.getUniform("mvpInv");
	m_cameraPos_slot = m_loader.getUniform("cameraPos");
	m_modelView_slot = m_loader.getUniform("modeView");
	m_modelViewInv_slot = m_loader.getUniform("modeViewInv");
	m_bbminSlot = m_loader.getUniform("imageMin");
	m_bbmax_slot = m_loader.getUniform("imageMax");

}
void DiffNormalShader::setParemeter()
{
	setColorTex(m_pfbo->getTexture(2));
	setNormalTex(m_pfbo->getTexture(1));
	setPositonTex(m_pfbo->getTexture(0));
	m_cameraPos = m_pCamera->getCameraPos();
	m_mvp  = m_pCamera->getMvpMat();
	m_modelView = m_pCamera->getModelViewMat();
}
void DiffNormalShader::bindParemeter()
{
	setParemeter();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_posTex);
	glUniform1i(m_posTexSlot,0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,m_normalTex);
	glUniform1i(m_normalTexSlot,1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,m_colorTex);
	glUniform1i(m_colorTexSlot,2);
	glUniform2f(m_res_slot,m_res.x,m_res.y);
	glUniformMatrix4fv(m_mvpSlot,1,GL_FALSE,m_mvp);
	nv::matrix4f invMatrix = inverse(nv::matrix4f(m_mvp));
	glUniformMatrix4fv(m_mvpInv_slot,1,GL_FALSE,invMatrix.get_value());
	glUniform3f(m_cameraPos_slot,m_cameraPos.x,m_cameraPos.y,m_cameraPos.z);

	glUniformMatrix4fv(m_modelView_slot,1,GL_FALSE,m_modelView);
	nv::matrix4f invModelViewMatrix = inverse(nv::matrix4f(m_modelView));
	glUniformMatrix4fv(m_modelViewInv_slot,1,GL_FALSE,invModelViewMatrix.get_value());
	glUniform2f(m_bbminSlot,m_pCamera->getImageMin().x,m_pCamera->getImageMin().y);
	glUniform2f(m_bbmax_slot,m_pCamera->getImageMax().x,m_pCamera->getImageMax().y);
	
	
}
void DiffNormalShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void DiffNormalShader::end()
{
	m_loader.DisUse();
}