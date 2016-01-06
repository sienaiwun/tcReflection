#include "cudaResource.h"
void CudaResource::init()
{
	m_poxCudaTex.set(0,rasterWidth,rasterHeight,worldPosRef_t);
	m_normalCudaTex.set(0,rasterWidth,rasterHeight,worldNormalRef_t);
	m_reflectCudaTex.set(0,rasterWidth,rasterHeight,reflecionRef_t);
	
	
	
//°ó¶¨opengl PBOµ½cuda
	
	m_vectorCudaArray.set(rasterWidth,rasterHeight,float4_t);
	m_vectorCudaArray.init();
	
	m_finalEffectCudaTex.set(0,rasterWidth,rasterHeight,finalEffect_t);
	
	m_finalEffectCudaArray.set(rasterWidth,rasterHeight,test_t);
	m_finalEffectCudaArray.init();


	int w = rasterWidth;
	int h = rasterHeight;
	MappingContantPeremeter(&w,&h);
	InitThrust();

}
void CudaResource::setCurrentCamer(CCamera & camera)
{
	m_curretnCamera= camera;
}
void CudaResource::setRefFrame(RefFrame &frame)
{	
	m_poxCudaTex.set(frame.getGbuffer().getTexture(0));
	m_normalCudaTex.set(frame.getGbuffer().getTexture(1));
	m_reflectCudaTex.set(frame.getOptixTex());
	m_refCamera = frame.getCamera();
}
void trasportCamera()
{

}
void CudaResource::predict()
{
	float modelViewF[16],projF[16];
	//Get MVP Matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//Optix_Camare.Look();
	//g_currentCamera.Look();
	m_refCamera.Look();
	//cout<<"Last Camera:"<<endl;
	//g_refCamera.CoutCamera();
	//cout<<"now Camera:"<<endl;
	//g_currentCamera.CoutCamera();



	glGetFloatv(GL_MODELVIEW_MATRIX,modelViewF);

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	//glMultMatrixf(ViewMat);

	glGetFloatv(GL_PROJECTION_MATRIX,projF);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	//nv::matrix4f ttt;
	nv::matrix4f viewM;
	nv::matrix4f projectM;

	//nv::matrix4f TMVPMat;


	viewM.set_value(modelViewF);
	projectM.set_value(projF);

	nv::matrix4f TMVPMat = projectM * viewM;

	const float *MVPMatf = TMVPMat.get_value();


	float3 CameraPos1 = make_float3(m_refCamera.Position().x,m_refCamera.Position().y,m_refCamera.Position().z);

	//float3 CameraPos2 = make_float3(-2.403542,34.498703,37.004547);
	float3 CameraPos2 = make_float3(m_curretnCamera.Position().x,m_curretnCamera.Position().y,m_curretnCamera.Position().z);





	TransConstData(m_refCamera.getMvpMat(),m_refCamera.getModelViewMat(),&CameraPos1,&CameraPos2);




	//MapResource 
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_WorldPos,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_WorldNormal,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_Reflect,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_VectorPBO,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cuda_LastVecPBO,0));
	m_vectorCudaArray.map();
	//lastCudaArray.map();

	
	m_poxCudaTex.map();
	m_normalCudaTex.map();
	m_reflectCudaTex.map();
	//TransGLRes2Cuda(&cudaRes_VectorPBO,&cudaRes_VectorPBO,&cudaRes_VectorPBO,&cudaRes_VectorPBO,&cuda_LastVecPBO);

	cudaPredict(rasterWidth,rasterHeight);
	
	m_reflectCudaTex.unmap();
	m_poxCudaTex.unmap();
	m_normalCudaTex.unmap();
	m_vectorCudaArray.unMap();

}
GLuint CudaResource::getPredictTex()
{
	m_vectorCudaArray.generateTex();
	return m_vectorCudaArray.getTexture();
}
void CudaResource::reductionGPU()
{
	int pixelNum = thrustReduction(rasterWidth,rasterHeight);
}

void CudaResource::tracingArtifact()
{
		m_finalEffectCudaTex.map();
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaTest_Pixels,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_VectorPBO,0));
	m_finalEffectCudaArray.map();
	m_vectorCudaArray.map();
	//TransProPixel2Cuda(&cudaRes_ProPixels,&cudaTest_Pixels,&cudaRes_VectorPBO);




	reductionGPU();
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_VectorPBO, 0));

	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaTest_Pixels, 0));
	///checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_ProPixels, 0));
	m_vectorCudaArray.unMap();
	m_finalEffectCudaTex.unmap();
	m_finalEffectCudaArray.unMap();
	
}