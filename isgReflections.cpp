﻿
/*
* Copyright (c) 2010 NVIDIA Corporation.  All rights reserved.
*
* NVIDIA Corporation and its licensors retain all intellectual property and proprietary
* rights in and to this software, related documentation and any modifications thereto.
* Any use, reproduction, disclosure or distribution of this software and related
* documentation without an express license agreement from NVIDIA Corporation is strictly
* prohibited.
*
* TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED *AS IS*
* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY
* SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
* BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR
* INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGES
*/

#include "main.h"
#include "Camera.h"
#include "3DS.H"
#include "Geometry.h"
#include "fpsCount.h"
#include "transform.h"
//#include"CFbo.h"
#include "Fbo.h"
//#include<glm/vec4.hpp>
//#include<glm/vec3.hpp>
#include<nvMath.h>
//nv::GlutExamine manipulator;
#include"cuda.h"
#include"glslShaderLoader.h"
#include "TimeMesure.h"
#include "transShader.h"
#include "ReprojectShader.h"
#include "mergeShader.h"
#include "texShader.h"
#include "testShader.h"
#include "blendShader.h"
#include "macro.h"
#include "RefFrame.h"
#include "cudaResource.h"
#include "reflectionShader.h"
#include "gBufferShader.h"
#include "reflectionShader.h"
#include "ImgMesh.h"
#include "screenBuffer.h"
#include "fboAnalyse.h"
#include "hybridRendering.h"
#include "ImageWarpingShader.h"
#include "compareShader.h"
#include "diffNormal.h"
#include "idShader.h"
#include  "keyFrame.h"
#include "showShader.h"
#include "TransShaderNoPipe.h"
#include "ImageWarpingShaderNopipe.h"
#include "blendShaderNopipe.h"
#include "Edge.h"
#include "optixToiletRecorder.h"
#include "tcToiletRecorder.h"
#include "tcToiletRatio.h"
#include "optixLivingRoom.h"
#include "tcLivingRoom.h"
#include "tcLivingRatio.h"
//#include "toiletScene.h"

unsigned int *Host_PixelSum;
unsigned int *Host_PixelState;

unsigned int *Dev_PixelPos;

int PixelsSum;

double AllTime = 0;

bool isSavePic = false;

const int ReflectNum = 4;


float glossiness = 0.0f;

GLuint reflectionMaps[ReflectNum];
GLuint reflectionMapTex_Now;

GLuint Merge_FrameCount;

//GLuint goldTex;
GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT,GL_COLOR_ATTACHMENT2_EXT,GL_COLOR_ATTACHMENT3_EXT,GL_COLOR_ATTACHMENT4_EXT,GL_COLOR_ATTACHMENT5_EXT,GL_COLOR_ATTACHMENT6_EXT};
optix::GeometryGroup geometrygroup;

//uint *Host_PixelSum;
//MyGeometry teapot;
int CountTime = 1000;

nv::vec3f viewDependentMissColor = nv::vec3f(255,0,0);
nv::vec3f viewIndepentdentMissColor = nv::vec3f(0,255,0);
//cudaGraphicsResource *cudaRes_WorldNormal,*cudaRes_WorldPos,*cudaRes_Reflect;
TestShader g_testShader;

CudaTexResourse poxCudaTex,normalCudaTex,reflectCudaTex,finalEffectCudaTex;
#ifdef DIFFNORMAL
CudaTexResourse diffCudaTex;
#endif
CudaPboResource vectorCudaArray,finalEffectCudaArray;

ReprojectShader g_reprojectShader;
TranShader g_transShader;
MergeShader g_mergeShader;
TexShader g_texShader;
GbufferShader g_gBufferShader;
reflectShader g_reflectionShader;
BlendShader g_blendShader;
HybridShader g_hybridShader;
ImgWarpingShader g_imgWarpShader;
CompareShader g_compareShader;
DiffNormalShader g_diffNormalShader;
showShader g_showShader;
IDShader g_idShader;
TranShaderNoPipe g_transShaderNoPipe;
ImgWarpingShaderNoPipe g_imageWarpNopipeShader;
BlendShaderNoPipe g_blendShaderNoPipe;
EdgeShader g_edgeShader;

KeyFrame g_keyFrame;

TimeMesure g_timeMesure(tcRenderingType 
						,CountTime);


//#include "toiletScene.h"
//toiletScene g_scene;
FileRecorder g_recorder = optixToiletRecorder(900) ;
//FileRecorder g_recorder = tcToiletRecorder(900) ;
//FileRecorder g_recorder = tcToiletRatioRecorder(900) ;

#include "livingRoom.h"
livingRoom g_scene;

//#include "livingRoom.h"
//livingRoom g_scene;
//FileRecorder g_recorder = optixLivingRoomRecorder(900) ;
//FileRecorder g_recorder = tcLivingRecorder(900) ;
//FileRecorder g_recorder = tcLivingRatioRecorder(900) ;
const int rayTraceDepth = 1;

int BEGININDEX = 0 ;   //must be 10 time
int ENDINDEX = 20;
int TIMERANGE= (ENDINDEX-BEGININDEX);


int currentTime  = 0;
int currentTime2 = 0;



FPS fcount(CountTime);


GLuint             normalTex;
GLuint             worldSpaceTex;
FramebufferObject* worldSpaceNormalFBO;

unsigned int warmup_frames = 10u, timed_frames = 10u;


int logReflectionSamplingRate = 0;

GLuint reflectionMapPBO;
GLuint addtionalMapPBO;

#ifdef NOGEOMETRY
GLuint colorPBO;
GLuint posPBO;
GLuint normalPBO;
optix::Buffer         rtPosBuffer;
optix::Buffer         rtNormalBuffer;
optix::Buffer         rtColorBuffer;
#endif
GLuint VecorTexture;


bool fixedCamera = false;
nv::matrix4f fixedCameraMatrix;
bool drawFps = true;
bool stat_breakdown = true;

// CG
CGcontext cgContext;

// OptiX
unsigned int traceWidth, traceHeight;

optix::Context        rtContext;
optix::Buffer         PixelBuffer;
optix::Buffer         rtReflectionBuffer;

optix::Buffer         rtAdditionalBuffer;
//Buffer

optix::TextureSampler rtNormalTexture;
optix::TextureSampler rtLastReflectTexture;
optix::TextureSampler rtWorldSpaceTexture;


Fbo currentGbuffer(3,rasterWidth,rasterHeight);
Fbo TransMapFbo(1,rasterWidth,rasterWidth);
Fbo FinalEffectFbo(1,rasterWidth,rasterWidth);
Fbo FinalEffectFbo2(1,rasterWidth,rasterWidth);
Fbo MergeEffectFbo(1,rasterWidth,rasterWidth);

#if defined(DIFFNORMAL )
	Fbo refGbuffer(4,rasterWidth,rasterWidth);
#else 
	Fbo refGbuffer(3,rasterWidth,rasterWidth);
#endif

ImgMesh g_imgMesh(rasterWidth,rasterWidth);
ScreenBuffer screenBuffer(windowWidth,windowHeight);
#ifdef ANALYSIS
FboAnalyse g_fboAnalyse(rasterWidth,rasterWidth);
#endif
//New TEXTURE
Fbo optixFrame(1,rasterWidth,rasterWidth);
Fbo tcFrame(1,rasterWidth,rasterWidth);


bool animate = true;
bool g_Recordpic = false;
float scene_epsilon = 1e-2f;

void init_cuda(int argc,char**argv)
{

	findCudaDevice(argc,(const char**) argv);
    //绑定opengl纹理到cuda
	poxCudaTex.set(refGbuffer.getTexture(0),rasterWidth,rasterHeight,worldPosRef_t);
	normalCudaTex.set(refGbuffer.getTexture(1),rasterWidth,rasterHeight,worldNormalRef_t);
	reflectCudaTex.set(reflectionMapTex_Now,rasterWidth,rasterHeight,reflecionRef_t);
#ifdef DIFFNORMAL
	diffCudaTex.set(refGbuffer.getTexture(3),rasterWidth,rasterHeight,diffRef_t);
#endif
	
	poxCudaTex.init();
	normalCudaTex.init();
	reflectCudaTex.init();
#ifdef DIFFNORMAL
	diffCudaTex.init();
#endif
	//绑定opengl PBO到cuda
	
	vectorCudaArray.set(rasterWidth,rasterHeight,float4_t);
	vectorCudaArray.init();
	


	finalEffectCudaTex.set(FinalEffectFbo.getTexture(0),rasterWidth,rasterHeight,finalEffect_t);
	finalEffectCudaTex.init();

	finalEffectCudaArray.set(rasterWidth,rasterHeight,test_t);
	finalEffectCudaArray.init();
	InitThrust();
	int w = rasterWidth;
	int h = rasterHeight;
	MappingContantPeremeter(&w,&h);
	/*Host_PixelSum = NULL;
	Host_PixelState = NULL;
	Host_PixelSum =(unsigned int*) malloc(sizeof(unsigned int)* rasterHeight * rasterWidth);
	Host_PixelState =(unsigned int*) malloc(sizeof(unsigned int)* rasterHeight * rasterWidth);
	if(Host_PixelSum == NULL ||Host_PixelState == NULL){
	cout<<"malloc failed"<<endl;
	return;
	}*/

}

#define EDGEDELTA 0.0




