#include "IDShader.h"
#include "scene.h"
void IDShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),m_geometryFileName.c_str(),m_fragmentFileName.c_str());
	m_vmpBinding = m_loader.getUniform("MVP");

	m_cameraPosBinding = m_loader.getUniform("cameraPos");
	m_lightPosBinding = m_loader.getUniform("lightPos");
	
	m_objectDiffuseBinding = m_loader.getUniform("objectDiffuseColor");
	m_objectTexBinding = m_loader.getUniform("objectTex");
	m_hasTex = m_loader.getUniform("hasTex");
	m_objectId = m_loader.getUniform("objectId");
	m_reflectFactor = m_loader.getUniform("reflectFactor");
	
}

void IDShader::bindParemeter()
{
	

}
void IDShader::setScene(scene * pScene)
{
	
}
void IDShader::setGeomtryIndex(int i)
{
	
}
void IDShader::setGeometry(MyGeometry * pGeometry)
{
	
}
void IDShader::begin()
{
	m_loader.useShder();
	//bindParemeter();
}
void IDShader::end()
{
	m_loader.DisUse();
}
void IDShader::setCamera(CCamera *pCamera)
{
	
	m_mvp = pCamera->getMvpMat();
	glUniformMatrix4fv(m_vmpBinding,1,GL_FALSE,m_mvp);
	glUniform3f(m_cameraPosBinding,pCamera->getCameraPos().x,pCamera->getCameraPos().y,pCamera->getCameraPos().z);
}