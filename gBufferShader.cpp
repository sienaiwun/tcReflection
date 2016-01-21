#include "gBufferShader.h"
#include "scene.h"
void GbufferShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_vmpBinding = m_loader.getUniform("MVP");

	m_cameraPosBinding = m_loader.getUniform("cameraPos");
	m_lightPosBinding = m_loader.getUniform("lightPos");
	
	m_objectDiffuseBinding = m_loader.getUniform("objectDiffuseColor");
	m_objectTexBinding = m_loader.getUniform("objectTex");
	m_hasTex = m_loader.getUniform("hasTex");
	m_objectId = m_loader.getUniform("objectId");
	m_reflectFactor = m_loader.getUniform("reflectFactor");
	
}

void GbufferShader::bindParemeter()
{
	

}
void GbufferShader::setScene(scene * pScene)
{
	glUniform3f(m_lightPosBinding,pScene->getLightPos().x,pScene->getLightPos().y,pScene->getLightPos().x);
}
void GbufferShader::setGeomtryIndex(int i)
{
	glUniform1i(m_objectId,i);

}
void GbufferShader::setGeometry(MyGeometry * pGeometry)
{
	CHECK_ERRORS();
	MyMeterial * pMet = pGeometry->getMaterial();
	glUniform1i(m_hasTex,pMet->hasTx());
	glUniform1f(m_reflectFactor,pGeometry->reflectValue);
	CHECK_ERRORS();
	if(pGeometry->getMaterial()->hasTx())
	{
		glActiveTexture(GL_TEXTURE0);
			CHECK_ERRORS();
		glBindTexture(GL_TEXTURE_2D,pMet->getTxId());
			CHECK_ERRORS();
		glUniform1i(m_objectTexBinding,0);
			CHECK_ERRORS();
	
	}
	else 
	{
		float3 diffuse = pMet->getDiffuse();
		glUniform3f(m_objectDiffuseBinding,diffuse.x,diffuse.y,diffuse.z);
	}
}
void GbufferShader::begin()
{
	m_loader.useShder();
	//bindParemeter();
}
void GbufferShader::end()
{
	m_loader.DisUse();
}
void GbufferShader::setCamera(CCamera *pCamera)
{
	m_mvp = pCamera->getMvpMat();
	glUniformMatrix4fv(m_vmpBinding,1,GL_FALSE,m_mvp);
	glUniform3f(m_cameraPosBinding,pCamera->getCameraPos().x,pCamera->getCameraPos().y,pCamera->getCameraPos().z);
}