void init_gl()
{
	
#ifndef __APPLE__
	glewInit();

	if (!glewIsSupported( "GL_VERSION_2_0 "
		"GL_EXT_framebuffer_object "))
	{
		printf("Unable to load the necessary extensions\n");
		printf("This sample requires:\n"
			"OpenGL 2.0\n"
			"GL_EXT_framebuffer_object\n"
			"Exiting...\n");
		exit(-1);
	}
#if defined(_WIN32)
	// Turn off vertical sync
	wglSwapIntervalEXT(0);
#endif
#endif


	glClearColor(.2f,.2f,.2f,1.f);
	glEnable(GL_DEPTH_TEST);

	//init_cg();

	// create PBO's
	glGenBuffers(1, &reflectionMapPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, rasterHeight*rasterWidth*sizeof(float4), 0, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glGenBuffers(1, &addtionalMapPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, addtionalMapPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, rasterHeight*rasterWidth*sizeof(float4), 0, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
#ifdef NOGEOMETRY
	glGenBuffers(1, &colorPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, colorPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, rasterHeight*rasterWidth*sizeof(float4), 0, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glGenBuffers(1, &posPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, posPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, rasterHeight*rasterWidth*sizeof(float4), 0, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glGenBuffers(1, &normalPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, normalPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, rasterHeight*rasterWidth*sizeof(float4), 0, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
#endif

	

	// setup FBO's

	currentGbuffer.init();
	FinalEffectFbo.init();
	//TransMapFbo.init();
	TransMapFbo.init(GL_LINEAR);
	refGbuffer.init();

	MergeEffectFbo.init();
	if(hybridRenderingType == g_timeMesure.getType())
	{
		FinalEffectFbo2.init();
		g_hybridShader.init();

	}
	//生成vector纹理
	glGenTextures(1, &VecorTexture);

	glBindTexture(GL_TEXTURE_2D, VecorTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, rasterWidth, rasterHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);



	glGenTextures(1, &reflectionMapTex_Now);
	glBindTexture(GL_TEXTURE_2D, reflectionMapTex_Now);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, rasterWidth, rasterHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);



	//生成NEW纹理
	/*
	glGenTextures(ReflectNum, reflectionMaps);
	for(int i = 0;i<ReflectNum;i++){
		glBindTexture(GL_TEXTURE_2D, reflectionMaps[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, rasterWidth, rasterHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	*/



	// cgGLSetTextureParameter(cgCudaTransTexParam, VecorTexture);


	//载入glslshader
	g_transShader.init();
	g_transShader.setClearColor(viewDependentMissColor);
	g_reprojectShader.init();
	g_mergeShader.init();
	g_reprojectShader.setClearColor(viewIndepentdentMissColor);
	g_texShader.init();
	g_blendShader.init();
	g_gBufferShader.init();
	g_reflectionShader.init();
	g_reflectionShader.setWindowSize(rasterWidth,rasterHeight);
	g_imgWarpShader.init();
	g_imgWarpShader.setRes(rasterWidth,rasterHeight);
	g_imgWarpShader.setClearColor(viewIndepentdentMissColor);

	g_compareShader.init();
	g_diffNormalShader.init();
	g_diffNormalShader.setRes(nv::vec2f(rasterWidth,rasterWidth));
	g_edgeShader.init();
	g_edgeShader.setRes(nv::vec2f(rasterWidth,rasterWidth));
	g_idShader.init();
	g_showShader.init();
	g_transShaderNoPipe.init();
	g_transShaderNoPipe.setClearColor(viewDependentMissColor);
	g_imageWarpNopipeShader.init();
	g_imageWarpNopipeShader.setRes(rasterWidth,rasterHeight);
	g_imageWarpNopipeShader.setClearColor(viewIndepentdentMissColor);
	g_blendShaderNoPipe.init();
	g_blendShaderNoPipe.setViewDependentColor(viewDependentMissColor);
	g_blendShaderNoPipe.setViewIndependColor(viewIndepentdentMissColor);
	//载入imgMesh
	g_imgMesh.init();

    screenBuffer.setBuffersize(rasterWidth,rasterHeight);
    screenBuffer.init();
	if(compareType == g_timeMesure.getType())
	{
		optixFrame.init();
         tcFrame.init();
	}
#ifdef ANALYSIS
	g_fboAnalyse.init();
#endif
	g_recorder.init();
	//g_fboAnalyse.init();
	//CHECK_ERRORS();
	//glEnable(GL_CULL_FACE);
}

void init_scene()
{
	printf("init scene\n");
	g_scene.init();
	g_scene.setTimeMesure(&g_timeMesure);
	//g_scene.cameraControl(currentTime2,g_scene.m_curCamera);
}

std::string ptxpath( const std::string& base )
{
	return std::string(sutilSamplesPtxDir()) + "/isgReflections_generated_" + base + ".ptx";
}
extern unsigned int  *g_PixelPos;
void init_optix()
{
	try 
	{
		//  printf("111");
		rtContext = optix::Context::create();
		
#ifdef NOGEOMETRY
		rtContext->setRayTypeCount(4);
		rtContext->setEntryPointCount(4);
#else
		rtContext->setRayTypeCount(2);
		rtContext->setEntryPointCount(2);
#endif

		rtContext->setPrintEnabled( true );
		rtContext->setPrintBufferSize( 4096 *8);

		rtContext["radiance_ray_type"]->setUint(0u);
		rtContext["shadow_ray_type"]->setUint(1u);
		rtContext["scene_epsilon"]->setFloat(scene_epsilon);
#if MerGe

		// PixelBuffer = rtContext->createBufferForCUDA(RT_BUFFER_INPUT,RT_FORMAT_UNSIGNED_INT,1024*1024);//PixelsSum);

		// rtContext["Pixels_Buffer"]->setBuffer(PixelBuffer);
#endif


		//

		PixelBuffer = rtContext->createBufferForCUDA(RT_BUFFER_INPUT,RT_FORMAT_UNSIGNED_INT,rasterWidth*rasterHeight);//PixelsSum);
		PixelBuffer->setDevicePointer(0, (CUdeviceptr)g_PixelPos);

		rtContext["Pixels_Buffer"]->setBuffer(PixelBuffer);

		rtWorldSpaceTexture = rtContext->createTextureSamplerFromGLImage(currentGbuffer.getTexture(0), RT_TARGET_GL_TEXTURE_2D);
		rtWorldSpaceTexture->setWrapMode(0, RT_WRAP_CLAMP_TO_EDGE);
		rtWorldSpaceTexture->setWrapMode(1, RT_WRAP_CLAMP_TO_EDGE);
		rtWorldSpaceTexture->setWrapMode(2, RT_WRAP_CLAMP_TO_EDGE);
		rtWorldSpaceTexture->setIndexingMode(RT_TEXTURE_INDEX_ARRAY_INDEX);
		rtWorldSpaceTexture->setReadMode(RT_TEXTURE_READ_ELEMENT_TYPE);
		rtWorldSpaceTexture->setMaxAnisotropy(1.0f);
		rtWorldSpaceTexture->setFilteringModes(RT_FILTER_NEAREST, RT_FILTER_NEAREST, RT_FILTER_NONE);
		
		rtContext["request_texture"]->setTextureSampler(rtWorldSpaceTexture);

		rtLastReflectTexture = rtContext->createTextureSamplerFromGLImage(FinalEffectFbo.getTexture(0), RT_TARGET_GL_TEXTURE_2D);
		rtLastReflectTexture->setWrapMode(0, RT_WRAP_CLAMP_TO_EDGE);
		rtLastReflectTexture->setWrapMode(1, RT_WRAP_CLAMP_TO_EDGE);
		rtLastReflectTexture->setWrapMode(2, RT_WRAP_CLAMP_TO_EDGE);
		rtLastReflectTexture->setIndexingMode(RT_TEXTURE_INDEX_ARRAY_INDEX);
		rtLastReflectTexture->setReadMode(RT_TEXTURE_READ_ELEMENT_TYPE);
		rtLastReflectTexture->setMaxAnisotropy(1.0f);
		rtLastReflectTexture->setFilteringModes(RT_FILTER_NEAREST, RT_FILTER_NEAREST, RT_FILTER_NONE);
		rtContext["Last_reflection"]->setTextureSampler(rtLastReflectTexture);


		rtNormalTexture = rtContext->createTextureSamplerFromGLImage(currentGbuffer.getTexture(1), RT_TARGET_GL_TEXTURE_2D);
		rtNormalTexture->setWrapMode(0, RT_WRAP_CLAMP_TO_EDGE);
		rtNormalTexture->setWrapMode(1, RT_WRAP_CLAMP_TO_EDGE);
		rtNormalTexture->setWrapMode(2, RT_WRAP_CLAMP_TO_EDGE);
		rtNormalTexture->setIndexingMode(RT_TEXTURE_INDEX_ARRAY_INDEX);
		rtNormalTexture->setReadMode(RT_TEXTURE_READ_ELEMENT_TYPE);
		rtNormalTexture->setMaxAnisotropy(1.0f);
		rtNormalTexture->setFilteringModes(RT_FILTER_NEAREST, RT_FILTER_NEAREST, RT_FILTER_NONE);
		rtContext["normal_texture"]->setTextureSampler(rtNormalTexture);




		rtReflectionBuffer = rtContext->createBufferFromGLBO(RT_BUFFER_OUTPUT, reflectionMapPBO);
		rtReflectionBuffer->setSize(rasterWidth,rasterHeight);
		rtReflectionBuffer->setFormat(RT_FORMAT_FLOAT4);
	
		rtContext["reflection_buffer"]->setBuffer(rtReflectionBuffer);


		rtAdditionalBuffer = rtContext->createBufferFromGLBO(RT_BUFFER_OUTPUT, addtionalMapPBO);
		rtAdditionalBuffer->setSize(rasterWidth,rasterHeight);
		rtAdditionalBuffer->setFormat(RT_FORMAT_FLOAT4);
		rtContext["addition_buffer"]->setBuffer(rtAdditionalBuffer);

#ifdef NOGEOMETRY

		rtPosBuffer = rtContext->createBufferFromGLBO(RT_BUFFER_OUTPUT, posPBO);
		rtPosBuffer->setSize(rasterWidth,rasterHeight);
		rtPosBuffer->setFormat(RT_FORMAT_FLOAT4);
		rtContext["posBuffer"]->setBuffer(rtPosBuffer);

		rtNormalBuffer = rtContext->createBufferFromGLBO(RT_BUFFER_OUTPUT, normalPBO);
		rtNormalBuffer->setSize(rasterWidth,rasterHeight);
		rtNormalBuffer->setFormat(RT_FORMAT_FLOAT4);
		rtContext["normalBuffer"]->setBuffer(rtNormalBuffer);

		rtColorBuffer = rtContext->createBufferFromGLBO(RT_BUFFER_OUTPUT, colorPBO);
		rtColorBuffer->setSize(rasterWidth,rasterHeight);
		rtColorBuffer->setFormat(RT_FORMAT_FLOAT4);
		rtContext["colorBuffer"]->setBuffer(rtColorBuffer);

#endif
    	rtContext->setRayGenerationProgram( 0, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_request" ) );
		rtContext->setExceptionProgram( 0, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_exception" ) );
		rtContext->setMissProgram( 0, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_miss" ) );

		rtContext->setRayGenerationProgram(1, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "addition_request" ) );
		rtContext->setExceptionProgram( 1, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_exception" ) );
		rtContext->setMissProgram( 1, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_miss" ) );
#ifdef NOGEOMETRY
		rtContext->setRayGenerationProgram(2, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "gBufferAndRequest" ) );
		rtContext->setExceptionProgram( 2, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_exception" ) );
		rtContext->setMissProgram( 2, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_miss" ) );


		rtContext->setRayGenerationProgram(3, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "gBufferAndRequest_addition") );
		rtContext->setExceptionProgram( 3, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_exception" ) );
		rtContext->setMissProgram( 3, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_miss" ) );
#endif
		// printf("222");
		MyGeometry::ms_rtContext = rtContext;
		MyMeterial::ms_rtContext = rtContext;
		//optix::Geometry sofaGemetry = sofa.getOptixGeometry(rtContext);
		g_scene.setOptix(&rtContext);
		g_scene.optixInit();
		rtContext["max_depth"]->setUint(rayTraceDepth);
		rtContext->setStackSize(1850);
		rtContext["rasterSize"]->setInt(rasterWidth, rasterHeight);
		rtContext->validate();
		

	} 
	catch ( optix::Exception& e )
	{
		sutilReportError( e.getErrorString().c_str() );
		exit(-1);
	}

	cout<<"ok"<<endl;
}



void drawFinalEffect()
{
	glClearColor(viewIndepentdentMissColor.x,viewIndepentdentMissColor.y,viewIndepentdentMissColor.z,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	g_reprojectShader.setParemeter(TransMapFbo.getTexture(0),currentGbuffer.getTexture(0),refGbuffer.getTexture(0), g_scene.m_refCamera.getMvpMat());
	//g_reprojectShader.setParemeter(currentGbuffer.getTexture(2),currentGbuffer.getTexture(0),refGbuffer.getTexture(0), g_scene.m_refCamera.getMvpMat());
	g_reprojectShader.setNormalTex(refGbuffer.getTexture(1));
	MyGeometry::drawQuad(g_reprojectShader);
}

/*
void New_drawscene(CGtechnique& current_technique){

	//glLoadIdentity();
	if(fixedCamera) {
		glMultMatrixf(fixedCameraMatrix.get_value());
	} else {
		// manipulator.applyTransform();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		g_scene.m_curCamera.Look();
	}


	cgGLSetStateMatrixParameter(
		cgWorldViewProjParam,
		CG_GL_MODELVIEW_PROJECTION_MATRIX,
		CG_GL_MATRIX_IDENTITY );
	cgGLSetStateMatrixParameter(
		cgWorldViewInvParam,
		CG_GL_MODELVIEW_MATRIX,
		CG_GL_MATRIX_INVERSE_TRANSPOSE );
	cgGLSetStateMatrixParameter(
		cgWorldViewParam,
		CG_GL_MODELVIEW_MATRIX,
		CG_GL_MATRIX_IDENTITY );


	cgSetParameter1i(cgIsWallParam, 0);
	cgSetParameter1i(cgIsModelParam, 1);
	cgSetParameter3f(cgDiffuseColor,0.0f,0.0f,0.0f);
	cgSetParameter3f(cgCameraPosition,g_scene.m_curCamera.Position().x,g_scene.m_curCamera.Position().y,g_scene.m_curCamera.Position().z);
	cgGLSetTextureParameter(cgDiffuseTexParam, woodTex);
	draw_model(current_technique);

	//  cgSetParameter1i(cgIsModelParam, 0);
	//  cgGLSetTextureParameter(cgDiffuseTexParam, groundTex);
	// draw_ground(current_technique);

	// cgSetParameter1i(cgIsWallParam, 0);
	// cgGLSetTextureParameter(cgDiffuseTexParam, woodTex);
	// draw_walls(current_technique);
	/*  glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	GLUquadricObj *pObj = gluNewQuadric();			
	gluQuadricDrawStyle(pObj, GLU_FILL);			

	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(lightPos.x,
	lightPos.y,
	lightPos.z );
	gluSphere(pObj, 5.0f, 25, 25);

	gluDeleteQuadric(pObj);	

	glPopMatrix();
	*/
	/*CHECK_ERRORS();


}


void draw_scene(CGtechnique& current_technique)
{
	// glLoadIdentity();
	if(fixedCamera) {
		glMultMatrixf(fixedCameraMatrix.get_value());
	} else {
		// manipulator.applyTransform();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		g_scene.m_refCamera.Look();
	}

	cgGLSetStateMatrixParameter(
		cgWorldViewProjParam,
		CG_GL_MODELVIEW_PROJECTION_MATRIX,
		CG_GL_MATRIX_IDENTITY );
	cgGLSetStateMatrixParameter(
		cgWorldViewInvParam,
		CG_GL_MODELVIEW_MATRIX,
		CG_GL_MATRIX_INVERSE_TRANSPOSE );
	cgGLSetStateMatrixParameter(
		cgWorldViewParam,
		CG_GL_MODELVIEW_MATRIX,
		CG_GL_MATRIX_IDENTITY );


	cgSetParameter1i(cgIsWallParam, 0);
	cgSetParameter1i(cgIsModelParam, 1);
	cgSetParameter3f(cgDiffuseColor,0.0f,0.0f,0.0f);
	cgSetParameter3f(cgCameraPosition,g_scene.m_refCamera.Position().x,g_scene.m_refCamera.Position().y,g_scene.m_refCamera.Position().z);
	cgGLSetTextureParameter(cgDiffuseTexParam, woodTex);
	draw_model(current_technique);

	//  cgSetParameter1i(cgIsModelParam, 0);
	//  cgGLSetTextureParameter(cgDiffuseTexParam, groundTex);
	// draw_ground(current_technique);

	// cgSetParameter1i(cgIsWallParam, 0);
	// cgGLSetTextureParameter(cgDiffuseTexParam, woodTex);
	// draw_walls(current_technique);
	/*  glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	GLUquadricObj *pObj = gluNewQuadric();			
	gluQuadricDrawStyle(pObj, GLU_FILL);			

	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(lightPos.x,
	lightPos.y,
	lightPos.z );
	gluSphere(pObj, 5.0f, 25, 25);

	gluDeleteQuadric(pObj);	

	glPopMatrix();
	*/
	/*CHECK_ERRORS();
}*/
void draw_scene(glslShader& shader,CCamera * pCamera)
{
	if(fixedCamera) 
	{
		glMultMatrixf(fixedCameraMatrix.get_value());
	} else {
		// manipulator.applyTransform();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		pCamera->Look();
	}
	shader.setCamera(pCamera);
	g_scene.draw_model(shader);


	CHECK_ERRORS();
}
extern unsigned int  *g_PixelPos;
void additionalTracingNopipe(int pixelNum)
{

	float temp = sqrt((float) pixelNum);
	int length = ceil(temp);

	PixelBuffer->setDevicePointer(0, (CUdeviceptr)g_PixelPos);
	CCamera& curCamera = g_scene.m_curCamera;
	rtContext["PixelNum"]->setInt(pixelNum);
	rtContext["PixelWidth"]->setInt(length);
	rtContext["eye_pos"]->setFloat(curCamera.Position().x, curCamera.Position().y, curCamera.Position().z);
	rtContext["bbmin"]->setFloat(curCamera.getImageMin().x, curCamera.getImageMin().y);
	rtContext["bbmax"]->setFloat(curCamera.getImageMax().x, curCamera.getImageMax().y);
	rtContext["optixModelView_Inv"]->setMatrix4x4fv(false,curCamera.getInvMvp());
	nv::matrix4f matInv = inverse(nv::matrix4f(curCamera.getModelViewMat()));
	rtContext["optixModelView_Inv"]->setMatrix4x4fv(false,matInv.get_value());
	
	try
	{
		rtContext->launch(3, pixelNum);
	}
	catch (optix::Exception& e) 
	{
		sutilReportError( e.getErrorString().c_str() );
		exit(1);
	}


	
	glPushAttrib(GL_PIXEL_MODE_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
	glBindTexture(GL_TEXTURE_2D,reflectionMapTex_Now);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,
		GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glPopAttrib();

}
void addtionalTracing(int pixelNum)
{
	/*nv::vec4f* pixelBuffer = (nv::vec4f*)rtReflectionBuffer->map();
			RTsize numVertices;
			rtReflectionBuffer->getSize( numVertices );
			for(int i=0;i<numVertices*numVertices;i++)
			{
				pixelBuffer[i] = nv::vec4f(1,1,1,1);
			}
	rtReflectionBuffer->unmap();

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER, rasterHeight*rasterWidth*sizeof(float4), 0, GL_STREAM_DRAW_ARB);

	pixelBuffer = (nv::vec4f*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER,
												GL_WRITE_ONLY_ARB);
	if(pixelBuffer	)
	{
		// update data directly on the mapped buffer
		for(int i=0;i<numVertices*numVertices;i++)
			{
				pixelBuffer[i] = nv::vec4f(1,1,1,1);
			}
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER); // release the mapped buffer
	}
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	*/
	float temp = sqrt((float) pixelNum);
	int length = ceil(temp);

	PixelBuffer->setDevicePointer(0, (CUdeviceptr)g_PixelPos);
	rtContext["PixelNum"]->setInt(pixelNum);
	rtContext["PixelWidth"]->setInt(length);
	rtContext["eye_pos"]->setFloat(g_scene.m_curCamera.Position().x, g_scene.m_curCamera.Position().y, g_scene.m_curCamera.Position().z);
	rtContext["lightPos"]->setFloat(g_scene.getLightPos().x, g_scene.getLightPos().y, g_scene.getLightPos().z);

	/*
	g_scene.cameraControl(currentTime2,g_scene.m_curCamera);

	currentGbuffer.begin();
	//draw_scene(cgTechniqueWorldPosNormal,&g_scene.m_curCamera);
	g_scene.draw_model(g_gBufferShader,&g_scene.m_curCamera);
	currentGbuffer.end();*/
	//currentGbuffer.SaveBMP("worPos.
	try
	{
		rtContext->launch(1, pixelNum);
	}
	catch (optix::Exception& e) 
	{
		sutilReportError( e.getErrorString().c_str() );
		exit(1);
	}


	
	glPushAttrib(GL_PIXEL_MODE_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
	glBindTexture(GL_TEXTURE_2D,reflectionMapTex_Now);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,
		GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glPopAttrib();

	/*
	glEnable(GL_TEXTURE_2D);
	BYTE *pTexture = NULL;
	pTexture = new BYTE[rasterWidth*  rasterHeight * 3];
	memset(pTexture, 0, rasterWidth* rasterHeight * 3 * sizeof(BYTE));

	glBindTexture(GL_TEXTURE_2D, reflectionMapTex_Now);//TexPosId   PboTex
	 
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	int w = rasterWidth;
	int h = rasterHeight;
	Fbo::SaveBMP("./test/newtrace.bmp", pTexture, w, h);
	if (pTexture)
	delete[] pTexture;
	glBindTexture(GL_TEXTURE_2D, 0);
	*/

}
void blending()
{
	MergeEffectFbo.begin();
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	g_blendShader.setParemeter(FinalEffectFbo.getTexture(0),reflectionMapTex_Now);
	MyGeometry::drawQuad(g_blendShader);
	//MergeEffectFbo.SaveBMP("./test/realBlending.bmp",0);
	MergeEffectFbo.end();
	

}
int reductionGPUNoPipe()
{
	int pixelNum = 	thrustReduction(rasterWidth,rasterHeight);
	additionalTracingNopipe(pixelNum);
	return pixelNum;
}
int reductionGPU()
{
	int pixelNum = 	thrustReduction(rasterWidth,rasterHeight);
	addtionalTracing(pixelNum);
	return pixelNum;
	//blending();
}


//Compute Vec in Cuda

void ComputeVecInCuda()
{

	


	float3 CameraPos1 = make_float3(g_scene.m_refCamera.Position().x,g_scene.m_refCamera.Position().y,g_scene.m_refCamera.Position().z);

	float3 CameraPos2 = make_float3(g_scene.m_curCamera.Position().x,g_scene.m_curCamera.Position().y,g_scene.m_curCamera.Position().z);





	TransConstData(g_scene.m_refCamera.getMvpMat(),g_scene.m_refCamera.getModelViewMat(),&CameraPos1,&CameraPos2);
	cudaSetImageFrame(g_scene.m_refCamera.getImageMin(),g_scene.m_refCamera.getImageMax());
	vectorCudaArray.map();
	
	poxCudaTex.map();
	normalCudaTex.map();
	reflectCudaTex.map();	
#ifdef DIFFNORMAL
	diffCudaTex.map();
#endif
	cudaPredict(rasterWidth,rasterHeight);
#ifdef DIFFNORMAL
	diffCudaTex.unmap();
#endif
	reflectCudaTex.unmap();
	poxCudaTex.unmap();
	normalCudaTex.unmap();
	vectorCudaArray.unMap();
	vectorCudaArray.generateTex();
	VecorTexture = vectorCudaArray.getTexture();

	//lastCudaArray.unMap();
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_WorldPos, 0));
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_WorldNormal, 0));
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_Reflect, 0));
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_VectorPBO, 0));
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cuda_LastVecPBO, 0));



};

int mapping()
{//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_ProPixels,0));
	finalEffectCudaTex.map();
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaTest_Pixels,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_VectorPBO,0));
	finalEffectCudaArray.map();
	vectorCudaArray.map();
	//TransProPixel2Cuda(&cudaRes_ProPixels,&cudaTest_Pixels,&cudaRes_VectorPBO);




	int pixelNum  = reductionGPU();
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_VectorPBO, 0));

	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaTest_Pixels, 0));
	///checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_ProPixels, 0));
	vectorCudaArray.unMap();
	finalEffectCudaTex.unmap();
	finalEffectCudaArray.unMap();
	return pixelNum;


}
int mappingNoPipe()
{//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_ProPixels,0));
	finalEffectCudaTex.map();
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaTest_Pixels,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_VectorPBO,0));
	finalEffectCudaArray.map();
	vectorCudaArray.map();
	//TransProPixel2Cuda(&cudaRes_ProPixels,&cudaTest_Pixels,&cudaRes_VectorPBO);




	int pixelNum  = reductionGPUNoPipe();
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_VectorPBO, 0));

	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaTest_Pixels, 0));
	///checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_ProPixels, 0));
	vectorCudaArray.unMap();
	finalEffectCudaTex.unmap();
	finalEffectCudaArray.unMap();
	return pixelNum;


}



