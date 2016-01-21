#include "reflectionShader.h"
#include "scene.h"
void reflectShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_vmpBinding = m_loader.getUniform("MVP");
	m_cameraPosBinding = m_loader.getUniform("cameraPos");
	m_lightPosBinding = m_loader.getUniform("lightPos");
	m_objectDiffuseBinding = m_loader.getUniform("objectDiffuseColor");
	m_objectTexBinding = m_loader.getUniform("objectTex");
	m_hasTex = m_loader.getUniform("hasTex");
	m_objectId = m_loader.getUniform("objectId");
	m_reflectValueBinding = m_loader.getUniform("reflectBlender");
	m_reflectMapBinding = m_loader.getUniform("reflectMap");

	m_windowsSizeSlot = m_loader.getUniform("windowsSize");
}

void reflectShader::bindParemeter()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_reflectionMap);
	glUniform1i(m_reflectMapBinding,0);
	glUniform2f(m_windowsSizeSlot,m_windowSize.x,m_windowSize.y);
}
void reflectShader::setScene(scene * pScene)
{
	glUniform3f(m_lightPosBinding,pScene->getLightPos().x,pScene->getLightPos().y,pScene->getLightPos().x);
}
void reflectShader::setGeomtryIndex(int i)
{
	glUniform1i(m_objectId,i);
}
void reflectShader::setReflectMap(GLuint reflectMap)
{
	m_reflectionMap = reflectMap;	
}
void reflectShader::setWindowSize(int width,int height)
{
	m_windowSize = nv::vec2f(width,height);
}
void reflectShader::setGeometry(MyGeometry * pGeometry)
{
	MyMeterial * pMet = pGeometry->getMaterial();
	CHECK_ERRORS();
	glUniform1i(m_hasTex,pMet->hasTx());
	CHECK_ERRORS();
	glUniform1f(m_reflectValueBinding,pGeometry->reflectValue);
	if(pGeometry->getMaterial()->hasTx())
	{	
		CHECK_ERRORS();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,pMet->getTxId());
		glUniform1i(m_objectTexBinding,1);
		CHECK_ERRORS();
	}
	else 
	{
		CHECK_ERRORS();
		float3 diffuse = pMet->getDiffuse();
		glUniform3f(m_objectDiffuseBinding,diffuse.x,diffuse.y,diffuse.z);
	}
}
void reflectShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void reflectShader::end()
{
	m_loader.DisUse();
}
void reflectShader::setCamera(CCamera *pCamera)
{
	m_mvp = pCamera->getMvpMat();
	glUniformMatrix4fv(m_vmpBinding,1,GL_FALSE,m_mvp);
}