#include "ImageWarpingShaderNopipe.h"
void ImgWarpingShaderNoPipe::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),m_geometryFileName.c_str(),m_fragmentFileName.c_str());
	m_worldPosSlot = m_loader.getUniform("worldTex");
	m_refColorSlot = m_loader.getUniform("colorTex");
	m_refNormalSlot = m_loader.getUniform("normalTex");
	m_resSlot = m_loader.getUniform("resolution");
	m_clearColorSlot = m_loader.getUniform("ClearColor");
	m_matrixSlot = m_loader.getUniform("MVP");
	m_reflectedColorTex_slot  = m_loader.getUniform("blendReflectTex");
}

void ImgWarpingShaderNoPipe::bindParemeter()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_refColorTex);
	glUniform1i(m_refColorSlot,0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,m_refPosTex);
	glUniform1i(m_worldPosSlot,1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,m_refNorTex);
	glUniform1i(m_refNormalSlot,2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,m_reflectedColorTex);
	glUniform1i(m_reflectedColorTex_slot,3);
	
	glUniform3f(m_clearColorSlot,m_bindingClearColor.x,m_bindingClearColor.y,m_bindingClearColor.z);
	glUniform2f(m_resSlot,m_res.x,m_res.y);
	glUniformMatrix4fv(m_matrixSlot,1,GL_FALSE,m_currentMvpMatrix);
}
void ImgWarpingShaderNoPipe::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void ImgWarpingShaderNoPipe::end()
{
	m_loader.DisUse();
}