void drawTransMap(int  optixId)
{
	glClearColor(viewDependentMissColor.x,viewDependentMissColor.y,viewDependentMissColor.z,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	RefFrame & frame = RefFrame::getFrameByIndex(optixId);
	g_transShader.setRes(rasterWidth,rasterHeight);
	g_transShader.setParemeter(frame.getOptixTex(),VecorTexture,frame.getGbuffer().getTexture(0),0);
	//g_transShader.setAdditionalTex(frame.getAdditionalTex());
	g_transShader.setRes(rasterWidth,rasterHeight);
	g_transShader.setEdgeTex(frame.getGbuffer().getTexture(2));
	
	g_imgMesh.drawImgMesh(g_transShader);

}
void drawTransMapNoPipe(int optixId)
{
	glClearColor(viewDependentMissColor.x,viewDependentMissColor.y,viewDependentMissColor.z,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	RefFrame & frame = RefFrame::getFrameByIndex(optixId);
	g_transShaderNoPipe.setRes(rasterWidth,rasterHeight);
	g_transShaderNoPipe.setParemeter(frame.getOptixTex(),VecorTexture,frame.getGbuffer().getTexture(0),0);
	//g_transShader.setAdditionalTex(frame.getAdditionalTex());
	g_transShaderNoPipe.setDiffuseTex(frame.getGbuffer().getTexture(2));
	g_transShaderNoPipe.setRes(rasterWidth,rasterHeight);
	g_transShaderNoPipe.setEdgeTex(frame.getGbuffer().getTexture(3));
	g_imgMesh.drawImgMesh(g_transShaderNoPipe);
}

void drawText( const std::string& text, float x, float y, void* font )
{
	// Save state
	glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST);

	glColor3f( .1f, .1f, .1f ); // drop shadow
	// Shift shadow one pixel to the lower right.
	glWindowPos2f(x + 1.0f, y - 1.0f);
	for( std::string::const_iterator it = text.begin(); it != text.end(); ++it )
		glutBitmapCharacter( font, *it );

	glColor3f( .95f, .95f, .95f );        // main text
	glWindowPos2f(x, y);
	for( std::string::const_iterator it = text.begin(); it != text.end(); ++it )
		glutBitmapCharacter( font, *it );

	// Restore state
	glPopAttrib();
}





void setTitle();
void optixRendering(Fbo renderFbo = Fbo())
{
	//glDisable(GL_CULL_FACE);
	setTitle();
	if (stat_breakdown) 
	{
		glFinish();
		double display_start = TimeMesure::getCurrentTime();
		// sutilCurrentTime(&display_start);
		g_timeMesure.setBeginTime(display_start);
	}
	CVector3& pos =g_scene.m_refCamera.Position();
	g_scene.cameraControl(currentTime2,g_scene.m_curCamera);

	g_scene.m_refCamera = g_scene.m_curCamera;
	/*
	int optixTime;
	{
		 optixTime  = g_keyFrame.getOptixFrame(currentTime2);


		g_scene.cameraControl(optixTime*JianGe+BEGININDEX,g_scene.m_refCamera);
	}*/
	
	currentGbuffer.begin();
	
	g_scene.draw_model(g_gBufferShader,&g_scene.m_refCamera);
	//currentGbuffer.SaveBMP("diffuse2.bmp",2);
	//currentGbuffer.debugPixel(0,593,669);
	currentGbuffer.end();

	rtContext["eye_pos"]->setFloat(g_scene.m_curCamera.Position().x, g_scene.m_curCamera.Position().y, g_scene.m_curCamera.Position().z);
	try
	{
		if (stat_breakdown) 
		{
			double optixBeginTime = TimeMesure::getCurrentTime();;
			//sutilCurrentTime(&optixBeginTime);
			g_timeMesure.setOptixBeginTime(optixBeginTime);
		}
		rtContext->launch(0, rasterWidth, rasterHeight);
	}
	catch (optix::Exception& e) 
	{
		sutilReportError( e.getErrorString().c_str() );
		exit(1);
	}

	glPushAttrib(GL_PIXEL_MODE_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
	glBindTexture(GL_TEXTURE_2D,reflectionMapTex_Now);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,
		GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glPopAttrib();
	/*
	glEnable(GL_TEXTURE_2D);
		
	BYTE *pTexture = NULL;
	pTexture = new BYTE[rasterWidth*rasterHeight * 3];
	memset(pTexture, 0, rasterWidth*rasterHeight * 3 * sizeof(BYTE));
		//glBindTexture(GL_TEXTURE_2D,reflectionMaps[i]);//TexPosId   PboTex
	glBindTexture(GL_TEXTURE_2D, reflectionMapTex_Now);//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	int w = rasterWidth;
	int h = rasterHeight;
	char sreenstr [32];
	sprintf(sreenstr,"./test/disoclu%d_%d.bmp",currentTime2,currentTime2);
	Fbo::SaveBMP(sreenstr, pTexture, w, h);
	if(pTexture)
		delete pTexture;
	*/
	//currentTime2++;
	if (stat_breakdown) 
	{
		double finalRendingTime = TimeMesure::getCurrentTime();;
		// sutilCurrentTime(&finalRendingTime);
		g_timeMesure.setFinalRenderingTime(finalRendingTime);
	}
    g_blendShader.setDiffuseTex(currentGbuffer.getTexture(2));
    g_blendShader.setReflectTex(reflectionMapTex_Now);
	if(renderFbo.isScreen())
	{
		screenBuffer.drawToScreen(g_blendShader);
	//	screenBuffer.SaveBMP("livRoom.bmp",0);
		//screenBuffer.SaveBMP("livRoom.bmp",0);
		/*
		char sreenstr[20];
		sprintf(sreenstr,"ref.bmp",rasterWidth);
		Fbo::saveScreen(sreenstr,windowWidth,windowHeight);
		*/
	}
	else
	{
		renderFbo.begin();
	    MyGeometry::drawQuad(g_blendShader);
	    renderFbo.end();
	}
	
    glFinish();
	CHECK_ERRORS();
	if (stat_breakdown) 
	{
		double finish_start = TimeMesure::getCurrentTime();;
		glFinish();
		//sutilCurrentTime(&finish_start);
		g_timeMesure.setEndTime(finish_start);
		g_timeMesure.print();
		g_recorder.addValue(g_timeMesure.getFps(),currentTime2);
		

	}
	/*
	char sreenstr[80];
	sprintf(sreenstr,"./groundTruth.bmp");
	//sprintf(sreenstr,"./test/disoclu%d_%d.bmp",currentTime2,optixTime*JianGe+BEGININDEX);
	Fbo::saveScreen(sreenstr,windowWidth,windowHeight);
	*/
}

void init_RefcletTex()
{
	
	CHECK_ERRORS();
	for(int i =0;i<KEYFRAMENUM;i++)
	{
		RefFrame::getFrameByIndex(i).init();
	}
	rtContext["max_depth"]->setUint(1u);
		
	for(int i = 0;i<KEYFRAMENUM;i++)
	{
		printf("render index:%d\n",i);
		CHECK_ERRORS();
		int FrameNums = i*TIMEGAP+BEGININDEX;
		CHECK_ERRORS();
		RefFrame & frame = RefFrame::getFrameByIndex(i);
		CCamera& camera = frame.getCamera();
		g_scene.cameraControl(FrameNums,camera);
		frame.getGbuffer().begin();
		g_scene.draw_model(g_gBufferShader,&camera);
		frame.getGbuffer().end();
		
#ifdef DIFFNORMAL
		
		g_diffNormalShader.setGbuffer(&frame.getGbuffer());
		g_diffNormalShader.setCamera(&camera);
	
		frame.getGbuffer().begin(nv::vec3f(0,0,0),false);
		MyGeometry::drawQuad(g_diffNormalShader);
		frame.getGbuffer().end();
		
		/*idmap 
		frame.getGbuffer().begin();d
		g_scene.draw_model(g_idShader,&camera);
		frame.getGbuffer().end();
		*/
		/*char str [32];
		sprintf(str,"./test/ref_i%d.bmp",i);
		frame.getGbuffer().SaveBMP(str,3);
		*/
		//frame.getGbuffer().debugPixel(3,358,389);
		//frame.getGbuffer().debugPixel(0,359,390);
#endif
		currentGbuffer.copyFromBuffer(frame.getGbuffer());
		CHECK_ERRORS();
		/*
		rtWorldSpaceTexture->unregisterGLTexture();
		glBindTexture(GL_TEXTURE_2D, nextframe.getGbuffer().getTexture(0) );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, traceWidth, traceHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		rtWorldSpaceTexture->registerGLTexture();

		rtNormalTexture->unregisterGLTexture();
		glBindTexture(GL_TEXTURE_2D, nextframe.getGbuffer().getTexture(1) );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, traceWidth, traceHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		rtNormalTexture->registerGLTexture();
		*/

		rtContext["eye_pos"]->setFloat(camera.Position().x, camera.Position().y, camera.Position().z);
	    rtContext["optixModelView_Inv"]->setMatrix4x4fv(false,camera.getModelViewInvMat());

		try
		{
			rtContext->launch(0, rasterWidth, rasterHeight);
		}
		catch (optix::Exception& e) 
		{
			sutilReportError( e.getErrorString().c_str() );
			exit(1);
		}
		CHECK_ERRORS();
		glPushAttrib(GL_PIXEL_MODE_BIT);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
		glBindTexture(GL_TEXTURE_2D, frame.getOptixTex());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,
			GL_RGBA, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glPopAttrib();


#ifdef EDGESHADER
		
		g_edgeShader.setGbuffer(&frame.getGbuffer());
		g_edgeShader.setCamera(&camera);
		g_edgeShader.setRes(nv::vec2f(rasterWidth,rasterWidth));
		g_edgeShader.setOptixTex(frame.getOptixTex());
	 
		frame.getGbuffer().begin(nv::vec3f(0,0,0),false);
		MyGeometry::drawQuad(g_edgeShader);
	//	frame.getGbuffer().SaveBMP("edge.bmp",2);
		frame.getGbuffer().end();

		//frame.getGbuffer().SaveBMP("difuse.bmp",3);
		/*idmap 
		frame.getGbuffer().begin();d
		g_scene.draw_model(g_idShader,&camera);
		frame.getGbuffer().end();
		*/
		/*char str [32];
		sprintf(str,"./test/ref_i%d.bmp",i);
		frame.getGbuffer().SaveBMP(str,3);
		*/
		//frame.getGbuffer().debugPixel(3,358,389);
		//frame.getGbuffer().debugPixel(0,359,390);
#endif
		/*
		glPushAttrib(GL_PIXEL_MODE_BIT);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, addtionalMapPBO);
		glBindTexture(GL_TEXTURE_2D,frame.getAdditionalTex());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,
			GL_RGBA, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glPopAttrib();
		*/
		/*
		glEnable(GL_TEXTURE_2D);
		
		BYTE *pTexture = NULL;
		pTexture = new BYTE[rasterWidth*rasterHeight * 3];
		memset(pTexture, 0, rasterWidth*rasterHeight * 3 * sizeof(BYTE));
		//glBindTexture(GL_TEXTURE_2D,reflectionMaps[i]);//TexPosId   PboTex
		glBindTexture(GL_TEXTURE_2D, (RefFrame::getFrameByIndex(i).getOptixTex()));//TexPosId   PboTex

		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

		int w = rasterWidth;
		int h = rasterHeight;
		char str [32];
		sprintf(str,"./ref%d.bmp",1024);
		Fbo::SaveBMP(str, pTexture, w, h);
		*/
		/*
		glEnable(GL_TEXTURE_2D);
		int x= 67,y =572;  
		int xlenght = 1024,ylength = 1024;
		float* pTexture = new float[rasterWidth*rasterHeight * 4];
		memset(pTexture, 0, rasterWidth*rasterHeight * 4 * sizeof(float));

		glBindTexture(GL_TEXTURE_2D, (RefFrame::getFrameByIndex(0).getAdditionalTex()));//TexPosId   PboTex

		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pTexture);

		int w = rasterWidth;
		int h = rasterHeight;
		nv::vec4f pixel = nv::vec4f(&pTexture[4*(y*ylength+x)]);
		nv::vec3f truePos = nv::vec3f(pixel.x,pixel.y,pixel.z);
		float id = pixel.x;

		if (pTexture)
		  delete[] pTexture;
		*/
	}
	rtContext["max_depth"]->setUint(rayTraceDepth);
		
	



}



void setTitle()
{
	static char titleBuffer[48];
	sprintf(titleBuffer,"Frame:%d",currentTime2);
	glutSetWindowTitle(titleBuffer); 
}
void drawFinalColor(int time,Fbo& finalFbo)
{
	g_scene.cameraControl(time*TIMEGAP,g_scene.m_refCamera);
	//test();
	RefFrame & frame = RefFrame::getFrameByIndex(time);
	refGbuffer = frame.getGbuffer();
	if (stat_breakdown) 
	{
		double cudaBeginTime=g_timeMesure.getCurrentTime();;
		g_timeMesure.setCudaBeginTime(cudaBeginTime);
	}
	float ComputeStar=0,ComputeEnd;
	
	
	poxCudaTex.setEveryTex(frame.getGbuffer().getTexture(0));
	normalCudaTex.setEveryTex(frame.getGbuffer().getTexture(1));
	reflectCudaTex.setEveryTex(frame.getOptixTex());

#ifdef DIFFNORMAL
	diffCudaTex.setEveryTex(frame.getGbuffer().getTexture(3));
#endif
	ComputeVecInCuda();
	fflush(stdout);
	if (stat_breakdown) 
	{
		double forwardBeginTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setForwardingTime(forwardBeginTime);
	}

	TransMapFbo.begin(); 
	drawTransMap(time);
	TransMapFbo.end();
	char str[100];

	if(stat_breakdown)
	{
		glFinish();
		
	}
	finalFbo.begin();
	drawFinalEffect();
	finalFbo.end();
	
}
void hybridRendering()
{
	
	setTitle();
	sutilFrameBenchmark("isgReflections", warmup_frames, timed_frames);
	double FrameStart,FrameEnd;

	//sutilCurrentTime(&FrameStart);
	FrameStart = TimeMesure::getCurrentTime();
	if (stat_breakdown) 
	{
		double frameStartTime =g_timeMesure.getCurrentTime();
		//sutilCurrentTime(&frameStartTime);
		g_timeMesure.setBeginTime(frameStartTime);
	}
	g_scene.cameraControl(currentTime2,g_scene.m_curCamera);
	glViewport(0,0, traceWidth, traceHeight);
	currentGbuffer.begin();
	g_scene.draw_model(g_gBufferShader,&g_scene.m_curCamera);
	currentGbuffer.end();
	
	
	int OptixFrame = g_keyFrame.getOptixFrame(currentTime2);

	

	drawFinalColor(OptixFrame,FinalEffectFbo);
	int anotherTime = OptixFrame;
	if(currentTime2 %JianGe> JianGe/2)
	{
		anotherTime--;
	}
	else
	{
		anotherTime++;
	}
	drawFinalColor(anotherTime,FinalEffectFbo2);
	
	
	double DrawTime1,DrawTime2;
	glFinish();
	sutilCurrentTime(&DrawTime1);
	if (stat_breakdown) 
	{
		// glFinish();
		double finalRenderingTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setFinalRenderingTime(finalRenderingTime);
	}
	g_hybridShader.setDiffuseTex(currentGbuffer.getTexture(2));
	g_hybridShader.setCurrentTime(currentTime2);
	g_hybridShader.setTex1(OptixFrame*JianGe,FinalEffectFbo.getTexture(0));
	g_hybridShader.setTex2(anotherTime*JianGe,FinalEffectFbo2.getTexture(0));
	screenBuffer.drawToScreen(g_hybridShader);
	glFinish();

	double finish_start;
	if (stat_breakdown) 
	{
		glFinish();
		double finish_start=g_timeMesure.getCurrentTime();
		g_timeMesure.setEndTime(finish_start);
		g_timeMesure.print();

		
	}
	
	/*char sreenstr[20];
	sprintf(sreenstr,"./test/hybrid%d.bmp",currentTime2);
	Fbo::saveScreen(sreenstr,windowWidth,windowHeight);
	*/
}
void noGeometryRendering(Fbo renderFbo = Fbo())
{
	setTitle();
	if (stat_breakdown) 
	{
		glFinish();
		double display_start = TimeMesure::getCurrentTime();
		// sutilCurrentTime(&display_start);
		g_timeMesure.setBeginTime(display_start);
	}
	CVector3& pos =g_scene.m_refCamera.Position();
	g_scene.cameraControl(currentTime2,g_scene.m_curCamera);
	g_scene.m_refCamera = g_scene.m_curCamera;
	rtContext["eye_pos"]->setFloat(g_scene.m_curCamera.Position().x, g_scene.m_curCamera.Position().y, g_scene.m_curCamera.Position().z);
	rtContext["bbmin"]->setFloat(g_scene.m_curCamera.getImageMin().x, g_scene.m_curCamera.getImageMin().y);
	rtContext["bbmax"]->setFloat(g_scene.m_curCamera.getImageMax().x, g_scene.m_curCamera.getImageMax().y);
	rtContext["optixModelView_Inv"]->setMatrix4x4fv(false,g_scene.m_refCamera.getInvMvp());
	CVector3 view = g_scene.m_refCamera.View()  - g_scene.m_curCamera.Position();
	nv::matrix4f matInv = inverse(nv::matrix4f(g_scene.m_refCamera.getModelViewMat()));
	rtContext["optixModelView_Inv"]->setMatrix4x4fv(false,matInv.get_value());
	
  
	try
	{
		if (stat_breakdown) 
		{
			double optixBeginTime = TimeMesure::getCurrentTime();;
			g_timeMesure.setOptixBeginTime(optixBeginTime);
		}
		rtContext->launch(2, rasterWidth, rasterHeight);
	}
	catch (optix::Exception& e) 
	{
		sutilReportError( e.getErrorString().c_str() );
		exit(1);
	}

	glPushAttrib(GL_PIXEL_MODE_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
	glBindTexture(GL_TEXTURE_2D,reflectionMapTex_Now);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,
		GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glPopAttrib();
#ifdef NOGEOMETRY
	glPushAttrib(GL_PIXEL_MODE_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, posPBO);
	glBindTexture(GL_TEXTURE_2D,currentGbuffer.getTexture(0));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,
		GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glPopAttrib();

	glPushAttrib(GL_PIXEL_MODE_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, normalPBO);
	glBindTexture(GL_TEXTURE_2D,currentGbuffer.getTexture(1));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,
		GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glPopAttrib();

	glPushAttrib(GL_PIXEL_MODE_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, colorPBO);
	glBindTexture(GL_TEXTURE_2D,currentGbuffer.getTexture(2));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,
		GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glPopAttrib();

#endif
	g_blendShader.setDiffuseTex(currentGbuffer.getTexture(2));
    g_blendShader.setReflectTex(reflectionMapTex_Now);
	if(renderFbo.isScreen())
	{
		  screenBuffer.drawToScreen(g_blendShader);
		 // screenBuffer.SaveBMP("optix.bmp",0);
    }
	else
	{
		renderFbo.begin();
	    MyGeometry::drawQuad(g_blendShader);
	    renderFbo.end();
	}
   glFinish();
	if (stat_breakdown) 
	{
		double finish_start = TimeMesure::getCurrentTime();;
		glFinish();
		//sutilCurrentTime(&finish_start);
		g_timeMesure.setEndTime(finish_start);
		g_timeMesure.print();
	}
}

void noGeometryRendering2()
{
	setTitle();
	sutilFrameBenchmark("isgReflections", warmup_frames, timed_frames);
	double FrameStart,FrameEnd;

	//sutilCurrentTime(&FrameStart);
	FrameStart = TimeMesure::getCurrentTime();
	if (stat_breakdown) 
	{
		double frameStartTime =g_timeMesure.getCurrentTime();
		//sutilCurrentTime(&frameStartTime);
		g_timeMesure.setBeginTime(frameStartTime);
	}
	/*g_scene.cameraControl(currentTime2,g_scene.m_curCamera);
	glViewport(0,0, traceWidth, traceHeight);
	currentGbuffer.begin();
	g_scene.draw_model(g_gBufferShader,&g_scene.m_curCamera);
	currentGbuffer.end();
	*/
	int OptixFrame = currentTime2 /JianGe;
	if(currentTime2 %JianGe> JianGe/2)
		OptixFrame++;
	RefFrame & frame = RefFrame::getFrameByIndex(OptixFrame);
	g_scene.cameraControl(currentTime2,g_scene.m_curCamera);
	g_imgWarpShader.setRefColorTex(frame.getGbuffer().getTexture(2));
	g_imgWarpShader.setRefNormalTex(frame.getGbuffer().getTexture(1));
	g_imgWarpShader.setRefPosTex(frame.getGbuffer().getTexture(0));
	g_imgWarpShader.setMvpMatrix(g_scene.m_curCamera.getMvpMat());
	//g_imgWarpShader.setMvpMatrix(frame.getCamera().getMvpMat());
	//currentGbuffer.copyFromBuffer(frame.getGbuffer());
	currentGbuffer.begin(viewIndepentdentMissColor);
	g_imgMesh.drawImgMesh(g_imgWarpShader);
	//currentGbuffer.debugPixel(0,678,678);
	/*glEnable(GL_TEXTURE_2D);
	float *pTexture = NULL;
	pTexture = new float[1024*1024 * 4];
	memset(pTexture, 0, 1024*1024 * 4 * sizeof(float));

	glBindTexture(GL_TEXTURE_2D, frame.getGbuffer().getTexture(0));//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pTexture);

	int w = 1024;
	int h = 1024;
	 float * pMvpMatrix =  frame.getCamera().getMvpMat();
	int y = 678;
	int x= 678;
	int scale = 1;
	int index = y*w+x;
	float  r = pTexture[4*index]*scale;
	float g = pTexture[4*index+1]*scale;
	float b = pTexture[4*index+2]*scale;
	float a = pTexture[4*index+3]*scale;

	nv::vec3f worldPos = nv::vec3f(r,g,b);
	nv::matrix4f mvpMatrix = nv::matrix4f(pMvpMatrix);
	nv::vec4f temp = mvpMatrix*  nv::vec4f(worldPos,1);
	temp/= temp.w;

	
	glBindTexture(GL_TEXTURE_2D, 0);
	currentGbuffer.end();*/
	//currentGbuffer.SaveBMP("./test/noGeometry.bmp",0);
	drawFinalColor(OptixFrame,FinalEffectFbo);
	/*int pixelNum = mapping();
	double DrawTime1,DrawTime2;
	glFinish();
	sutilCurrentTime(&DrawTime1);
	if (stat_breakdown) 
	{
		// glFinish();
		double finalRenderingTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setFinalRenderingTime(finalRenderingTime);
	}
	
	g_blendShader.setDiffuseTex(currentGbuffer.getTexture(2));
	g_blendShader.setReflectTex(FinalEffectFbo.getTexture(0));
	g_blendShader.setNewReflectTex(reflectionMapTex_Now);
	screenBuffer.drawToScreen(g_blendShader);
	glFinish();
	*/
	screenBuffer.clear();
	

	double finish_start;
	if (stat_breakdown) 
	{
		glFinish();
		double finish_start=g_timeMesure.getCurrentTime();
		g_timeMesure.setEndTime(finish_start);
		g_timeMesure.print();

		
	}
}
void noGeometryTc(Fbo renderFbo = Fbo())
{
	printf("time:%d\n",currentTime2);
	//currentTime2++;
	setTitle();

	sutilFrameBenchmark("isgReflections", warmup_frames, timed_frames);
	double FrameStart,FrameEnd;

	//sutilCurrentTime(&FrameStart);
	FrameStart = TimeMesure::getCurrentTime();
	if (stat_breakdown) 
	{
		double frameStartTime =g_timeMesure.getCurrentTime();
		//sutilCurrentTime(&frameStartTime);
		g_timeMesure.setBeginTime(frameStartTime);
	}
	g_scene.cameraControl(currentTime2,g_scene.m_curCamera);
	
	
	int OptixFrame = g_keyFrame.getOptixFrame(currentTime2);

	g_scene.cameraControl(OptixFrame*JianGe+BEGININDEX,g_scene.m_refCamera);
	//test();
	RefFrame & frame = RefFrame::getFrameByIndex(OptixFrame);
	refGbuffer = frame.getGbuffer();

	if (stat_breakdown) 
	{
		double cudaBeginTime=g_timeMesure.getCurrentTime();;
		g_timeMesure.setCudaBeginTime(cudaBeginTime);
	}
	
	if(g_keyFrame.isKeyFrameChage())
	{
		poxCudaTex.setEveryTex(frame.getGbuffer().getTexture(0));
		normalCudaTex.setEveryTex(frame.getGbuffer().getTexture(1));
#ifdef DIFFNORMAL
		diffCudaTex.setEveryTex(frame.getGbuffer().getTexture(3));
#endif
		reflectCudaTex.setEveryTex(frame.getOptixTex());
	}
	ComputeVecInCuda();
	
	if (stat_breakdown) 
	{
		double forwardBeginTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setForwardingTime(forwardBeginTime);
	}
	TransMapFbo.begin(); 
	drawTransMapNoPipe(OptixFrame);

	
	char ttt[30];
	//sprintf(ttt,"test.bmp");
	//sprintf(sreenstr,"./test/disoclu%d_%d.bmp",currentTime2,optixTime*JianGe+BEGININDEX);
	
	//TransMapFbo.SaveBMP(ttt,0);
	
	//TransMapFbo.debugPixel(0,608,551);
	TransMapFbo.end();
	char str[100];
	

	g_imageWarpNopipeShader.setRefColorTex(frame.getGbuffer().getTexture(2));
	g_imageWarpNopipeShader.setRefNormalTex(frame.getGbuffer().getTexture(1));
 	g_imageWarpNopipeShader.setRefPosTex(frame.getGbuffer().getTexture(0));
	g_imageWarpNopipeShader.setEdgeTex(frame.getGbuffer().getTexture(3));
	g_imageWarpNopipeShader.setMvpMatrix(g_scene.m_curCamera.getMvpMat());
	g_imageWarpNopipeShader.setReflectedTex(TransMapFbo.getTexture(0));
#ifdef NAIVE
	g_imageWarpNopipeShader.setNaive(true);
#else
	g_imageWarpNopipeShader.setNaive(false);
#endif
	//g_imgWarpShader.setMvpMatrix(frame.getCamera().getMvpMat());
	//currentGbuffer.copyFromBuffer(frame.getGbuffer());

	FinalEffectFbo.begin(viewIndepentdentMissColor);
	g_imgMesh.drawImgMesh(g_imageWarpNopipeShader);
	FinalEffectFbo.end();
	
	//sprintf(ttt,"FinalEffectFbo%d.bmp",currentTime2);
	//sprintf(sreenstr,"./test/disoclu%d_%d.bmp",currentTime2,optixTime*JianGe+BEGININDEX);
	
//	FinalEffectFbo.SaveBMP(ttt,0);
	
	/*
	if (stat_breakdown) 
	{
		// glFinish();
		double secendRacingTimec=g_timeMesure.getCurrentTime();
		g_timeMesure.setSecondTraceTime(secendRacingTimec);
	}*/
		int pixelNum =0;
    pixelNum = mappingNoPipe();
	glFinish();
	if (stat_breakdown) 
	{
		// glFinish();
		double finalRenderingTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setFinalRenderingTime(finalRenderingTime);
	}
	g_blendShaderNoPipe.setOriginTex(FinalEffectFbo.getTexture(0));
	g_blendShaderNoPipe.setAdditionalTex(reflectionMapTex_Now);
	
	if(renderFbo.isScreen())
	{
		  //screenBuffer.drawToScreen(g_blendShader);
			  //screenBuffer.SaveBMP("diffuse1.bmp",0);
		screenBuffer.drawToScreen(g_blendShaderNoPipe);
	//	Fbo::saveScreen("optix2.bmp",windowWidth,windowHeight);
		
	}
	else
	{
		renderFbo.begin();
	    MyGeometry::drawQuad(g_blendShaderNoPipe);
	    renderFbo.end();
	}
	
	

	//screenBuffer.clearScreen();
	
	double finish_start;
	if (stat_breakdown) 
	{
		double finish_start=g_timeMesure.getCurrentTime();
		//int ananlyseNum = g_fboAnalyse.analyseColorNum(FinalEffectFbo,0);
		//float ratio = ananlyseNum/(float)g_fboAnalyse.getRes().x/(float)g_fboAnalyse.getRes().y;
		g_timeMesure.setEndTime(finish_start);
		g_timeMesure.print();

		
	}
	
}
void tcRendering(Fbo renderFbo = Fbo())
{
	
	setTitle();
	sutilFrameBenchmark("isgReflections", warmup_frames, timed_frames);
	double FrameStart,FrameEnd;

	//sutilCurrentTime(&FrameStart);
	FrameStart = TimeMesure::getCurrentTime();
	if (stat_breakdown) 
	{
		double frameStartTime =g_timeMesure.getCurrentTime();
		//sutilCurrentTime(&frameStartTime);
		g_timeMesure.setBeginTime(frameStartTime);
	}
	g_scene.cameraControl(currentTime2,g_scene.m_curCamera);
	
	
	int OptixFrame = g_keyFrame.getOptixFrame(currentTime2);

	g_scene.cameraControl(OptixFrame*JianGe+BEGININDEX,g_scene.m_refCamera);
	RefFrame & frame = RefFrame::getFrameByIndex(OptixFrame);
	refGbuffer = frame.getGbuffer();
	//refGbuffer.SaveBMP("diffuse.bmp",2);
	if (stat_breakdown) 
	{
		double cudaBeginTime=g_timeMesure.getCurrentTime();;
		g_timeMesure.setCudaBeginTime(cudaBeginTime);
	}
	
	if(g_keyFrame.isKeyFrameChage())
	{
		poxCudaTex.setEveryTex(frame.getGbuffer().getTexture(0));
		normalCudaTex.setEveryTex(frame.getGbuffer().getTexture(1));
#ifdef DIFFNORMAL
		diffCudaTex.setEveryTex(frame.getGbuffer().getTexture(3));
#endif
		reflectCudaTex.setEveryTex(frame.getOptixTex());
	}
	/*
	glEnable(GL_TEXTURE_2D);
	BYTE *pTexture = NULL;
	int w = 1024;
	int h = 1024;
	pTexture = new BYTE[w*h * 3];
	memset(pTexture, 0, w*h* 3 * sizeof(BYTE));

	glBindTexture(GL_TEXTURE_2D, frame.getOptixTex());//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	
	Fbo::SaveBMP("aa.bmp", pTexture, w, h);
	if (pTexture)
	   delete[] pTexture;
	glBindTexture(GL_TEXTURE_2D, 0);//
	*/
	//CpuTracint(reflectionMaps[OptixFrame]);
	//   cudaEvent_t start,stop;
	//   cudaEventCreate(&start);
	//   cudaEventCreate(&stop);

	// cudaEventRecord(start,0);
 	ComputeVecInCuda();
	//checkCudaErrors(cudaGraphicsUnregisterResource(cudaRes_Reflect));
	//    cudaEventRecord(stop,0);
	//    cudaEventSynchronize(start);
	//    cudaEventSynchronize(stop);
	//    cudaEventElapsedTime(&ComputeStar,start,stop);
	// cudaMemcpy(

	/*glPushAttrib(GL_PIXEL_MODE_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, VectorPBO);
	glBindTexture(GL_TEXTURE_2D, VecorTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rasterWidth, rasterHeight,GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glPopAttrib();
	*/
	

	/*
	glEnable(GL_TEXTURE_2D);
	float *pTexture = NULL;
	pTexture = new float[rasterWidth*rasterHeight * 4];
	memset(pTexture, 0, rasterWidth*rasterHeight * 4 * sizeof(float));

	glBindTexture(GL_TEXTURE_2D, VecorTexture);//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pTexture);
	int w = rasterWidth,h= rasterHeight;

	int x = 1023;
	int y = 586;
	int index = y*w+x;
	float r = pTexture[4*index]*1024;
	float g = pTexture[4*index+1]*1024;
	float b = pTexture[4*index+2];
	float a = pTexture[4*index+3];
	if (pTexture)
	   delete[] pTexture;
	glBindTexture(GL_TEXTURE_2D, 0);
	*/
	/*
	g_showShader.setTex1(VecorTexture);
	currentGbuffer.begin();
	MyGeometry::drawQuad(g_showShader);
	currentGbuffer.SaveBMP("./test/vector.bmp",0);
	currentGbuffer.end();
	*/
	//Draw TransMap

	if (stat_breakdown) 
	{
		double forwardBeginTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setForwardingTime(forwardBeginTime);
	}
	
	TransMapFbo.begin(); 
	drawTransMap(OptixFrame);
	//TransMapFbo.SaveBMP("transMap.bmp",0);
	//TransMapFbo.debugPixel(0,917,574);
	TransMapFbo.end();
	char str[100];
	

	if (stat_breakdown) 
	{
		double imgProjStartTime =g_timeMesure.getCurrentTime();
		g_timeMesure.setImageProjectStartTime(imgProjStartTime);
	}
	
	glViewport(0,0, traceWidth, traceHeight);
	currentGbuffer.begin();
	g_scene.draw_model(g_gBufferShader,&g_scene.m_curCamera);
	//currentGbuffer.SaveBMP("mark.bmp",2);
	currentGbuffer.end();
	
	
	if (stat_breakdown) 
	{
		double forwardBeginTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setForwardingTime(forwardBeginTime);
	}
	FinalEffectFbo.begin();
	
	
	drawFinalEffect();
	//FinalEffectFbo.SaveBMP("FinalEffectFbo.bmp",0);
	
	FinalEffectFbo.end();
	


	/*
	if (stat_breakdown) 
	{
		// glFinish();
		double secendRacingTimec=g_timeMesure.getCurrentTime();
		g_timeMesure.setSecondTraceTime(secendRacingTimec);
	}*/
	int pixelNum =0;
    pixelNum = mapping();
	glFinish();
	if (stat_breakdown) 
	{
#ifdef ANALYSIS
		g_fboAnalyse.setCompareColor(nv::vec3f(0,0,1));
		g_fboAnalyse.setEuqal(false);
		int reflectPixl = g_fboAnalyse.analyseColorNum(FinalEffectFbo,0);
		float additionRaito = (float)pixelNum/reflectPixl;
		g_recorder.addValue(additionRaito,currentTime2);
#endif
		// glFinish();
		double finalRenderingTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setFinalRenderingTime(finalRenderingTime);
	}
	g_blendShader.setDiffuseTex(currentGbuffer.getTexture(2));
	g_blendShader.setReflectTex(FinalEffectFbo.getTexture(0));
	g_blendShader.setNewReflectTex(reflectionMapTex_Now);
	
	if(renderFbo.isScreen())
	{
		screenBuffer.drawToScreen(g_blendShader);
		//screenBuffer.SaveBMP("test.bmp",0);
	}
	else
	{
		renderFbo.begin();
	    MyGeometry::drawQuad(g_blendShader);
	    renderFbo.end();
	}
	//screenBuffer.clearScreen();
	
	double finish_start;
	if (stat_breakdown) 
	{
		double finish_start=g_timeMesure.getCurrentTime();
			g_timeMesure.setEndTime(finish_start);
		// g_timeMesure.print();
		//g_recorder.addValue(g_timeMesure.getFps(),currentTime2);

		
	}
	/*
	char sreenstr[80];
	sprintf(sreenstr,"./movie/livingRoomTcAddition/livingRoomTcAddition%d.bmp",currentTime2);
	//sprintf(sreenstr,"./test/disoclu%d_%d.bmp",currentTime2,optixTime*JianGe+BEGININDEX);
	Fbo::saveScreen(sreenstr,windowWidth,windowHeight);
	*/
	/*
	char sreenstr[60];
	sprintf(sreenstr,"./test%d.bmp",currentTime2);
	Fbo::saveScreen(sreenstr,windowWidth,windowHeight);
	*/
	
	char sreenstr[20];
	sprintf(sreenstr,"./re-compu1.bmp");
	Fbo::saveScreen(sreenstr,windowWidth,windowHeight);
	
}

void compareTypRendering()
{
	//optixRendering(optixFrame);
	tcRendering(tcFrame);
	tcFrame.SaveBMP("pg.bmp",0);
	return;
	tcRendering(tcFrame);
	g_compareShader.setTex1(optixFrame.getTexture(0));
	g_compareShader.setTex2(tcFrame.getTexture(0));
	screenBuffer.drawToScreen(g_compareShader);
	//screenBuffer.SaveBMP("test.bmp",0);
	glFinish();
	char sreenstr[80];
	sprintf(sreenstr,"./compare%d.bmp",rasterWidth);
	//sprintf(sreenstr,"./test/disoclu%d_%d.bmp",currentTime2,optixTime*JianGe+BEGININDEX);
	Fbo::saveScreen(sreenstr,windowWidth,windowHeight);

}
void display()
{
	if(optixRenderingType == g_timeMesure.getType())
	{
		optixRendering();
	}
	else if(tcRenderingType == g_timeMesure.getType())
	{
		tcRendering();
	}
	else if(hybridRenderingType == g_timeMesure.getType())
	{
		hybridRendering();
	}
	else if(noGeometryRenderingType == g_timeMesure.getType())
	{
		noGeometryRendering();
	}
	else if(compareType == g_timeMesure.getType())
	{
		compareTypRendering();
	}
	else if(noGeometryTcType == g_timeMesure.getType())
	{
		noGeometryTc();
	}
	static int sampleNum =0;
	sampleNum++;
	/*
	if(sampleNum == 10)
	{
		sampleNum = 0;
		currentTime2 ++;
	}
	*/
	if(currentTime2 == ENDINDEX)
	{
		g_recorder.save();
		exit(0);
	}
	
	
	//g_timeMesure.nextFrame(currentTime2);
	g_timeMesure.updateLastTime();
	CHECK_ERRORS();
	glutSwapBuffers();
	glutReportErrors();
}
void resize(int w, int h)
{
	// We don't want to allocate 0 memory for the PBOs
	w = w == 0 ? 1 : w; 
	h = h == 0 ? 1 : h; 

	//rasterWidth = w;
	//rasterHeight = h;

	traceWidth = rasterWidth >> logReflectionSamplingRate;
	traceHeight = rasterHeight >> logReflectionSamplingRate;

	//manipulator.reshape(w,h);
	/*
	float aspect = (float)w/(float)h;
	float diag = nv::length(modelBBMax - modelBBMin);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspect, diag*.01, diag*100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/
	// resize 
	try {
		rtReflectionBuffer->unregisterGLBuffer();
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, traceWidth*traceHeight*sizeof(float4), 0, GL_STREAM_READ);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		rtReflectionBuffer->registerGLBuffer();

		// resize FBOs
		rtWorldSpaceTexture->unregisterGLTexture();
		glBindTexture(GL_TEXTURE_2D, worldSpaceNormalFBO->GetAttachedId( GL_COLOR_ATTACHMENT0_EXT ) );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, traceWidth, traceHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		rtWorldSpaceTexture->registerGLTexture();

		rtNormalTexture->unregisterGLTexture();
		glBindTexture(GL_TEXTURE_2D, worldSpaceNormalFBO->GetAttachedId( GL_COLOR_ATTACHMENT1_EXT ) );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, traceWidth, traceHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		rtNormalTexture->registerGLTexture();

		rtLastReflectTexture->unregisterGLTexture();
		glBindTexture(GL_TEXTURE_2D, FinalEffectFbo.getTexture(0) );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, traceWidth, traceHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		rtLastReflectTexture->registerGLTexture();



	} 
	catch ( optix::Exception& e )
	{
		sutilReportError( e.getErrorString().c_str() );
		exit(-1);
	}

	glBindTexture(GL_TEXTURE_2D, worldSpaceNormalFBO->GetAttachedId( GL_DEPTH_ATTACHMENT_EXT ) );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8_EXT, traceWidth, traceHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

	// resize reflection map texture
	glBindTexture(GL_TEXTURE_2D, reflectionMapTex_Now);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, traceWidth, traceHeight, 0, GL_RGBA, GL_FLOAT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// resize rt buffers
	rtReflectionBuffer->setSize(traceWidth,traceHeight);

	glutPostRedisplay();

	CHECK_ERRORS();
}

void idle()
{
	//g_scene.m_curCamera.Update();
	g_scene.update();
	// Optix_Camare.Update();
	//  manipulator.idle();
	glutPostRedisplay();
}

void special_key(int k, int x, int y)
{
	glutPostRedisplay();
}


void keyboard(unsigned char k, int x, int y)
{
	bool reshape = false;
	nv::vec3f & sceneLight = g_scene.getLightPos();
	switch(k) {

	case 27: // esc
		exit(0);
		break;

	/*case 'w':
		printf("write file");
		g_recorder.save();
		break;*/
/*	case 'e':
		scene_epsilon /= 10;
		rtContext["scene_epsilon"]->setFloat(scene_epsilon);
		break;
	
	case 'E':
		scene_epsilon *= 10;
		rtContext["scene_epsilon"]->setFloat(scene_epsilon);
		break;
		*/
	case ' ':

		// Optix_Camare.navigate(posArray[0],posArray[1],currentTime2,0,100);
		// cout<<currentTime2<<endl;

		g_Recordpic = true;
		break;
	case 'f':
		{
			static int old_width = -1;
			static int old_height = -1;
			static int old_x = -1;
			static int old_y = -1;
			if(old_width == -1) {
				old_width = glutGet(GLUT_WINDOW_WIDTH);
				old_height = glutGet(GLUT_WINDOW_HEIGHT);
				old_x = glutGet(GLUT_WINDOW_X);
				old_y = glutGet(GLUT_WINDOW_Y);
				glutFullScreen();
			} else {
				glutPositionWindow(old_x, old_y);
				glutReshapeWindow(old_width, old_height);
				old_width = -1;
			}
		}
		break;

	case 'g':
		glossiness -= 0.0025f;
		if(glossiness < 0)
			glossiness = 0;
		break;

	case 'G':
		glossiness += 0.0025f;
		if(glossiness > .1f)
			glossiness = .1f;
		break;

	case 'i':
		stat_breakdown = !stat_breakdown;
		break;

	case 'r':
		animate = !animate;
		if(animate) {
			glutIdleFunc(idle);
		} else {
			glutIdleFunc(0);
		}
		break;
	case 'p':
		printf("{make_float3(%f,%f,%f ),make_float3(%f,%f,%f)},\n",g_scene.m_curCamera.Position().x,g_scene.m_curCamera.Position().y,g_scene.m_curCamera.Position().z,g_scene.m_curCamera.View().x,g_scene.m_curCamera.View().y,g_scene.m_curCamera.View().z);
		fflush(stdout);
		break;
		/*case 's':
		logReflectionSamplingRate += 1;
		if(rasterWidth  >> logReflectionSamplingRate < 2 ||
		rasterHeight >> logReflectionSamplingRate < 2 ||
		logReflectionSamplingRate >= 7)
		logReflectionSamplingRate -= 1;
		reshape = true;
		break;

		case 'S':
		logReflectionSamplingRate -= 1;
		if(logReflectionSamplingRate < 0)
		logReflectionSamplingRate = 0;
		reshape = true;
		break;
		*/
	/*case 'a':sceneLight.x-=2;
		break;
	case 'd':sceneLight.x +=2;
		break;
	case 'w':sceneLight.y +=2;
		break;
	case 's':sceneLight.y -=2;
		break;
	case 'q':sceneLight.z -=2;
		break;
	case 'e':sceneLight.z +=2;
		break;*/
	case '=':
		currentTime2++;
		currentTime2 = std::min<float>(currentTime2,900);
		//currentTime++;
		break;
	case '-':
		currentTime2--;
		currentTime2 = std::max<float>(currentTime2,0);
		//currentTime++;
		//currentTime--;
		break;

		// Optix_Camare.CoutCamera();

	}

	if(reshape) {
		resize(rasterWidth, rasterHeight);
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	//manipulator.mouse(button, state, x, y);
	glutPostRedisplay();
}

void motion(int x, int y)
{
	//manipulator.motion(x,y);
	glutPostRedisplay();
}

void printUsageAndExit( const std::string& argv0, bool doExit = true )
{
	std::cerr
		<< "Usage  : " << argv0 << " [options]\n"
		<< "App options:\n"
		<< "  -h  | --help                               Print this usage message\n"
		<< "  -o  | --obj <obj_file>                     Specify .OBJ model to be rendered\n"
		<< "  -s  | --shader <cgfx_file>                 Specify cgfx effect to load\n"
		<< "  -z  | --zup                                Load model as positive Z as the up direction\n"
		<< "  -c  | --camera                             Use the following 16 values to specify the modelview matrix\n"
		<< "  -nf | --nofps                              Do not display the FPS counter\n"
		<< "  -cn | --compute-normals                    Compute normals instead of reading them from the file\n" 
		<< "  -b  | --benchmark[=<w>x<t>]                Render and display 'w' warmup and 't' timing frames, then exit\n"
		<< "        --dim=<width>x<height>               Set image dimensions\n"
		<< std::endl;

	std::cerr
		<< "App keystrokes:\n"
		<< "  q Quit\n"
		<< "  f Toggle full screen\n"
		<< "  e Decrease scene epsilon size (used for shadow ray offset)\n"
		<< "  E increase scene epsilon size (used for shadow ray offset)\n"
		<< "  s Decrease the reflection sampling rate\n"
		<< "  S Increase the reflection sampling rate\n"
		<< "  l Toggle mouse control of the light position\n"
		<< "  r Toggle camera hold/continuous rendering\n"
		<< "  b Toggle reflection blurring\n"
		<< "  g Decrease the glossiness scalar\n"
		<< "  G Increase the glossiness scalar\n"
		<< "  t Toggle relections\n"
		<< "  i Toggle frame break down timings\n"
		<< std::endl;

	if ( doExit ) exit(1);
}

int main(int argc, char** argv)
{
	// Allow glut to consume its args first
	if(argc==2)
	{
		BEGININDEX = atoi(argv[1]);
		ENDINDEX = BEGININDEX+TIMEGAP;
		currentTime2 = BEGININDEX;
	}
	if(argc==3)
	{
		BEGININDEX = atoi(argv[1]);
		ENDINDEX = atoi(argv[2]);
		currentTime2 = BEGININDEX;
	}
	
	/*FileRecorder g_optixRecorder = optixToiletRecorder(900) ;
    FileRecorder g_tcRecorder = tcToiletRecorder(900) ;
	g_optixRecorder.init();
	g_tcRecorder.init();
	std::vector<frameStruct> & tcVector = g_tcRecorder.getVec();
	std::vector<frameStruct> & optixVector = g_optixRecorder.getVec();
	for(int i=0;i<900;i+=30)
	{
		float avgValue = optixVector[i].getAvgNumber();
		tcVector[i].maxValue = avgValue;
	}
	g_tcRecorder.save();*/
	freopen("stderr.txt","w",stderr);
	glutInit(&argc, argv);

	bool dobenchmark = false;


	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutCreateWindow("isgReflections");

	init_gl();
	init_scene();
	init_optix();
	CHECK_ERRORS();
	init_cuda(argc,argv);
	CHECK_ERRORS();
	if( g_timeMesure.needPreocompute()){
		init_RefcletTex();
	}
	CHECK_ERRORS();
	glutReportErrors();

	//MyCuda();
	glutDisplayFunc(display);
	//glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	// glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	glutSpecialFunc(special_key);
	glutIdleFunc(idle);
	if(dobenchmark) {
		glutIdleFunc(idle);
		animate = true;
		stat_breakdown = false;
	} else {
		warmup_frames = timed_frames = 0;
	}

	glutMainLoop();
}
