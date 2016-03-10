#include "IDShader.h"
#include "scene.h"
void IDShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),m_geometryFileName.c_str(),m_fragmentFileName.c_str());
	m_vmpBinding = m_loader.getUniform("MVP");

	m_vertexTotal_Slot = m_loader.getUniform("m_total_vertex");
	
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
	glUniform1f(m_vertexTotal_Slot,pGeometry->m_totalVertexNumber);
	
}
void IDShader::begin()
{
	m_loader.useShder();
//	bindParemeter();
}
void IDShader::end()
{
	m_loader.DisUse();
}
void IDShader::setCamera(CCamera *pCamera)
{
	
	m_mvp = pCamera->getMvpMat();
	glUniformMatrix4fv(m_vmpBinding,1,GL_FALSE,m_mvp);
	
}