//#include <helper_math.h>





#include<Windows.h>
#include <driver_types.h>
#include <cuda_runtime.h>
//#include <cutil_inline.h>
#include <helper_cuda.h>

#include "nvMath.h"
#include <cuda_gl_interop.h>
//#include <cutil_gl_inline.h>
#include <helper_cuda_gl.h>
//#include <cutil_math.h>
//#include <helper_math.h>
#include <curand_kernel.h>
//#include <GL/gl.h>

#ifndef CUDA_H
#define  CUDA_H

enum cudaTexType
{
	worldPosRef_t,
	worldNormalRef_t,
	reflecionRef_t,
	finalEffect_t,
	diffRef_t,

};


class CudaKernel
{
	public:
		CudaKernel(int w,int h):m_height(h),m_width(w)
		{
		}
		void init();
		void run();
private:
	    int m_height,m_width;


};
class CudaTexResourse
{
public: 
	
	inline void set(int texId,int w, int h,cudaTexType t)
	{
		m_texture = texId;
		m_width = w;
		m_height = h;
		m_type = t;
	}
	void init();
	void map();
	void unmap();
	void setEveryTex(int );
	void set(int);
	inline cudaTexType getType()
	{
		return m_type;
	}
	inline cudaGraphicsResource ** getResPoint()
	{
		return &m_CudaReourse;
	}
private:
	GLuint m_texture;
	GLuint m_tempTex;
	int m_width,m_height;
	cudaGraphicsResource * m_CudaReourse;
	cudaTexType m_type;
};
enum cudaPboType
{
	float4_t,
	float2_t,
	test_t,
	diff_normal_t,
};
class CudaPboResource
{
public:
	inline void set(int w,int h,cudaPboType t)
	{
		m_width = w;
		m_height = h;
		m_type = t;
	}
	inline cudaPboType getType()
	{
		return m_type;
	}
	void init();
	void map();
	void unMap();
	inline GLuint getTexture()
	{
		return m_texture;
	}
	inline cudaGraphicsResource ** getResPoint()
	{
		return & m_CudaReourse;
	}
	void generateTex();
private:
	cudaPboType m_type;
	void initTex();
	void initPbo();
	GLuint m_pbo,m_texture;
	int m_width,m_height;
	cudaGraphicsResource * m_CudaReourse;
};



extern "C" void TransProPixel2Cuda(cudaGraphicsResource **cuda_tex,cudaGraphicsResource **,cudaGraphicsResource **);


extern "C" void TransGLRes2Cuda(cudaGraphicsResource **cuda_tex1,cudaGraphicsResource **cuda_tex2,cudaGraphicsResource **cuda_tex3,cudaGraphicsResource **cuda_pbo,cudaGraphicsResource **cuda_pbo2);

extern "C" void RunKernel(dim3,dim3,int,int);

extern "C" void RunFirstPass(dim3,dim3,int,int);
extern "C" void MappingContantPeremeter(int *w,int* h);
extern "C" void TransConstData(const float *MvpMat,const float *ViewMat,float3 *CameraPos1,float3 *CameraPos2);
extern "C" void cudaSetImageFrame(nv::vec2f bbmin,nv::vec2f bbmax);
extern "C" void InitCudpp();
extern "C" void RunCudpp(unsigned int* ,unsigned int *);

extern "C" void RunThridPass(dim3,dim3);
extern "C" int thrustReduction(int width,int height);
extern "C" void cudaPredict(int width,int height);
extern "C" void InitThrust();

extern "C" void cudaRelateTex(CudaTexResourse * pResouce);
extern "C" void cudaRelateArray(CudaPboResource * pResouce);
#endif
