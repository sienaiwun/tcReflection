#include "mergeShader.h"
void MergeShader::init()
{
	m_loader.loadShader(m_vertexFileName.c_str(),0,m_fragmentFileName.c_str());
	m_optixTex = m_loader.getUniform("OptixTex");
	m_computeTex = m_loader.getUniform("ComputeTex");
	m_frameCount = m_loader.getUniform("FrameCount");


}
void MergeShader::bindParemeter()
{
	


}
void MergeShader::begin()
{
	m_loader.useShder();
	bindParemeter();
}
void MergeShader::end()
{
	m_loader.DisUse();
}