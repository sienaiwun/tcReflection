
#include "refFrame.h"
#include "cuda.h"
#include "macro.h"


#ifndef CUDARESOURCE_H
#define CUDARESOURCE_H
class CudaResource
{
public:
	CudaResource(CCamera refCamera,CCamera currentCamera):m_curretnCamera(currentCamera),m_refCamera(refCamera)
	{
	}
	CCamera &m_curretnCamera,&m_refCamera;
	CudaTexResourse m_poxCudaTex,m_normalCudaTex,m_reflectCudaTex,m_finalEffectCudaTex;
	CudaPboResource m_vectorCudaArray,m_finalEffectCudaArray;
	void init();
	void setCurrentCamer( CCamera &Camera);
	void setRefFrame(RefFrame &frame);
	void predict();
	GLuint getPredictTex();
	void tracingArtifact();
	void reductionGPU();

};
#endif // !CUDARESOURCE_H
