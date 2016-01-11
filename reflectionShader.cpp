#include "reflectionShader.h"
#include "scene.h"
void reflectShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_vmpBinding = m_loader.getUniform("MVP");

	m_cameraPosBinding = m_loader.getUniform("cameraPos");
	m_lightPosBinding = m_loader.getUniform("lightPos");
	
	m_objectDiffuseBinding = m_loader.getUniform("diffuseColor");
	m_objectTexBinding = m_loader.getUniform("objectDiffuseColor");
	m_hasTex = m_loader.getUniform("hasTex");
	m_objectId = m_loader.getUniform("objectId");

	m_reflectValueBinding = m_loader.getUniform("reflectorValue");
	m_reflectMapBinding = m_loader.getUniform("reflectMap");
}

void reflectShader::bindParemeter()
{
	glUniformMatrix4fv(m_vmpBinding,1,GL_FALSE,m_mvp);

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
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,reflectMap);
	glUniform1i(m_reflectMapBinding,1);
	
}
void reflectShader::setGeometry(MyGeometry * pGeometry)
{
	CHECK_ERRORS();
	MyMeterial * pMet = pGeometry->getMaterial();
	
	CHECK_ERRORS();
	glUniform1i(m_hasTex,pMet->hasTx());
	glUniform1f(m_reflectValueBinding,pGeometry->reflectValue);
	
	CHECK_ERRORS();
	if(pGeometry->getMaterial()->hasTx())
	{	
		CHECK_ERRORS();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,pMet->getTxId());
		
		CHECK_ERRORS();
		glUniform1i(m_objectTexBinding,0);
	
	}
	else 
	{
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
	//glUniform3f(m_cameraPosBinding,pCamera->getCameraPos().x,pCamera->getCameraPos().y,pCamera->getCameraPos().z);
}