
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
#include "Fbo1.h"
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
//#include "toiletScene.h"

unsigned int *Host_PixelSum;
unsigned int *Host_PixelState;

unsigned int *Dev_PixelPos;

int PixelsSum;

double AllTime = 0;

bool isSavePic = false;

const int ReflectNum = 4;

nv::Model* model,*sofaModel;

nv::vec3f modelBBMin, modelBBMax, modelBBCenter;
nv::vec3f lightPos;
float glossiness = 0.0f;
GLuint reflectionMapTex;
GLuint reflectionMaps[ReflectNum];
GLuint reflectionMapTex_Now;

GLuint Merge_FrameCount;
GLuint groundTex;
GLuint wallTex;

//GLuint goldTex;
GLuint woodTex;
GLuint sampleFbo;
GLuint sampleTex;
GLuint depthTex;
GLuint blendFactorTex;
GLuint MyVBO;
GLuint MyCallList;
char blendFactorPath[] = "./model/blendFactor0.15.bmp";
GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT,GL_COLOR_ATTACHMENT2_EXT,GL_COLOR_ATTACHMENT3_EXT,GL_COLOR_ATTACHMENT4_EXT,GL_COLOR_ATTACHMENT5_EXT,GL_COLOR_ATTACHMENT6_EXT};
optix::GeometryGroup geometrygroup;

CCamera g_refCamera,g_currentCamera;
//uint *Host_PixelSum;
//MyGeometry teapot;
int CountTime = 1000;
TimeMesure g_timeMesure(optixRenderingType,CountTime);
nv::vec3f viewDependentMissColor = nv::vec3f(255,0,0);
nv::vec3f viewIndepentdentMissColor = nv::vec3f(0,255,0);
//cudaGraphicsResource *cudaRes_WorldNormal,*cudaRes_WorldPos,*cudaRes_Reflect;
TestShader g_testShader;

CudaTexResourse poxCudaTex,normalCudaTex,reflectCudaTex,finalEffectCudaTex;
CudaPboResource vectorCudaArray,lastCudaArray,finalEffectCudaArray;

//#define  geoNumber  43

//#define  geoNumber  2


ReprojectShader g_reprojectShader;
TranShader g_transShader;
MergeShader g_mergeShader;
TexShader g_texShader;
GbufferShader g_gBufferShader;
reflectShader g_reflectionShader;
BlendShader g_blendShader;


#include "glossyScene.h"
glossyScene t_scene;

void cameraControl(int,CCamera&);



/*
posPara posArray[] = 
{
	//{make_float3(16.033649,38.942272,15.414365  ),make_float3(22.289124,34.077927,9.314583 )},
	//{make_float3(20.541248,38.556011,16.854353  ),make_float3(24.467501,33.042343,9.493363 )},
	//{make_float3(18.591887,38.901070,18.539888  ),make_float3(22.336548,34.001892,10.667604 )},
	//{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},

	//{make_float3(24.898901,45.217281,-10.226630  ),make_float3(25.289194,38.099701,-3.213274 )},
	//	{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},
	//{make_float3(19.777601,40.256077,18.724140  ),make_float3(21.469124,33.352386,11.690137 )},
	//{make_float3(21.208389,43.804527,-13.503925  ),make_float3(24.115210,37.705105,-6.132097 )},
	//{make_float3(18.743200,38.181194,-3.370136  ),make_float3(23.238724,31.557341,2.622727 )},
	//{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},
	//{make_float3(24.302208,48.071339,26.106796  ),make_float3(23.576305,42.725662,17.686817 )},
	//{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.228128,47.533394,25.267078 )},
	//{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.231396,47.535080,25.265717 )},
	//{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )},
	//{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )},

	//1st
	//	{make_float3(-2.403542,34.498703,37.004547  ),make_float3(0.809388,32.024666,27.863651 )},//
	//Burning add
	//{make_float3(12.813933,30.045429,23.866928),make_float3(16.026863,27.571392,14.726033)},
	{make_float3(-2.403542,34.498703,37.004547  ),make_float3(3.482440,31.107332,29.666050 )},//2
	{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},//4
	{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},//5

	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7
	//10
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7

	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7

	{make_float3(13.948970,43.934372,17.002832  ),make_float3(18.528547,37.866623,10.505962 )},//14
	//{make_float3(4.111155,44.597073,8.203527  ),make_float3(12.948935,40.556053,5.844954 )},
	//{make_float3(3.722486,49.674381,28.524050  ),make_float3(10.202328,45.074345,22.453533 )},//7
	{make_float3(6.768429,44.201805,11.434514  ),make_float3(7.580947,43.711884,11.118615 )},
	{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )}, //16
	{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )},  //18
	//{make_float3(21.208389,43.804527,-13.503925  ),make_float3(24.115210,37.705105,-6.132097 )},  //14

	//{make_float3(8.996864,46.504173,-18.891628  ),make_float3(14.546986,39.261303,-14.800542 )},//16
	//{make_float3(-0.140227,33.588104,3.662050  ),make_float3(8.603185,30.805325,-0.314001 )},  //18
	//{make_float3(-0.140227,33.588104,3.662050  ),make_float3(1.301753,30.177917,-5.627264 )},    //1000
	//{make_float3(-0.140227,33.588104,3.662050  ),make_float3(1.301753,30.177917,-5.627264 )},    //1000

	//{make_float3(-19.653139,33.588104,-3.880467  ),make_float3(-14.104563,29.767727,-11.270871 )},   //800
	{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )},//22
	{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )}//25

};*/


posPara posArray[] = 
{
	//{make_float3(16.033649,38.942272,15.414365  ),make_float3(22.289124,34.077927,9.314583 )},
	//{make_float3(20.541248,38.556011,16.854353  ),make_float3(24.467501,33.042343,9.493363 )},
	//{make_float3(18.591887,38.901070,18.539888  ),make_float3(22.336548,34.001892,10.667604 )},
	//{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},

	//{make_float3(24.898901,45.217281,-10.226630  ),make_float3(25.289194,38.099701,-3.213274 )},
	//	{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},
	//{make_float3(19.777601,40.256077,18.724140  ),make_float3(21.469124,33.352386,11.690137 )},
	//{make_float3(21.208389,43.804527,-13.503925  ),make_float3(24.115210,37.705105,-6.132097 )},
	//{make_float3(18.743200,38.181194,-3.370136  ),make_float3(23.238724,31.557341,2.622727 )},
	//{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},
	//{make_float3(24.302208,48.071339,26.106796  ),make_float3(23.576305,42.725662,17.686817 )},
	//{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.228128,47.533394,25.267078 )},
	//{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.231396,47.535080,25.265717 )},
	//{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )},
	//{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )},

	//1st
	//	{make_float3(-2.403542,34.498703,37.004547  ),make_float3(0.809388,32.024666,27.863651 )},//
	//Burning add
	//{make_float3(12.813933,30.045429,23.866928),make_float3(16.026863,27.571392,14.726033)},
	{make_float3(-23.214741,132.878403,104.965492  ),make_float3(-23.922035,132.399780,105.471191 )},
	{make_float3(-64.204552,128.025604,54.879829  ),make_float3(-64.709442,127.616676,55.533115 )},
	{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},//5

	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7
	//10
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7

	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7

	{make_float3(13.948970,43.934372,17.002832  ),make_float3(18.528547,37.866623,10.505962 )},//14
	//{make_float3(4.111155,44.597073,8.203527  ),make_float3(12.948935,40.556053,5.844954 )},
	//{make_float3(3.722486,49.674381,28.524050  ),make_float3(10.202328,45.074345,22.453533 )},//7
	{make_float3(6.768429,44.201805,11.434514  ),make_float3(7.580947,43.711884,11.118615 )},
	{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )}, //16
	{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )},  //18
	//{make_float3(21.208389,43.804527,-13.503925  ),make_float3(24.115210,37.705105,-6.132097 )},  //14

	//{make_float3(8.996864,46.504173,-18.891628  ),make_float3(14.546986,39.261303,-14.800542 )},//16
	//{make_float3(-0.140227,33.588104,3.662050  ),make_float3(8.603185,30.805325,-0.314001 )},  //18
	//{make_float3(-0.140227,33.588104,3.662050  ),make_float3(1.301753,30.177917,-5.627264 )},    //1000
	//{make_float3(-0.140227,33.588104,3.662050  ),make_float3(1.301753,30.177917,-5.627264 )},    //1000

	//{make_float3(-19.653139,33.588104,-3.880467  ),make_float3(-14.104563,29.767727,-11.270871 )},   //800
	{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )},//22
	{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )}//25

};


//float k = 900/25.0;
float k = 1800/25.0;
float kk[13]={2*k,4*k,5*k,7*k,10*k,13*k,15*k,19*k,21*k,23*k,25*k,25*k,25*k};
//float kk[12]={2*k,3*k,5*k,8*k,11*k,13*k,17*k,19*k,21*k,23*k,23*k,23*k};//,25*k,25*k,25*k};
int currentTime  = 9;
int currentTime2 = 9;
int OptixFrame;
FPS fcount(CountTime);


GLuint             normalTex;
GLuint             worldSpaceTex;
FramebufferObject* worldSpaceNormalFBO;

unsigned int warmup_frames = 10u, timed_frames = 10u;
unsigned int initialWindowWidth  = 1024;
unsigned int initialWindowHeight = 1024;

int logReflectionSamplingRate = 0;

GLuint reflectionMapPBO;
GLuint VectorPBO;
GLuint LastVecPBO;
GLuint testPBO;

GLuint VecorTexture;


nv::matrix4f TMVPMat;

bool fixedCamera = false;
nv::matrix4f fixedCameraMatrix;
bool drawFps = true;
bool stat_breakdown = true;

// CG
CGcontext cgContext;
CGeffect  cgEffect;
CGtechnique cgTechniqueWorldPosNormal;
CGtechnique cgTechniqueGlossyReflections;
//My Technique
CGtechnique cgReProjectTechnique;
CGtechnique cgTechniqueTranMap;

//glsl]
//uniform
GLuint TransVecTexUniform;
GLuint RefelctTexUniform;

//Trans shader Uniform
GLuint TranWorPosTexUniform;
GLuint TranRePosTexUniform;

GLuint LastMVP;
GLuint NewWorldPosTex;
GLuint LastWorldPosTex;
GLuint RefelctTex;

GLuint MergeComputTex,MergeOptixTex;


// OptiX
unsigned int traceWidth, traceHeight;
CGparameter cgBlendFactor;
CGparameter cgCameraMVPParam;
CGparameter cgCameraModelViewParam;
CGparameter cgCameraModelViewInvParem;
CGparameter cgWorldViewProjParam;
CGparameter cgWorldViewInvParam;
CGparameter cgWorldViewParam;
CGparameter cgGlossinessParam;
CGparameter cgReflectionMapParam;
CGparameter cgWorldPosMapParam;
CGparameter cgNormalMapParam;
CGparameter cgDoISGParam;
CGparameter cgDiffuseTexParam;
CGparameter cgInvSceneScaleParam;
CGparameter cgLightPosParam;
CGparameter cgDoReflectionsParam;
CGparameter cgIsModelParam;
CGparameter cgIsWallParam;
CGparameter cgDiffuseColor;
CGparameter cgIsTex;
CGparameter cgShiness;
CGparameter cgReflectVale;
CGparameter cgCameraPosition;

CGparameter cgCudaTransTexParam;

CGparameter cgModelIdParam;

CGparameter NewWorldPosTexParam;
CGparameter LastEffectTexParam;
CGparameter lastMVPParam;


optix::Context        rtContext;
optix::Buffer         rtReflectionBuffer;
optix::Buffer         rtLights;

//Buffer
optix::Buffer PixelBuffer;

optix::TextureSampler rtNormalTexture;
optix::TextureSampler rtLastReflectTexture;
optix::TextureSampler rtWorldSpaceTexture;

std::string CGFX_PATH;
std::string GROUNDTEX_PATH;
std::string WALLTEX_PATH;
//My Fbo For Render

Fbo currentGbuffer(2,rasterWidth,rasterHeight);
Fbo TransMapFbo(1,rasterWidth,rasterWidth);
Fbo FinalEffectFbo(1,rasterWidth,rasterWidth);
Fbo MergeEffectFbo(1,rasterWidth,rasterWidth);
Fbo refGbuffer(2,rasterWidth,rasterWidth);

//New TEXTURE
GLuint New_Tex;


bool animate = true;
bool doISG  = true;
bool zUp = false;
bool doReflections = true;
bool stripNormals = false;
bool g_Recordpic = false;
float scene_epsilon = 1e-2f;

void cgErrorCallback()
{
	CGerror lastError = cgGetError();
	if(lastError)
	{
		printf("%s\n", cgGetErrorString(lastError));
		printf("%s\n", cgGetLastListing(cgContext));
		exit(1);
	}
}
void getEffectParam( CGeffect effect,
					const char* semantic,
					CGparameter* param )
{
	(*param) = cgGetEffectParameterBySemantic(effect, semantic);
	if (!(*param))
	{
		std::cerr << "getCGParam: Couldn't find parameter with " << semantic << " semantic\n";
		exit(1);
	}
}
//init Cuda
void init_cuda(int argc,char**argv)
{

	findCudaDevice(argc,(const char**) argv);
    //绑定opengl纹理到cuda
	poxCudaTex.set(refGbuffer.getTexture(0),rasterWidth,rasterHeight,worldPosRef_t);
	normalCudaTex.set(refGbuffer.getTexture(1),rasterWidth,rasterHeight,worldNormalRef_t);
	reflectCudaTex.set(reflectionMapTex,rasterWidth,rasterHeight,reflecionRef_t);
	
	
	poxCudaTex.init();
	normalCudaTex.init();
	reflectCudaTex.init();
	//绑定opengl PBO到cuda
	
	vectorCudaArray.set(rasterWidth,rasterHeight,float4_t);
	lastCudaArray.set(rasterWidth,rasterHeight,float2_t);
	vectorCudaArray.init();
	lastCudaArray.init();



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


void IniteMyVBO(){

	//float2 Vertices[1024 * 1024 *6];
	float2 *Vertices;
	Vertices = new float2[rasterWidth *rasterHeight * 6];
	int PointId = 0;
	for(int x = 0;x<rasterWidth-1;x++)
		for(int y = 0;y<rasterHeight-1;y++)
		{
			/*if(x==1022||y==1022)
				;
			else
				continue;*/
			Vertices[PointId++] = make_float2((float)(x+0.5-EDGEDELTA)/rasterWidth,(float)(y+0.5-EDGEDELTA)/rasterHeight);
#if DrawPoint
			;//continue;
#endif

			Vertices[PointId++] =  make_float2((float)(x+1.5+EDGEDELTA)/rasterWidth,(float)(y+0.5-EDGEDELTA)/rasterHeight);
			Vertices[PointId++] =  make_float2((float)(x+1.5+EDGEDELTA)/rasterWidth,(float)(y+1.5+EDGEDELTA)/rasterHeight);

			Vertices[PointId++] = make_float2((float)(x+0.5-EDGEDELTA)/rasterWidth,(float)(y+0.5-EDGEDELTA)/rasterHeight);
			Vertices[PointId++] =  make_float2((float)(x+1.5+EDGEDELTA)/rasterWidth,(float)(y+1.5+EDGEDELTA)/rasterHeight);
			Vertices[PointId++] =  make_float2((float)(x+0.5-EDGEDELTA)/rasterWidth,(float)(y+1.5+EDGEDELTA)/rasterHeight);

		}

		MyCallList = glGenLists(1);
		glNewList(MyCallList,GL_COMPILE);

		glPointSize(1);
#if DrawPoint
		glBegin(GL_POINTS);
#else
		glBegin(GL_TRIANGLES);
#endif
		for(int i = 0;i<PointId;i++)
			glVertex2f(Vertices[i].x,Vertices[i].y);
		glEnd();
		glEndList();
		// 		cout<<Vertices[1].x<<" "<<Vertices[1].y<<endl;
		glGenBuffers(1,&MyVBO);
		glBindBuffer(GL_ARRAY_BUFFER,MyVBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float2)* rasterWidth *rasterHeight * 6,(GLvoid*)Vertices,GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER,0);

		delete [] Vertices;




}


void init_cg()
{
	
	cgContext = cgCreateContext();
	cgSetErrorCallback(cgErrorCallback);
	cgGLSetDebugMode( CG_FALSE );
	cgSetParameterSettingMode(cgContext, CG_DEFERRED_PARAMETER_SETTING);
	cgGLRegisterStates(cgContext);

#ifdef __APPLE__
	const char* cgc_args[] = { "-DCG_FRAGMENT_PROFILE=fp40", "-DCG_VERTEX_PROFILE=vp40", NULL };
#else
	const char* cgc_args[] = { "-DCG_FRAGMENT_PROFILE=gp4fp", "-DCG_VERTEX_PROFILE=gp4vp", NULL };
#endif

	cgEffect = cgCreateEffectFromFile(cgContext, CGFX_PATH.c_str(), cgc_args);
	if(!cgEffect) {
		printf("CGFX error:\n %s\n", cgGetLastListing(cgContext));
		exit(-1);
	}

	cgTechniqueWorldPosNormal = cgGetNamedTechnique(cgEffect, "WorldPosNormal");
	if(!cgTechniqueWorldPosNormal) {
		printf("CGFX error:\n %s\n", cgGetLastListing(cgContext));
		exit(-1);
	}

	cgTechniqueGlossyReflections = cgGetNamedTechnique(cgEffect, "GlossyReflections");
	if(!cgTechniqueGlossyReflections) {
		printf("CGFX error:\n %s\n", cgGetLastListing(cgContext));
		exit(-1);
	}

	cgReProjectTechnique = cgGetNamedTechnique(cgEffect,"ReProject");
	if(!cgReProjectTechnique){
		printf("CGFX error:\n %s\n", cgGetLastListing(cgContext));
		exit(-1);
	}
	//  
	cgTechniqueTranMap = cgGetNamedTechnique(cgEffect, "TransMap");
	if(!cgTechniqueGlossyReflections) {
		printf("CGFX error:\n %s\n", cgGetLastListing(cgContext));
		exit(-1);
	}


	getEffectParam(cgEffect, "mvpMat" ,&cgCameraMVPParam);
	getEffectParam(cgEffect, "modelViewMat", &cgCameraModelViewParam);
	getEffectParam(cgEffect, "modelViewInv", &cgCameraModelViewInvParem);

	getEffectParam(cgEffect, "WorldViewProj", &cgWorldViewProjParam);
	getEffectParam(cgEffect, "WorldViewInv", &cgWorldViewInvParam);
	getEffectParam(cgEffect, "WorldView", &cgWorldViewParam);
	getEffectParam(cgEffect, "LightPos", &cgLightPosParam);
	getEffectParam(cgEffect, "ReflectionMap", &cgReflectionMapParam);
	getEffectParam(cgEffect, "WorldPosMap", &cgWorldPosMapParam);
	getEffectParam(cgEffect, "NormalMap", &cgNormalMapParam);
	getEffectParam(cgEffect, "DiffuseTex", &cgDiffuseTexParam);
	getEffectParam(cgEffect, "InvSceneScale", &cgInvSceneScaleParam);
	getEffectParam(cgEffect, "Glossiness", &cgGlossinessParam);
	getEffectParam(cgEffect, "DiffuseColor", &cgDiffuseColor);
	getEffectParam(cgEffect, "isTex", &cgIsTex);
	getEffectParam(cgEffect, "shiness", &cgShiness);
	getEffectParam(cgEffect,"reflectValue",&cgReflectVale);
	getEffectParam(cgEffect, "DoISG", &cgDoISGParam);
	getEffectParam(cgEffect,"cameraPosition",&cgCameraPosition);
	getEffectParam(cgEffect,"BlendFactorTex",&cgBlendFactor);

	getEffectParam(cgEffect,"TheModelId",&cgModelIdParam);
	// getEffectParam(cgEffect,"CudaTransMap",&cgCudaTransTexParam);
	//cgBlendFactor
	cgSetParameter1i(cgDoISGParam, (int)doISG);

	getEffectParam(cgEffect, "DoReflections", &cgDoReflectionsParam);
	cgSetParameter1i(cgDoReflectionsParam, (int)doReflections);

	getEffectParam(cgEffect, "IsModel", &cgIsModelParam);
	getEffectParam(cgEffect, "IsWall", &cgIsWallParam);

	//For Reprojection 

	getEffectParam(cgEffect,"NewWorldPosMap",&NewWorldPosTexParam);
	getEffectParam(cgEffect,"LastEffectMap",&LastEffectTexParam);
	getEffectParam(cgEffect,"LastMVP",&lastMVPParam);
	g_testShader.initCG(cgContext);
	
}
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

	init_cg();

	// create PBO's
	glGenBuffers(1, &reflectionMapPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, rasterHeight*rasterWidth*sizeof(float4), 0, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);


	glGenBuffers(1,&VectorPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,VectorPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER,rasterHeight*rasterWidth*sizeof(float4),0,GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);

	glGenBuffers(1,&LastVecPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,LastVecPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER,rasterHeight*rasterWidth*sizeof(float2),0,GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);

	glGenBuffers(1,&testPBO);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,testPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER,rasterHeight*rasterWidth*sizeof(float4),0,GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);

	// setup FBO's

	currentGbuffer.init();
	FinalEffectFbo.init();
	//TransMapFbo.init();
	TransMapFbo.init(GL_LINEAR);
	refGbuffer.init();

	MergeEffectFbo.init();
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


	//生成所需的reflcet纹理
	glGenTextures(1, &New_Tex);

	glBindTexture(GL_TEXTURE_2D, New_Tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, rasterWidth, rasterHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);



	// cgGLSetTextureParameter(cgCudaTransTexParam, VecorTexture);


	//载入glslshader
	g_transShader.init();
	g_reprojectShader.init();
	g_mergeShader.init();
	g_reprojectShader.setClearColor(viewIndepentdentMissColor);
	g_texShader.init();
	g_blendShader.init();
	g_gBufferShader.init();
	g_reflectionShader.init();
	g_reflectionShader.setWindowSize(rasterWidth,rasterHeight);
	/*
#if DrawPoint
	myTransShader.loadShader("Shader/Trans_Point.vert",0,"Shader/Trans_Point.frag");
#else
	myTransShader.loadShader("Shader/Trans.vert","Shader/Trans.geom","Shader/Trans.frag");
#endif
	TransVecTexUniform = myTransShader.getUniform("TransVecTex");
	RefelctTexUniform = myTransShader.getUniform("RefelctTex");
	TranWorPosTexUniform = myTransShader.getUniform("WorldPosTex");
	TranRePosTexUniform = myTransShader.getUniform("RePosTex");
	*/
	/*
	RepjectShader.loadShader("shader/Reprojec.vert",0,"Shader/Reprojec.frag");
	NewWorldPosTex = RepjectShader.getUniform("NewWorldPos");
	RefelctTex = RepjectShader.getUniform("Reflection");
	LastMVP =  RepjectShader.getUniform("LastMVP");
	LastWorldPosTex = RepjectShader.getUniform("LastWorldPos");
	*/
/*
	MergeShader.loadShader("Shader/Merge.vert",0,"Shader/Merge.frag");
	MergeOptixTex = MergeShader.getUniform("OptixTex");
	MergeComputTex = MergeShader.getUniform("ComputeTex");
	Merge_FrameCount = MergeShader.getUniform("FrameCount");
	*/
	//生成bufferdraw
#if SpeedUp
	IniteMyVBO();
#endif




	//setMatrixMode



	//原来的optix Fbo
	/* worldSpaceNormalFBO = new FramebufferObject();

	GLuint worldSpaceDepthTex;
	glGenTextures(1, &worldSpaceTex);
	glGenTextures(1, &normalTex);
	glGenTextures(1, &worldSpaceDepthTex);

	glBindTexture(GL_TEXTURE_2D, worldSpaceTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, 32, 32, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	cgGLSetTextureParameter(cgWorldPosMapParam, worldSpaceTex);

	glBindTexture(GL_TEXTURE_2D, normalTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, 32, 32, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	cgGLSetTextureParameter(cgNormalMapParam, normalTex);

	glBindTexture(GL_TEXTURE_2D, worldSpaceDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float BorderColor[4] = { std::numeric_limits<float>::max() };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BorderColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8_EXT, 32, 32, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	worldSpaceNormalFBO->AttachTexture( GL_TEXTURE_2D, worldSpaceTex, GL_COLOR_ATTACHMENT0_EXT );
	worldSpaceNormalFBO->AttachTexture( GL_TEXTURE_2D, normalTex, GL_COLOR_ATTACHMENT1_EXT );
	worldSpaceNormalFBO->AttachTexture( GL_TEXTURE_2D, worldSpaceDepthTex, GL_DEPTH_ATTACHMENT_EXT );
	if(!worldSpaceNormalFBO->IsValid()) {
	printf("FBO is incomplete\n");
	exit(-1);
	}*/

	// and reflection map texture
	glGenTextures(1, &reflectionMapTex);
	glBindTexture(GL_TEXTURE_2D, reflectionMapTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
#if MerGe
	cgGLSetTextureParameter(cgReflectionMapParam, FinalEffectFbo.getTexture(0));
#else
	cgGLSetTextureParameter(cgReflectionMapParam,FinalEffectFbo.getTexture(0));

#endif

	glGenFramebuffersEXT(1, &sampleFbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sampleFbo);
	glDrawBuffers(1, buffers);

	glGenTextures (1, &sampleTex);
	glBindTexture(GL_TEXTURE_2D, sampleTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, initialWindowHeight, initialWindowWidth, 0,	GL_RGBA, GL_FLOAT, 0);
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, sampleTex, 0);
	glGenRenderbuffersEXT(1,&depthTex);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,depthTex);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT, initialWindowHeight, initialWindowWidth);
	//将深度缓冲与FBO绑定
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,depthTex);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// load ground texture
	/* PPMLoader ground_ppm(GROUNDTEX_PATH);
	if(ground_ppm.failed()) {
	std::cerr << "Could not load PPM file " << GROUNDTEX_PATH << '\n';
	exit(1);
	}
	glGenTextures(1, &groundTex);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, ground_ppm.width(), ground_ppm.height(), GL_RGB, GL_UNSIGNED_BYTE, ground_ppm.raster());
	glBindTexture(GL_TEXTURE_2D, 0);

	// load wall texture
	PPMLoader wall_ppm(WALLTEX_PATH);
	if(wall_ppm.failed()) {
	std::cerr << "Could not load PPM file " << WALLTEX_PATH << '\n';
	exit(1);
	}
	glGenTextures(1, &wallTex);
	glBindTexture(GL_TEXTURE_2D, wallTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, wall_ppm.width(), wall_ppm.height(), GL_RGB, GL_UNSIGNED_BYTE, wall_ppm.raster());
	glBindTexture(GL_TEXTURE_2D, 0);

	// 2x2 texture for gold
	glGenTextures(1, &goldTex);
	glBindTexture(GL_TEXTURE_2D, goldTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	float3 gold_data[4];
	for(int i = 0; i < 4; ++i) gold_data[i] = make_float3(.05f,.92f,.0f);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, 2, 2, GL_RGB, GL_FLOAT, gold_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	*/
	CHECK_ERRORS();
}

void init_scene(const char* model_filename)
{
	printf("init scence\n");
	t_scene.init();
	float diag;
	//model->computeBoundingBox(modelBBMin, modelBBMax);
	modelBBMin = MyGeometry::modelBBMin,modelBBMax = MyGeometry::modelBBMax;
	modelBBCenter = (modelBBMin + modelBBMax) * 0.5;

	diag = nv::length(modelBBMax - modelBBMin);
	cgSetParameter1f(cgInvSceneScaleParam, 1.f/diag);

	cgSetParameter3f(   cgLightPosParam,
		t_scene.getLightPos().x,
		t_scene.getLightPos().y,
		t_scene.getLightPos().z );

	/* manipulator.setTrackballActivate(GLUT_LEFT_BUTTON, 0);
	manipulator.setDollyActivate( GLUT_RIGHT_BUTTON, 0);
	manipulator.setPanActivate( GLUT_MIDDLE_BUTTON, 0);

	manipulator.setDollyScale( diag*0.02f );
	manipulator.setPanScale( diag*0.02f );
	manipulator.setDollyPosition(-diag);
	manipulator.rotate( nv::vec3f(0.f,1.f,0.f), 3.f/4.f * 3.14159f );
	manipulator.rotate( nv::vec3f(1.f,0.f,0.f), .4f );
	// manipulator.setCenterOfRotation(nv::vec3f(178,21,156));
	*/
	CLoad3DS* loader=new(CLoad3DS);
	loader->BuildTexture(blendFactorPath, blendFactorTex);
	delete loader;
	cgGLSetTextureParameter(cgBlendFactor, blendFactorTex);
	// g_currentCamera.PositionCamera(-40, 54, -151,    -22.8827, 42.5, -180.212,     0, 1, 0);
	// g_currentCamera.navigate(posArray[0],posArray[1],currentTime,0,kk[0]);
	// bathroom scene
	// 

	//记得换回来
	//g_refCamera.PositionCamera(-2.403542,34.498703,37.004547 ,0.809388,32.024666,27.863651,0,1,0);
	//g_currentCamera.PositionCamera(-10.403542,34.498703,37.004547 ,0.809388,32.024666,27.863651,0,1,0);


	cameraControl(currentTime2,g_currentCamera);
	//g_currentCamera.PositionCamera(-2.403542,34.498703,37.004547 ,0.809388,32.024666,27.863651,0,1,0);
	//g_refCamera.PositionCamera(-10.403542,34.498703,37.004547 ,0.809388,32.024666,27.863651,0,1,0);
	// simple scene
	//g_currentCamera.PositionCamera(27.2195,52.8272,141.653, 0, 0, 0, 0, 1, 0);
	// g_currentCamera.PositionCamera(-2.403542,34.498703,37.004547,3.482440,31.107332,29.666050,0,1,0);
	//
	//Optix_Camare.PositionCamera( -1.5515 ,35.3627 ,37.3552 ,1.5802 ,32.7854 ,28.2370,0,1,0);
	/****************** bath room scene*****************************/
	//Optix_Camare.navigate(posArray[1],posArray[2],1,0,36);


	/*********************simple scene**************************/
	//Optix_Camare.PositionCamera(29.2195,53.8272,140.653,0, 0, 0, 0, 1, 0);
	//Optix_Camare.PositionCamera(28.2195,51.8272,141.653, 0, 0.1, 0, 0, 1, 0);
	//Optix_Camare.PositionCamera(12.813933,30.045429,23.866928 ,16.026863,27.571392,14.726033,0,1,0);

	//Optix_Camare.CoutCamera();
	//Optix_Camare.navigate(posArray[0],posArray[1],currentTime,0,kk[0]);



	//float diag1 = nv::length(modelBBMax - modelBBMin);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1.0, diag*.01, diag*100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	CHECK_ERRORS();
}

std::string ptxpath( const std::string& base )
{
	return std::string(sutilSamplesPtxDir()) + "/isgReflections_generated_" + base + ".ptx";
}

optix::Geometry createParallelogram( optix::Context context,
									const nv::vec3f& anchor,
									const nv::vec3f& offset1,
									const nv::vec3f& offset2)
{
	optix::Geometry parallelogram = context->createGeometry();
	parallelogram->setPrimitiveCount( 1u );
	parallelogram->setIntersectionProgram( context->createProgramFromPTXFile( ptxpath("parallelogram.cu"), "intersect" ));
	parallelogram->setBoundingBoxProgram( context->createProgramFromPTXFile(  ptxpath("parallelogram.cu"), "bounds" ));

	nv::vec3f normal = normalize( cross( offset1, offset2 ) );
	float d = dot( normal, anchor );
	nv::vec4f plane = nv::vec4f( normal, d );

	nv::vec3f v1 = offset1 / dot( offset1, offset1 );
	nv::vec3f v2 = offset2 / dot( offset2, offset2 );

	parallelogram["plane"]->setFloat( plane.x, plane.y, plane.z, plane.w );
	parallelogram["anchor"]->setFloat( anchor.x, anchor.y, anchor.z );
	parallelogram["v1"]->setFloat( v1.x, v1.y, v1.z );
	parallelogram["v2"]->setFloat( v2.x, v2.y, v2.z );

	return parallelogram;
}
extern unsigned int  *g_PixelPos;
void init_optix()
{
	try 
	{
		//  printf("111");
		rtContext = optix::Context::create();
		rtContext->setRayTypeCount(2);
		rtContext->setEntryPointCount(2);

		rtContext["radiance_ray_type"]->setUint(0u);
		rtContext["shadow_ray_type"]->setUint(1u);
		rtContext["scene_epsilon"]->setFloat(scene_epsilon);
#if MerGe

		// PixelBuffer = rtContext->createBufferForCUDA(RT_BUFFER_INPUT,RT_FORMAT_UNSIGNED_INT,1024*1024);//PixelsSum);

		// rtContext["Pixels_Buffer"]->setBuffer(PixelBuffer);
#endif


		//

		PixelBuffer = rtContext->createBufferForCUDA(RT_BUFFER_INPUT,RT_FORMAT_UNSIGNED_INT,1024*1024);//PixelsSum);
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

		rtLights = rtContext->createBuffer(RT_BUFFER_INPUT);
		rtLights->setSize(1);
		rtLights->setFormat(RT_FORMAT_USER);
		rtLights->setElementSize(sizeof(BasicLight));
		rtContext["lights"]->setBuffer(rtLights);

		rtContext->setRayGenerationProgram( 0, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_request" ) );
		rtContext->setExceptionProgram( 0, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_exception" ) );
		rtContext->setMissProgram( 0, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_miss" ) );

		rtContext->setRayGenerationProgram(1, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "addition_request" ) );
		rtContext->setExceptionProgram( 1, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_exception" ) );
		rtContext->setMissProgram( 1, rtContext->createProgramFromPTXFile( ptxpath("reflection_request_isg.cu"), "reflection_miss" ) );

		// printf("222");
		MyGeometry::ms_rtContext = rtContext;
		MyMeterial::ms_rtContext = rtContext;
		MyGeometry::mspCgisTex = &cgIsTex;
		MyGeometry::mspCgshiness = &cgShiness;
		MyGeometry::mspCgIsModel = &cgIsModelParam;
		MyGeometry::mspCgIsWall = &cgIsWallParam;
		MyGeometry::mspCgDiffuseColor = &cgDiffuseColor;
		MyGeometry::msgCgDiffuseTexParam = &cgDiffuseTexParam;
		MyGeometry::msgCgreflectVale = &cgReflectVale;
		//optix::Geometry sofaGemetry = sofa.getOptixGeometry(rtContext);
		t_scene.setOptix(&rtContext);
		t_scene.optixInit();
		rtContext["max_depth"]->setUint(2u);
		rtContext["lightPos"]->set3fv((const float*)&make_float3(t_scene.getLightPos().x,
			t_scene.getLightPos().y,
			t_scene.getLightPos().z));

		rtContext->setStackSize(1850);
		rtContext->validate();
		printf("4444");


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
	g_reprojectShader.setParemeter(TransMapFbo.getTexture(0),currentGbuffer.getTexture(0),refGbuffer.getTexture(0), g_refCamera.getMvpMat());
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
		g_currentCamera.Look();
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
	cgSetParameter3f(cgCameraPosition,g_currentCamera.Position().x,g_currentCamera.Position().y,g_currentCamera.Position().z);
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
		g_refCamera.Look();
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
	cgSetParameter3f(cgCameraPosition,g_refCamera.Position().x,g_refCamera.Position().y,g_refCamera.Position().z);
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
	t_scene.draw_model(shader);


	CHECK_ERRORS();
}
void draw_scene(CGtechnique& current_technique, CCamera * pCamera)
{
	// glLoadIdentity();
	if(fixedCamera) 
	{
		glMultMatrixf(fixedCameraMatrix.get_value());
	} else {
		// manipulator.applyTransform();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		pCamera->Look();
	}

	cgGLSetMatrixParameterfc(cgCameraMVPParam,pCamera->getMvpMat());
	cgGLSetMatrixParameterfc(cgCameraModelViewParam,pCamera->getModelViewMat());
	cgGLSetMatrixParameterfc(cgCameraModelViewInvParem,pCamera->getModelViewInvMat());



	cgSetParameter1i(cgIsWallParam, 0);
	cgSetParameter1i(cgIsModelParam, 1);
	cgSetParameter3f(cgDiffuseColor,0.0f,0.0f,0.0f);
	cgSetParameter3f(cgCameraPosition,pCamera->Position().x,pCamera->Position().y,pCamera->Position().z);
	cgGLSetTextureParameter(cgDiffuseTexParam, woodTex);
	t_scene.draw_model(current_technique);
	CHECK_ERRORS();
}
extern unsigned int  *g_PixelPos;
void addtionalTracing(int pixelNum)
{

	/*  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, traceWidth*traceHeight*sizeof(float4), 0, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	*/

	float temp = sqrt((float) pixelNum);
	int length = ceil(temp);

	PixelBuffer->setDevicePointer(0, (CUdeviceptr)g_PixelPos);
	rtContext["PixelNum"]->setInt(pixelNum);
	rtContext["PixelWidth"]->setInt(length);
	rtContext["eye_pos"]->setFloat(g_currentCamera.Position().x, g_currentCamera.Position().y, g_currentCamera.Position().z);
	BasicLight* lightData = static_cast<BasicLight*>(rtLights->map());
	lightData[0].pos = make_float3(lightPos.x, lightPos.y, lightPos.z);
	lightData[0].color.x = 1;
	lightData[0].color.y = 1;
	lightData[0].color.z = 1;
	lightData[0].casts_shadow = 0;
	rtLights->unmap();


	cameraControl(currentTime2,g_currentCamera);

	currentGbuffer.begin();
	draw_scene(cgTechniqueWorldPosNormal,&g_currentCamera);
	currentGbuffer.end();
	//currentGbuffer.SaveBMP("worPos.
	try
	{
		rtContext->launch(1, length, length);
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
	SaveBMP("./test/newtrace.bmp", pTexture, w, h);
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
	MergeEffectFbo.end();
	cgGLSetTextureParameter(cgReflectionMapParam,MergeEffectFbo.getTexture(0));
	currentGbuffer.begin();
	draw_scene(cgTechniqueGlossyReflections,&g_currentCamera);
	currentGbuffer.end();

}
void reductionGPU()
{
	int pixelNum = 	thrustReduction(rasterWidth,rasterHeight);
	addtionalTracing(pixelNum);
	blending();
}


//Compute Vec in Cuda

void ComputeVecInCuda()
{

	float modelViewF[16],projF[16];
	//Get MVP Matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Optix_Camare.Look();
	//g_currentCamera.Look();
	g_refCamera.Look();


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

	TMVPMat = projectM * viewM;

	const float *MVPMatf = TMVPMat.get_value();


	float3 CameraPos1 = make_float3(g_refCamera.Position().x,g_refCamera.Position().y,g_refCamera.Position().z);

	//float3 CameraPos2 = make_float3(-2.403542,34.498703,37.004547);
	float3 CameraPos2 = make_float3(g_currentCamera.Position().x,g_currentCamera.Position().y,g_currentCamera.Position().z);





	TransConstData(g_refCamera.getMvpMat(),g_refCamera.getModelViewMat(),&CameraPos1,&CameraPos2);




	//MapResource 
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_WorldPos,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_WorldNormal,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_Reflect,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_VectorPBO,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cuda_LastVecPBO,0));
	vectorCudaArray.map();
	//lastCudaArray.map();

	
	poxCudaTex.map();
	normalCudaTex.map();
	reflectCudaTex.map();
	//TransGLRes2Cuda(&cudaRes_VectorPBO,&cudaRes_VectorPBO,&cudaRes_VectorPBO,&cudaRes_VectorPBO,&cuda_LastVecPBO);

	cudaPredict(rasterWidth,rasterHeight);
	
	reflectCudaTex.unmap();
	poxCudaTex.unmap();
	normalCudaTex.unmap();
	vectorCudaArray.unMap();
	//lastCudaArray.unMap();
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_WorldPos, 0));
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_WorldNormal, 0));
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_Reflect, 0));
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_VectorPBO, 0));
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cuda_LastVecPBO, 0));



};
void mapping()
{

	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_ProPixels,0));
	finalEffectCudaTex.map();
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaTest_Pixels,0));
	//checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_VectorPBO,0));
	finalEffectCudaArray.map();
	vectorCudaArray.map();
	//TransProPixel2Cuda(&cudaRes_ProPixels,&cudaTest_Pixels,&cudaRes_VectorPBO);




	reductionGPU();
	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_VectorPBO, 0));

	//checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaTest_Pixels, 0));
	///checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_ProPixels, 0));
	vectorCudaArray.unMap();
	finalEffectCudaTex.unmap();
	finalEffectCudaArray.unMap();
	



}


//Compute Vector
void ComputeVector(){
	//int x = 696;
	//int y = 640;
	int x = 373,y = 139;


	float3 *WorldPosFloat = NULL;
	float4 *ReflectFloat = NULL;

	float3 *WorldNormalFloat = NULL;

	WorldPosFloat = new float3[1024*1024];
	ReflectFloat = new float4[1024*1024];
	WorldNormalFloat = new float3[1024*1024];

	memset(WorldPosFloat,0,1024*1024*sizeof(float3));
	memset(ReflectFloat,0,1024*1024*sizeof(float4));
	memset(WorldNormalFloat,0,1024*1024*sizeof(float3));

	glBindTexture(GL_TEXTURE_2D,currentGbuffer.getTexture(0));
	glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_FLOAT,WorldPosFloat);

	glBindTexture(GL_TEXTURE_2D,0);

	glBindTexture(GL_TEXTURE_2D,reflectionMapTex);
	glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,ReflectFloat);
	glBindTexture(GL_TEXTURE_2D,0);

	glBindTexture(GL_TEXTURE_2D,currentGbuffer.getTexture(1));
	glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_FLOAT,WorldNormalFloat);

	glBindTexture(GL_TEXTURE_2D,0);

	float3 WorldPos = WorldPosFloat[(y*1024) + x];
	float3 WorldNormal = normalize(WorldNormalFloat[(y*1024) + x]);
	float4 tmpReflect = ReflectFloat[(y*1024) + x ];

	//float3 ReflectPos = make_float3(tmpReflect.x,tmpReflect.y,tmpReflect.z);
	float ReflectDis = tmpReflect.w;

	float3 CameraPos1 = make_float3(-2.403542,34.498703,37.004547);

	float3 CameraPos2 = make_float3(g_currentCamera.Position().x,g_currentCamera.Position().y,g_currentCamera.Position().z);

	float3 CamerVec = normalize(CameraPos2 - CameraPos1);

	float3 LookVec = normalize(WorldPos-CameraPos1);

	float3 reflectVec = normalize(reflect(LookVec,WorldNormal));



	//被反射物体坐标
	float3 TmpRePos = WorldPos + reflectVec * ReflectDis;


	//求镜像点位置
	float cosTheta = abs(dot(reflectVec,WorldNormal));

	float3 RefRealPos = ReflectDis * cosTheta * 2 *(-1) * WorldNormal + TmpRePos;


	//float PlaneD = -(WorldNormal.x * WorldPos.x + WorldNormal.y + WorldPos.y + WorldNormal.z * WorldPos.z);

	float DisEye2Plane = abs(dot(CameraPos2 - WorldPos,WorldNormal));

	float3 VecEye2Ref = normalize(RefRealPos - CameraPos2);
	float CosTheta2 = abs(dot(VecEye2Ref,WorldNormal));

	float3 FinalPos = CameraPos2 + (DisEye2Plane/CosTheta2)* VecEye2Ref;



	//matrix4f MVP;
	float ViewMat[16],MVPFloat[16];
	//MVPFloat = new float[16];
	float modelViewF[16],projF[16];

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Optix_Camare.Look();
	g_currentCamera.Look();

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


	viewM.set_value(modelViewF);
	projectM.set_value(projF);


	float4 tmpPos = make_float4(RefRealPos,1.0);

	nv::vec4f tmpPosVec(FinalPos.x,FinalPos.y,FinalPos.z,1.0);

	//	nv::vec4f ProPos = projectM* viewM*nv::vec4f(RefRealPos.x,RefRealPos.y,RefRealPos.z,1) ;
	nv::vec4f ProPos = projectM* viewM*tmpPosVec ;

	ProPos.x = ProPos.x / ProPos.w;
	ProPos.y = ProPos.y / ProPos.w;

	//project位置
	ProPos.x = (ProPos.x * 0.5 +0.5) * 1024;
	ProPos.y = (ProPos.y * 0.5 +0.5) * 1024;

	float2 NowProPos = make_float2(ProPos.x ,ProPos.y);

	int2 TmpUv = make_int2(x,y);

	float2 MoveVec = NowProPos - make_float2(TmpUv.x,TmpUv.y);

	int IterTime = 0;
	while(IterTime < 400 && dot(MoveVec,MoveVec)>=2){
		int2 TmpUv1,TmpUv2, TmpUv3;
		TmpUv3 = TmpUv;
		if(MoveVec.x >0){
			TmpUv1 = make_int2(TmpUv.x + 1,TmpUv.y);
			TmpUv3.x = TmpUv3.x + 1;
		}
		else{
			TmpUv1 = make_int2(TmpUv.x - 1,TmpUv.y);
			TmpUv3.x = TmpUv3.x - 1;

		}
		if(MoveVec.y>0){
			TmpUv2 = make_int2(TmpUv.x,TmpUv.y+1);
			TmpUv3.y += 1;
		}
		else{
			TmpUv2 = make_int2(TmpUv.x,TmpUv.y-1);
			TmpUv3.y -= 1;
		}

		//各个采样点对应的WorPos
		float3 WorldPos1 = WorldPosFloat[(TmpUv1.y*1024)+TmpUv1.x];
		float3 WorldPos2 = WorldPosFloat[(TmpUv2.y*1024)+TmpUv2.x];
		float3 WorldPos3 = WorldPosFloat[(TmpUv3.y * 1024) + TmpUv3.x];

		//求相机与采样点组成平面的法线
		float3 CamWorldPlaneNormal = normalize(cross(CamerVec,normalize(CameraPos1- RefRealPos)));
		//求相机轨迹所在平面的法线
		float3 CameraVecPlaneNormal = normalize(cross(CamWorldPlaneNormal,CamerVec));

		//分别求两个点与该平面的交点
		float3 InsertPoint1 = abs(dot(CameraVecPlaneNormal,CameraPos1 - RefRealPos)/dot(CameraVecPlaneNormal,normalize(WorldPos1-RefRealPos))) * normalize(WorldPos1-RefRealPos) + RefRealPos;
		float3 InsertPoint2 = abs(dot(CameraVecPlaneNormal,CameraPos1 - RefRealPos)/dot(CameraVecPlaneNormal,normalize(WorldPos2-RefRealPos))) * normalize(WorldPos2-RefRealPos) + RefRealPos;
		float3 InsertPoint3 = abs(dot(CameraVecPlaneNormal,CameraPos1 - RefRealPos)/dot(CameraVecPlaneNormal,normalize(WorldPos3-RefRealPos))) * normalize(WorldPos3-RefRealPos) + RefRealPos;

		//求点与直线的距离
		float DisPoint2Line1,DisPoint2Line2,DisPoint2Line3;
		float TmpDis1,TmpDis2;
		TmpDis1 = dot(InsertPoint1 - CameraPos1,CamerVec);
		TmpDis2 = dot(InsertPoint1 - CameraPos1,InsertPoint1 - CameraPos1);
		DisPoint2Line1 = TmpDis2 - TmpDis1*TmpDis1 ;

		TmpDis1 = dot(InsertPoint2 - CameraPos1,CamerVec);
		TmpDis2 = dot(InsertPoint2 - CameraPos1,InsertPoint2 - CameraPos1);
		//距离较小的作为下一个采样点
		DisPoint2Line2 = TmpDis2 - TmpDis1*TmpDis1 ;

		TmpDis1 = dot(InsertPoint3 - CameraPos1,CamerVec);
		TmpDis2 = dot(InsertPoint3 - CameraPos1,InsertPoint3 - CameraPos1);
		DisPoint2Line3 = TmpDis2 - TmpDis1*TmpDis1 ;

		if( DisPoint2Line2 < DisPoint2Line1){
			TmpUv1 = TmpUv2;
			WorldPos1 = WorldPos2;
			DisPoint2Line1 = DisPoint2Line2;
		}
		if(DisPoint2Line3 < DisPoint2Line1){

			TmpUv1 = TmpUv3;
			WorldPos1 = WorldPos3;

		}

		TmpUv = TmpUv1;
		WorldPos = WorldPos1;

		//得出该点法线
		WorldNormal = WorldNormalFloat[TmpUv.y*1024+TmpUv.x];

		//求出该点的位平面的被发射物体的镜像点
		RefRealPos = abs(dot(WorldPos - TmpRePos,WorldNormal)) * (-2) * WorldNormal + TmpRePos;
		//求该镜像点与相机连线与新平面的交点
		DisEye2Plane = abs(dot(CameraPos2-WorldPos,WorldNormal));
		//视点与镜像点连线与视点到平面垂线的cos值
		CosTheta2 = abs(dot(normalize(CameraPos2 - RefRealPos),WorldNormal));
		VecEye2Ref = normalize(RefRealPos - CameraPos2);
		//求出交点
		FinalPos = CameraPos2 +  VecEye2Ref * (DisEye2Plane/CosTheta2);


		tmpPosVec = nv::vec4f(FinalPos.x,FinalPos.y,FinalPos.z,1);

		ProPos = projectM* viewM*tmpPosVec ;

		ProPos.x = ProPos.x / ProPos.w;
		ProPos.y = ProPos.y / ProPos.w;

		//project位置
		ProPos.x = (ProPos.x * 0.5 +0.5) * 1024;
		ProPos.y = (ProPos.y * 0.5 +0.5) * 1024;

		NowProPos = make_float2(ProPos.x ,ProPos.y);

		MoveVec = NowProPos - make_float2(TmpUv.x,TmpUv.y);

		IterTime++;


	}


	//printf("origin Pos is %f,%f,%f\n",ReflectPos.x,ReflectPos.y,ReflectPos.z);
	//printf("comput Pos is %f,%f,%f\n",TmpRePos.x,TmpRePos.y,TmpRePos.z);
	printf("the uv is %f,%f\n",ProPos.x,ProPos.y);






	//delete [] MVPFloat;
	delete [] WorldNormalFloat;
	delete [] WorldPosFloat;
	delete [] ReflectFloat;





}





void drawTransMap(int  optixId)
{



	// cgGLSetTextureParameter(cgReflectionMapParam, reflectionMapTex);
	// cgGLEnableTextureParameter(cgReflectionMapParam);

	//cgGLSetTextureParameter(cgCudaTransTexParam, VecorTexture);

	//CGpass pass = cgGetFirstPass(currentTech);

	//cgSetPassState(pass);

	// 
	// 	CGpass pass = cgGetFirstPass(tech);
	// 	while(pass) {
	// 		cgSetPassState(pass);
	// 		glDrawElements( GL_TRIANGLES, m_model->getCompiledIndexCount(), GL_UNSIGNED_INT, 0 );
	// 		cgResetPassState(pass);
	// 		pass = cgGetNextPass(pass);
	// 	}

	//glPointSize(5.0);


	//myTransShader.useShder();



	//glUniform1i(TransVecTexUniform,1);
	//glUniform1i(RefelctTexUniform,0);


	// 	glBindTexture(GL_TEXTURE_2D,VecorTexture);
	// 	float2 *pFloat2 = NULL;
	// 	pFloat2 = new float2[1024*1024];
	// 	memset(pFloat2, 0, 1024*1024 * sizeof(float2));
	// 
	// 	glBindTexture(GL_TEXTURE_2D, VecorTexture);//TexPosId   PboTex
	// 
	// 	glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, pFloat2);
	// 	glBindTexture(GL_TEXTURE_2D, 0);

	glClearColor(viewDependentMissColor.x,viewDependentMissColor.y,viewDependentMissColor.z,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//glViewport(0,0,1024,1024);
	// 
	// 

	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,VecorTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,reflectionMaps[optixId]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,RePosTexture);


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,currentGbuffer.getTexture(0));

	glUniform1i(TransVecTexUniform,1);
	glUniform1i(RefelctTexUniform,0);
	glUniform1i(TranWorPosTexUniform,2);
	glUniform1i(TranRePosTexUniform,3);
	*/
	RefFrame & frame = RefFrame::getFrameByIndex(OptixFrame);

	g_transShader.setParemeter(frame.getOptixTex(),VecorTexture,frame.getGbuffer().getTexture(0),0);
	g_transShader.setRes(nv::vec2f(rasterWidth,rasterHeight));
	g_transShader.begin();
	// 
/*
	glEnable(GL_TEXTURE_2D);
	float *pTexture = NULL;
	pTexture = new float[1024*1024 * 3];
	memset(pTexture, 0, 1024*1024 * 3 * sizeof(float));
	
	glBindTexture(GL_TEXTURE_2D, VecorTexture);//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	int x = 466
	glBindTexture(GL_TEXTURE_2D, 0);//TexPosId   PboTex
	*/
	/*float2 TexCoord0,TexCoord2,TexCoord3,TexCoord4,TexCoord1,TexCoord5,TexCoord6,TexCoord7;
	float2 Toffset1,Toffset2,Toffset3,Toffset4;
	float2 TPostion1,TPostion2,TPostion3,TPostion4;
	glDisable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,reflectionMapTex);
	 	glBegin( GL_TRIANGLES);
	 	//glBegin(GL_TRIANGLES_ADJACENCY_EXT);
	 	//glBegin( GL_QUADS);
	 	//glBegin( GL_POINTS);
	 	for(int x = 0;x<1024;x++){
	 		TexCoord0.x = x/1024.0f;
	 		TexCoord1.x = (x - 1.0) /1024.0;
	 		TexCoord2.x = x/1024.0f;
	 		TexCoord3.x = x/1024.0f;
	 		TexCoord4.x = (x + 1.0)/1024.0f;
	 		TexCoord5.x = (x + 1.0)/1024.0f;
	 		TexCoord6.x = (x + 1.0)/1024.0f;
	 		TexCoord7.x = (x + 2.0)/1024.0f;
	 			for(int y = 0;y<1024;y++){
	 
	 				TexCoord0.y = y/1024.0f;
	 				TexCoord1.y = (y+1)/1024.0f;
	 				TexCoord2.y = (y+1)/1024.0f;
	 				TexCoord3.y = (y+2)/1024.0f;
	 				TexCoord4.y = (y+1)/1024.0f;
	 				TexCoord5.y = y/1024.0f;
	 				TexCoord6.y = (y-1)/1024.0f;
	 				TexCoord7.y = y/1024.0f;
	 
	 				//TPostion1 = Toffset1 + TexCoord1;
	 
	 				//glTexCoord2f(TexCoord1.x,TexCoord1.y);
	 				//glVertex2f(TPostion1.x,TPostion1.y);
	 				glVertex2f(TexCoord0.x,TexCoord0.y);
	 				//glVertex2f(TexCoord1.x,TexCoord1.y);
	 				glVertex2f(TexCoord2.x,TexCoord2.y);
	 				//glVertex2f(TexCoord3.x,TexCoord4.y);
	 				glVertex2f(TexCoord4.x,TexCoord4.y);
	 				//glVertex2f(TexCoord5.x,TexCoord5.y);
	 
	 
	 				glVertex2f(TexCoord0.x,TexCoord0.y);
	 				//glVertex2f(TexCoord2.x,TexCoord3.y);
	 				glVertex2f(TexCoord4.x,TexCoord4.y);
	 				//glVertex2f(TexCoord7.x,TexCoord4.y);
	 				glVertex2f(TexCoord5.x,TexCoord5.y);
	 				//glVertex2f(TexCoord6.x,TexCoord6.y);
	 
	 				//glVertex2f(x/1024.0,y/1024.0);
	 				//glVertex2f((x+1)/1024.0,y/1024.0);
	 				//glVertex2f((x+1)/1024.0,(y+1)/1024.0);
	 
	 				//glVertex2f((x+1)/1024.0,(y+1)/1024.0);
	 				//glVertex2f(x/1024.0,(y+1)/1024.0);
	 				//glVertex2f(x/1024.0,y/1024.0);
	 
	 			}
	 	}
	 	glEnd();
	// 	//glBindTexture(GL_TEXTURE_2D,reflectionMapTex);
	// 	
	*/
	//glCallList(MyCallList);

	glEnableClientState(GL_VERTEX_ARRAY);;
	glBindBuffer(GL_ARRAY_BUFFER, MyVBO);
	glVertexPointer  ( 2, GL_FLOAT, 0, (char *) 0);
#if DrawPoint
	glDrawArrays(GL_POINTS, 0, rasterHeight*rasterWidth*6);

#else
	glDrawArrays(GL_TRIANGLES, 0, rasterHeight*rasterWidth*6);
#endif

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);

	g_transShader.end();
	//myTransShader.DisUse();
	//delete [] pFloat2;

	//cgResetPassState(pass);



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




float delta;
float height1= 85;
float height2= 44;
float height3= 12;
float g_lerp = 0;
void cupRound(int beginTime,int endTime)
{
	if(currentTime<beginTime)
	{
		delta = 1000;
	}	
	else if(currentTime<endTime)
	{
		g_lerp = (currentTime-beginTime)*1.0f/(endTime-beginTime);
		delta = (1-g_lerp)*(height3);
	}
	else
	{
		delta = 0;	
	}
	//  delta = 0;	
}
extern transform1 transformArray[];
extern action ActArray[];

void cameraControl(int currentTime,CCamera& NowCamera)
{
	// cupRound(0,300);
	// updateGeometry();
	// if(currentTime==0)
	// 			g_currentCamera.navigate(posArray[0],posArray[0],currentTime,0,10000);
	// 	 currentTime++;
	/*
	if(currentTime<=300)
	{
	//	  g_currentCamera.navigate(posArray[7],posArray[7],currentTime,0,300);
	}
	if(currentTime>300)
	exit(0);
	currentTime++;
	// cupRound*/
	//	cupRound(10*k,12*k);
	// updateGeometry();
	//if(once ==0)
	{
	if(currentTime<=kk[0])
	{
		NowCamera.navigate(posArray[0],posArray[1],currentTime,0,kk[0]);
	}
	else if(currentTime<=kk[1])
	{
		NowCamera.navigate(posArray[1],posArray[2],currentTime,kk[0],kk[1]);
	}
	else if(currentTime<=kk[2])
	{
		NowCamera.navigate(posArray[2],posArray[3],currentTime,kk[1],kk[2]);
	}
	else if(currentTime<=kk[3])
	{
		NowCamera.navigate(posArray[3],posArray[4],currentTime,kk[2],kk[3]);
	}
	else if(currentTime<=kk[4])
	{
		NowCamera.navigate(posArray[4],posArray[5],currentTime,kk[3],kk[4]);
	}
	else if(currentTime<=kk[5])
	{
		NowCamera.navigate(posArray[5],posArray[6],currentTime,kk[4],kk[5]);
	}
	else if(currentTime<=kk[6])
	{
		NowCamera.navigate(posArray[6],posArray[7],currentTime,kk[5],kk[6]);
	}
	else if(currentTime<=kk[7])
	{
		NowCamera.navigate(posArray[7],posArray[8],currentTime,kk[6],kk[7]);
	}
	else if(currentTime<=kk[8])
	{
		NowCamera.navigate(posArray[8],posArray[9],currentTime,kk[7],kk[8]);
	}
	else if(currentTime<=kk[9])
	{
		NowCamera.navigate(posArray[9],posArray[10],currentTime,kk[8],kk[9]);
	}
	else if(currentTime<=kk[10])
	{
		NowCamera.navigate(posArray[10],posArray[11],currentTime,kk[9],kk[10]);
	}
	else if(currentTime<=kk[11])
	{
		NowCamera.navigate(posArray[11],posArray[12],currentTime,kk[10],kk[11]);
	}
	else if(currentTime<=kk[12])
	{
		NowCamera.navigate(posArray[12],posArray[13],currentTime,kk[11],kk[12]);
	}
	else if(currentTime<=kk[13])
	{
		NowCamera.navigate(posArray[13],posArray[14],currentTime,kk[12],kk[13]);
	}
	NowCamera.Look();
	//once = 1;
	}
	//currentTime++;



	//new paht


	// 	 if(currentTime<=kk[0])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[1],posArray[2],currentTime,0,kk[0]);
	// 	 }
	// 	 else if(currentTime<=kk[1])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[2],posArray[3],currentTime,kk[0],kk[1]);
	// 	 }
	// 	 else if(currentTime<=kk[2])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[3],posArray[4],currentTime,kk[1],kk[2]);
	// 	 }
	// 	 else if(currentTime<=kk[3])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[4],posArray[5],currentTime,kk[2],kk[3]);
	// 	 }
	// 	 else if(currentTime<=kk[4])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[5],posArray[6],currentTime,kk[3],kk[4]);
	// 	 }
	// 	 else if(currentTime<=kk[5])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[6],posArray[7],currentTime,kk[4],kk[5]);
	// 	 }
	// 	 else if(currentTime<=kk[6])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[7],posArray[8],currentTime,kk[5],kk[6]);
	// 	 }
	// 	 else if(currentTime<=kk[7])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[8],posArray[9],currentTime,kk[6],kk[7]);
	// 	 }
	// 	 else if(currentTime<=kk[8])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[9],posArray[10],currentTime,kk[7],kk[8]);
	// 	 }
	// 	 else if(currentTime<=kk[9])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[10],posArray[11],currentTime,kk[8],kk[9]);
	// 	 }
	// 	 else if(currentTime<=kk[10])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[11],posArray[12],currentTime,kk[9],kk[10]);
	// 	 }
	// 	 else if(currentTime<=kk[11])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[12],posArray[13],currentTime,kk[10],kk[11]);
	// 	 }
	// 	 else if(currentTime<=kk[12])
	// 	 {
	// 		 g_currentCamera.navigate(posArray[13],posArray[14],currentTime,kk[11],kk[12]);
	// 	 }
	//else if(currentTime<=kk[13])
	//{
	//g_currentCamera.navigate(posArray[13],posArray[14],currentTime,kk[12],kk[13]);
	// }

}
# if MerGe
void GetTracingPixels(){

	dim3 blockSize(32,32,1);
	dim3 GridSize(rasterWidth/32,rasterHeight/32,1);
	checkCudaErrors(cudaGraphicsMapResources(1,&cudaRes_ProPixels,0));

	TransProPixel2Cuda(&cudaRes_ProPixels);

	RunFirstPass(blockSize,GridSize,rasterWidth,rasterHeight);

	checkCudaErrors(cudaGraphicsUnmapResources(1, &cudaRes_ProPixels, 0));

	RunCudpp(Host_PixelState,Host_PixelSum);

	PixelsSum = Host_PixelSum[rasterWidth * rasterHeight - 1] + Host_PixelState[rasterWidth * rasterHeight - 1];
	//cout<<PixelsSum<<endl;
	checkCudaErrors(cudaMalloc((void**)&Dev_PixelPos,PixelsSum*sizeof(unsigned int)));

	RunThridPass(blockSize,GridSize,Dev_PixelPos);

	//if(currentTime2 >0){
	if(0){
		unsigned *Host_PixPos;
		Host_PixPos = (unsigned*)malloc(PixelsSum*sizeof(unsigned int));
		checkCudaErrors(cudaMemcpy(Host_PixPos,Dev_PixelPos,PixelsSum*sizeof(unsigned),cudaMemcpyDeviceToHost));

		cout<<"Pixel states:"<<endl;
		for(int i = 0;i< rasterHeight* rasterWidth;i++)
			cout<<Host_PixelState[i]<<endl;
		cout<<endl<<"Pixel Sums:"<<endl;
		for(int i = 0;i< rasterHeight* rasterWidth;i++)
			cout<<Host_PixelSum[i]<<endl;

		cout<<endl<<"Pixel Pos"<<endl;
		for(int i = 0;i<PixelsSum;i++)
			cout<<Host_PixPos[i]%rasterWidth<<" "<<Host_PixPos[i]/rasterWidth<<endl;
		free(Host_PixPos);
	}
	cout<<"tracing num: "<<PixelsSum<<"rate: "<<PixelsSum/(1024*1024*1.0)<<endl;










}
#endif


void DrawQuad(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//refGbuffer.BindForWrite(0);
	glViewport(0, 0, (int)1024, (int)1024);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// 
	glBegin( GL_QUADS);
	glTexCoord2f( 0.0f, 0.0f);
	glVertex2f( -1.0f, -1.0f);
	glTexCoord2f( 1.0f, 0.0f);
	glVertex2f( 1.0f,-1.0f);
	glTexCoord2f( 1.0f, 1.0f);
	glVertex2f( 1.0f, 1.0f);
	glTexCoord2f( 0.0f, 1.0f);
	glVertex2f( -1.0f, 1.0f);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
nv::vec3f reflect(const nv::vec3f & i, const nv::vec3f & n)
{
  return i - 2.0f * n * dot(n,i);
}
void testRendering();
void optixRendering()
{
	if (stat_breakdown) 
	{
		glFinish();
		double display_start = TimeMesure::getCurrentTime();
		// sutilCurrentTime(&display_start);
		g_timeMesure.setBeginTime(display_start);
	}
	CVector3& pos =g_refCamera.Position();
	//cameraControl(currentTime,g_refCamera);
	cameraControl(currentTime2,g_currentCamera);
	g_refCamera = g_currentCamera;
	currentGbuffer.begin();
	//draw_scene(cgTechniqueWorldPosNormal,&g_refCamera);
	//currentGbuffer.SaveBMP("test/worldPos.bmp",0);
	t_scene.draw_model(g_gBufferShader,&g_refCamera);
	currentGbuffer.end();

	rtContext["eye_pos"]->setFloat(g_currentCamera.Position().x, g_currentCamera.Position().y, g_currentCamera.Position().z);
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
	{
	nv::vec3f worldPos;
	glEnable(GL_TEXTURE_2D);
	float *pTexture = NULL;
	int x = 571,y =702;  
	if(currentTime2==9)
	{
		 x = 579,y=706;
	}
	int xlenght = 1024,ylength = 1024;
	pTexture = new float[rasterWidth*rasterHeight * 4];
	memset(pTexture, 0, rasterWidth*rasterHeight * 4 * sizeof(float));

	glBindTexture(GL_TEXTURE_2D, (reflectionMapTex_Now));//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pTexture);

	int w = rasterWidth;
	int h = rasterHeight;
	nv::vec4f pixel = nv::vec4f(&pTexture[4*(y*ylength+x)]);
	nv::vec3f truePos = nv::vec3f(pixel.x,pixel.y,pixel.z);
	float reDis = pixel.w;

	if (pTexture)
	   delete[] pTexture;

	glEnable(GL_TEXTURE_2D);
	///*pTexture = NULL;
	pTexture = new float[rasterWidth*rasterHeight * 4];
	memset(pTexture, 0, rasterWidth*rasterHeight * 4 * sizeof(float));

	glBindTexture(GL_TEXTURE_2D, (currentGbuffer.getTexture(0)));//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pTexture);
	 pixel = nv::vec4f(&pTexture[4*(y*ylength+x)]);
	worldPos = nv::vec3f(pixel.x,pixel.y,pixel.z);

	glBindTexture(GL_TEXTURE_2D, (currentGbuffer.getTexture(1)));//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pTexture);
	 pixel = nv::vec4f(&pTexture[4*(y*ylength+x)]);
	nv::vec3f floorNomral =  nv::vec3f(pixel.x,pixel.y,pixel.z);
	
	if (pTexture)
	   delete[] pTexture;
	nv::vec3f floorPos = nv::vec3f(worldPos.x,worldPos.y,worldPos.z);
	nv::vec3f lateCameraPos = g_refCamera.getCameraPos();
	nv::vec3f cameraPos = g_currentCamera.getCameraPos();

	cameraPos = g_currentCamera.getCameraPos();
	
	nv::vec3f V = normalize(floorPos-cameraPos);
	nv::vec3f reflectDirction = normalize( reflect(V, floorNomral));
	nv::vec3f testPos = floorPos+reflectDirction*reDis;
	nv::vec3f targetPos = nv::vec3f(15.8356676,0.000000000,13.9763155 );
	float targetDis = length(testPos-targetPos);

	nv::vec3f reflectedPos = testPos;
	

	nv::vec3f toCamera = normalize(cameraPos-floorPos);
	float toFloorDis = dot(( floorPos-reflectedPos),-floorNomral);
	nv::vec3f mirrorPos = reflectedPos - 2*toFloorDis*floorNomral;

	cameraPos = cameraPos;
	nv::vec3f mirrorPosToCamera = normalize(cameraPos - mirrorPos);
	float delta = dot(mirrorPosToCamera,floorNomral);
	nv::vec3f intersectPos = mirrorPos+ toFloorDis/delta*mirrorPosToCamera;
	
	nv::matrix4f mvp = g_refCamera.getMvpMat();
	nv::vec4f temp = (mvp*nv::vec4f(intersectPos,1));
	temp /= temp.w;
	temp.x = temp.x*0.5+0.5;
	temp.y = temp.y*0.5+0.5;
	temp.x*=1024;
	temp.y*=1024;
	temp = temp;
	}
	
	*/
	
	

	//currentTime2++;
	if (stat_breakdown) 
	{
		double finalRendingTime = TimeMesure::getCurrentTime();;
		// sutilCurrentTime(&finalRendingTime);
		g_timeMesure.setFinalRenderingTime(finalRendingTime);
	}

	cgGLSetTextureParameter(cgReflectionMapParam,reflectionMapTex_Now);

	g_reflectionShader.setReflectMap(reflectionMapTex_Now);
	currentGbuffer.begin();

	//New_drawscene(cgTechniqueGlossyReflections);
	//draw_scene(cgTechniqueGlossyReflections,&g_refCamera);
	//testRendering();
     t_scene.draw_model(g_reflectionShader,&g_refCamera);
	currentGbuffer.end();

	char str[100];
	sprintf(str,"./test/optix%d_%d.bmp",currentTime,currentTime2);
	currentGbuffer.SaveBMP(str,0);
	glFinish();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);     // bind the screen FBO
	glViewport(0, 0, rasterWidth , rasterHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// 

	glBindFramebuffer(GL_READ_FRAMEBUFFER, currentGbuffer.fboId);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, rasterWidth, rasterHeight, 0, 0, rasterWidth, rasterHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glFinish();
	if (stat_breakdown) 
	{
		double finish_start = TimeMesure::getCurrentTime();;
		glFinish();
		//sutilCurrentTime(&finish_start);
		g_timeMesure.setEndTime(finish_start);
		g_timeMesure.print();
	}
	//currentTime2++;


}
void testRendering()
{
	glDisable(GL_DEPTH_TEST);
	g_testShader.setMvpMatrix(g_currentCamera.getMvpMat());
	g_testShader.setModelViewMatrix(g_currentCamera.getModelViewMat());
	CGtechnique tech= g_testShader.getTechnique();
	CGpass pass = cgGetFirstPass(tech);
	while(pass) {
		cgSetPassState(pass);
		glPointSize(4);
		glBegin(GL_POINTS);
// 
 			glVertex3f(36.3191872,-45.5569725,-50.4489975 );
 			glVertex3f(36.2738419 ,-45.6298523 ,-50.3806381  );
 		glEnd();

		cgResetPassState(pass);
		pass = cgGetNextPass(pass);
	}
	glEnable(GL_DEPTH_TEST);
}
void test()
{
	nv::vec3f reflectedPos = nv::vec3f(2.78407288,167.412674,77.0427780);
	nv::vec3f floorPos = nv::vec3f(-1.76701510,0.000000000,0.142561898	);
	nv::vec3f floorNomral = nv::vec3f(0,1,0);

	nv::vec3f cameraPos = g_refCamera.getCameraPos();
	nv::vec3f toCamera = normalize(cameraPos-floorPos);
	float toFloorDis = dot(( floorPos-reflectedPos),-floorNomral);
	nv::vec3f mirrorPos = reflectedPos - 2*toFloorDis*floorNomral;
	nv::vec3f mirrorPosToCamera = normalize(cameraPos - mirrorPos);
	float delta = dot(mirrorPosToCamera,floorNomral);
	nv::vec3f intersectPos = reflectedPos- toFloorDis/delta*mirrorPosToCamera;
	
	nv::matrix4f mvp = g_currentCamera.getMvpMat();
	nv::vec4f temp = (mvp*nv::vec4f(reflectedPos,1));
	temp /= temp.w;
	temp.x = temp.x*0.5+0.5;
	temp.y = temp.y*0.5+0.5;
	temp.x*=1024;
	temp.y*=1024;
}
void init_RefcletTex()
{

	BasicLight* lightData = static_cast<BasicLight*>(rtLights->map());
	lightData[0].pos = make_float3(lightPos.x, lightPos.y, lightPos.z);
	lightData[0].color.x = 1;
	lightData[0].color.y = 1;
	lightData[0].color.z = 1;
	lightData[0].casts_shadow = 0;
	rtLights->unmap();

	for(int i =0;i<KEYFRAMENUM;i++)
	{
		RefFrame::getFrameByIndex(i).init();
	}
	for(int i = 0;i<KEYFRAMENUM;i++)
	{
		int FrameNums = i*TIMEGAP;
		RefFrame & frame = RefFrame::getFrameByIndex(i);
		CCamera camera = frame.getCamera();
		cameraControl(FrameNums,camera);

		frame.getGbuffer().begin();
		draw_scene(cgTechniqueWorldPosNormal,&camera);

		 // t_scene.draw_model(g_gBufferShader,&g_refCamera);
		frame.getGbuffer().end();
		
		currentGbuffer.copyFromBuffer(frame.getGbuffer());
	//	currentGbuffer.SaveBMP("./test/world234.bmp",0);
	//	currentGbuffer.SaveBMP("./test/world123.bmp",1);
		/*currentGbuffer.begin();
		draw_scene(cgTechniqueWorldPosNormal,&g_currentCamera);
		currentGbuffer.end();*/
		rtContext["eye_pos"]->setFloat(camera.Position().x, camera.Position().y, camera.Position().z);
		try
		{
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
		glBindTexture(GL_TEXTURE_2D, frame.getOptixTex());
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
		glBindTexture(GL_TEXTURE_2D, (RefFrame::getFrameByIndex(0).getOptixTex()));//TexPosId   PboTex

		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

		int w = rasterWidth;
		int h = rasterHeight;
		char str [32];
		sprintf(str,"./test/ref%d.bmp",0);
		SaveBMP(str, pTexture, w, h);
		if (pTexture)
		   delete[] pTexture;*/
	}
	/*
	for(int i = 0;i<ReflectNum;i++)
	{

		int FrameNums = i * 10;
		cameraControl(FrameNums,g_currentCamera);

		currentGbuffer.begin();
		draw_scene(cgTechniqueWorldPosNormal,&g_currentCamera);
		currentGbuffer.end();
		//currentGbuffer.SaveBMP("worPos.bmp",0);

		rtContext["eye_pos"]->setFloat(g_currentCamera.Position().x, g_currentCamera.Position().y, g_currentCamera.Position().z);
		try
		{
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
		glBindTexture(GL_TEXTURE_2D, reflectionMaps[i]);
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

	glBindTexture(GL_TEXTURE_2D, (reflectionMaps[i]));//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	int w = rasterWidth;
	int h = rasterHeight;
	char str [32];
	sprintf(str,"ref%d.bmp",i);
	SaveBMP(str, pTexture, w, h);
	if (pTexture)
	   delete[] pTexture;*/
		/*
		glEnable(GL_TEXTURE_2D);
		BYTE *pTexture = NULL;
		pTexture = new BYTE[rasterWidth*  rasterHeight * 3];
		memset(pTexture, 0, rasterWidth* rasterHeight * 3 * sizeof(BYTE));

		glBindTexture(GL_TEXTURE_2D, reflectionMaps[i]);//TexPosId   PboTex

		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

		int w = rasterWidth;
		int h = rasterHeight;
		SaveBMP("234.bmp", pTexture, w, h);
		if (pTexture)
		delete[] pTexture;
		*/

		// 		char filename[256];
		// 		sprintf(filename,"pic/reflec/pic%d.bmp",FrameNums);
		// 		BYTE *pTexture = new BYTE[rasterWidth*rasterHeight*3];
		// 		memset(pTexture, 0, rasterWidth*rasterHeight*3 * sizeof(BYTE));
		// 
		// 		glBindTexture(GL_TEXTURE_2D, reflectionMaps[i]);//TexPosId   PboTex
		// 
		// 		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);
		// 
		// 		SaveBMP(filename,pTexture,rasterWidth,rasterHeight);
		// 		delete[] pTexture;
		// 		glBindTexture(GL_TEXTURE_2D,0);

	/*}

	cout<<"reflec ok"<<endl;
	*/




}



void setTile()
{
	static char titleBuffer[48];
	sprintf(titleBuffer,"Frame:%d",currentTime2);
	glutSetWindowTitle(titleBuffer); 
}
nv::vec4f myTex2d(GLuint texId,nv::vec2f coor)
{
	glEnable(GL_TEXTURE_2D);
	float *pTexture = NULL;
	pTexture = new float[rasterWidth*rasterHeight * 4];
	memset(pTexture, 0, rasterWidth*rasterHeight * 4 * sizeof(float));

	glBindTexture(GL_TEXTURE_2D, texId);//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pTexture);

	int w = rasterWidth;
	int h = rasterHeight;
	nv::vec4f test = nv::vec4f(&pTexture[4*((int)coor.y*rasterHeight+(int)coor.x)]);
	if (pTexture)
	   delete[] pTexture;
	glBindTexture(GL_TEXTURE_2D, 0);
	return test;
}
nv::vec3f to3(nv::vec4f test)
{
	return nv::vec3f(test.x,test.y,test.z);
}
nv::vec3f myReflect(const nv::vec3f& i, const nv::vec3f& n)
{
  return i - 2.0f * n * dot(n,i);
}

void CpuTracint(GLuint reflectOptix)
{
	return;
	nv::vec2f coord = nv::vec2f(0,0);
	nv::vec3f worldPos = to3(myTex2d(refGbuffer.getTexture(0),coord));
	nv::vec3f worldNormal = to3(myTex2d(refGbuffer.getTexture(1),coord));
	nv::vec4f reflectValue = myTex2d(reflectOptix,coord);

	printf("woldPos:(%f,%f,%f)\n",worldPos.x,worldPos.y,worldPos.z);
	printf("worldNomal:(%f,%f,%f)\n",worldNormal.x,worldNormal.y,worldNormal.z);
	
	float ReflectDis = reflectValue.w;
	
	nv::vec3f refCamera = nv::vec3f(g_refCamera.Position().x,g_refCamera.Position().y,g_refCamera.Position().z);

	nv::vec3f newCameraPos = nv::vec3f(g_currentCamera.Position().x,g_currentCamera.Position().y,g_currentCamera.Position().z);

	nv::vec3f LookVec = normalize(worldPos - refCamera);
	//计算反射光线方向
	nv::vec3f ReflectVec = normalize(myReflect(LookVec,worldNormal));
	//计算被反射物体坐标
	nv::vec3f ReflectPos = worldPos + ReflectVec * ReflectDis;
	

 	//RefelctInCameraDepth  = TMpReflecPosInCamera.z / TMpReflecPosInCamera.w;


	float CosReCorner = dot(ReflectVec,worldNormal);
	//镜像点的坐标
	nv::vec3f ReMirrorPos = ReflectDis * CosReCorner * 2 * (-1) * worldNormal + ReflectPos;
	printf("ReMirrorPos(%f,%f,%f)\n",ReMirrorPos.x,ReMirrorPos.y,ReMirrorPos.z);

	
	{
	nv::matrix4f mvpMatr = nv::matrix4f(g_currentCamera.getMvpMat());
	nv::vec4f temp = mvpMatr*nv::vec4f(ReMirrorPos.x,ReMirrorPos.y,ReMirrorPos.z,1);
	temp /= temp.w;
	temp.x  = temp.x*0.5+0.5;
	temp.y = temp.y*0.5+0.5;
	printf("cpu coord;%f,%f\n",temp.x*1024,temp.y*1024);
	//相机到反射面的距离
	}
	{
		nv::matrix4f mvpMatr = nv::matrix4f(g_refCamera.getMvpMat());
	nv::vec4f temp = mvpMatr*nv::vec4f(ReMirrorPos.x,ReMirrorPos.y,ReMirrorPos.z,1);
	temp /= temp.w;
	temp.x  = temp.x*0.5+0.5;
	temp.y = temp.y*0.5+0.5;
	printf("g_refCamera cpu coord;%f,%f\n",temp.x*1024,temp.y*1024);
	//相机到反射面的距离
	}
}

void tcRendering()
{

	//if(currentTime2>=200)
	//return;
	printf("frame#:%d\n",currentTime2);
	setTile();
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
	cameraControl(currentTime2,g_currentCamera);
	
	OptixFrame = currentTime2 /JianGe;
	if(currentTime2 /JianGe> 5)
		OptixFrame++;

	cameraControl(OptixFrame*10,g_refCamera);
	//test();
	
	refGbuffer.begin();
	//draw_scene(cgTechniqueWorldPosNormal,&g_refCamera);
    t_scene.draw_model(g_gBufferShader,&g_refCamera);
	//draw_scene(g_gBufferShader,&g_refCamera);
	//refGbuffer.SaveBMP("./test/gbuffer.bmp",0);
	refGbuffer.end();
	
	//checkCudaErrors(cudaGraphicsGLRegisterImage(&cudaRes_Reflect,reflectionMaps[OptixFrame+1],GL_TEXTURE_2D,cudaGraphicsMapFlagsReadOnly));
	//cudaGraphicsUnmapResources()
	//cout<<OptixFrame<<endl;
	if (stat_breakdown) 
	{
		double cudaBeginTime=g_timeMesure.getCurrentTime();;
		g_timeMesure.setCudaBeginTime(cudaBeginTime);
	}
	float ComputeStar=0,ComputeEnd;
	
	RefFrame & frame = RefFrame::getFrameByIndex(OptixFrame);
	//poxCudaTex.setEveryTex(frame.getGbuffer().getTexture(0));
	//normalCudaTex.setEveryTex(frame.getGbuffer().getTexture(1));
	//reflectCudaTex.setEveryTex(frame.getOptixTex());
	reflectCudaTex.setEveryTex(frame.getOptixTex());
	//CpuTracint(reflectionMaps[OptixFrame]);
	//   cudaEvent_t start,stop;
	//   cudaEventCreate(&start);
	//   cudaEventCreate(&stop);

	// cudaEventRecord(start,0);
	ComputeVecInCuda();
	fflush(stdout);
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
	vectorCudaArray.generateTex();
	VecorTexture = vectorCudaArray.getTexture();


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

	
	//Draw TransMap
#if SpeedUp

	double BeginDrawT;
	double EndDirawT;
	
	if (stat_breakdown) 
	{
		double forwardBeginTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setForwardingTime(forwardBeginTime);
	}

	TransMapFbo.BindForWrite(0); 
	drawTransMap(OptixFrame);
	TransMapFbo.end();
	char str[100];
	sprintf(str,"test/tttransMap%d.bmp",currentTime2);
	TransMapFbo.debugPixel(0,0,1023,1024);
	TransMapFbo.SaveBMP(str,0);
	if(stat_breakdown)
	{
		glFinish();
		sutilCurrentTime(&EndDirawT);
	}
	


#endif
	//TransMapFbo.SaveBMP(str,0);
		glViewport(0,0, traceWidth, traceHeight);
	currentGbuffer.begin();
	//draw_scene(cgTechniqueWorldPosNormal,&g_currentCamera);
	t_scene.draw_model(g_gBufferShader,&g_currentCamera);
	currentGbuffer.end();
	


#if SpeedUp

	
	FinalEffectFbo.BindForWrite(0);

	drawFinalEffect();

	FinalEffectFbo.end();

	

#endif
	FinalEffectFbo.SaveBMP("test/FinalEffectFbo.bmp",0);

	if (stat_breakdown) 
	{
		// glFinish();
		double secendRacingTimec=g_timeMesure.getCurrentTime();
		g_timeMesure.setSecondTraceTime(secendRacingTimec);
	}
	mapping();
	
		

	vectorCudaArray.generateTex();
	VecorTexture = vectorCudaArray.getTexture();

	/*glEnable(GL_TEXTURE_2D);
	BYTE* pTexture = NULL;
	pTexture = new BYTE[rasterWidth*rasterHeight * 3];
	memset(pTexture, 0, rasterWidth*rasterHeight * 3 * sizeof(BYTE));

	glBindTexture(GL_TEXTURE_2D, VecorTexture);//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	SaveBMP("123.bmp", pTexture, rasterWidth, rasterHeight);
	*/
	double DrawTime1,DrawTime2;

	glFinish();
	sutilCurrentTime(&DrawTime1);
	cgGLSetTextureParameter(cgReflectionMapParam,FinalEffectFbo.getTexture(0));
	if (stat_breakdown) 
	{
		// glFinish();
		double finalRenderingTime=g_timeMesure.getCurrentTime();
		g_timeMesure.setFinalRenderingTime(finalRenderingTime);
	}

	currentGbuffer.begin();

	draw_scene(cgTechniqueGlossyReflections,&g_currentCamera);

	currentGbuffer.end();

	// currentGbuffer.SaveBMP("asd.bmp",0);
	glFinish();
	sutilCurrentTime(&DrawTime2);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);     // bind the screen FBO
	glViewport(0, 0, rasterWidth , rasterHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// 

	glBindFramebuffer(GL_READ_FRAMEBUFFER, currentGbuffer.fboId);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, rasterWidth, rasterHeight, 0, 0, rasterWidth, rasterHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// 	 glBegin( GL_QUADS);
	// 	 glColor3f(1.0f,0,0);//lTexCoord2f( 0.0f, 0.0f);
	// 	 glVertex2f( -1.0f, -1.0f);
	// 	// glTexCoord2f( 1.0f, 0.0f);
	// 	 glVertex2f( 1.0f,-1.0f);
	// 	// glTexCoord2f( 1.0f, 1.0f);
	// 	 glVertex2f( 1.0f, 1.0f);
	// 	// glTexCoord2f( 0.0f, 1.0f);
	// 	 glVertex2f( -1.0f, 1.0f);
	// 	 glEnd();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glFinish();
	sutilCurrentTime(&FrameEnd);

	char filename1[256];
	sprintf(filename1,"pic/heihei2/pic%d.bmp",currentTime2);
	// currentGbuffer.SaveBMP(filename1,0);


	double finish_start;
	if (stat_breakdown) 
	{
		glFinish();
		double finish_start=g_timeMesure.getCurrentTime();
		g_timeMesure.setEndTime(finish_start);
		g_timeMesure.print();
	}
	currentTime2++;
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

	}
	//timeMesure.nextFrame();
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

	float aspect = (float)w/(float)h;
	float diag = nv::length(modelBBMax - modelBBMin);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspect, diag*.01, diag*100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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
	glBindTexture(GL_TEXTURE_2D, reflectionMapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, traceWidth, traceHeight, 0, GL_RGBA, GL_FLOAT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// resize rt buffers
	rtReflectionBuffer->setSize(traceWidth,traceHeight);

	glutPostRedisplay();

	CHECK_ERRORS();
}

void idle()
{
	g_currentCamera.Update();
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

	switch(k) {

	case 27: // esc
	case 'q':
		exit(0);
		break;

	case 'b':
		doISG = !doISG;
		cgSetParameter1i(cgDoISGParam, (int)doISG);
		break;

	case 'c': {
		/*  nv::matrix4f xform = manipulator.getTransform();
		for(int i = 0; i < 16; ++i) 
		{
		std::cerr << xform(i%4, i/4) << ' ';
		if(i%4==3)
		std::cerr << std::endl;
		}
		std::cerr << std::endl;*/
			  } break;

	case 'e':
		scene_epsilon /= 10;
		rtContext["scene_epsilon"]->setFloat(scene_epsilon);
		break;
	
	case 'E':
		scene_epsilon *= 10;
		rtContext["scene_epsilon"]->setFloat(scene_epsilon);
		break;
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
		printf("{make_float3(%f,%f,%f  ),make_float3(%f,%f,%f )},\n",g_currentCamera.Position().x,g_currentCamera.Position().y,g_currentCamera.Position().z,g_currentCamera.View().x,g_currentCamera.View().y,g_currentCamera.View().z);
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
	case 't':
		doReflections = !doReflections;
		cgSetParameter1i(cgDoReflectionsParam, (int)doReflections);
		break;
	case '=':
		currentTime2++;
		break;
	case '-':
		currentTime2--;
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
	//freopen("stdout.txt","w",stdout);
	//freopen("stderr.txt","w",stderr);
	glutInit(&argc, argv);

	bool dobenchmark = false;
	std::string filename;
	std::string cgfx_path;
	std::string ground_tex;
	for(int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);
		if( arg == "--obj" || arg == "-o" ) {
			if( i == argc-1 ) {
				printUsageAndExit(argv[0]);
			}
			filename = argv[++i];
		} else if( arg == "--help" || arg == "-h" ) {
			printUsageAndExit(argv[0]);
		} else if (arg.substr(0, 3) == "-B=" || arg.substr(0, 23) == "--benchmark-no-display=" ||
			arg.substr(0, 3) == "-b=" || arg.substr(0, 23) == "--benchmark=") {
				dobenchmark = true;
				std::string bnd_args = arg.substr(arg.find_first_of('=') + 1);
				if (sutilParseImageDimensions(bnd_args.c_str(), &warmup_frames, &timed_frames) != RT_SUCCESS) {
					std::cerr << "Invalid --benchmark-no-display arguments: '" << bnd_args << "'" << std::endl;
					printUsageAndExit(argv[0]);
				}
		} else if (arg == "-B" || arg == "--benchmark-no-display" || arg == "-b" || arg == "--benchmark") {
			dobenchmark = true;
		} else if( arg == "--shader" || arg == "-s" ) {
			if( i == argc-1 ) {
				printUsageAndExit(argv[0]);
			}
			cgfx_path = argv[++i];
		} else if( arg == "--zup" || arg == "-z" ) {
			zUp = true;
		} else if (arg == "--camera" || arg == "-c" ) {
			if( i == argc-16 ) {
				printUsageAndExit(argv[0]);
			}
			for(int j = 0; j < 16; ++j) {
				fixedCameraMatrix(j%4,j/4) = static_cast<float>(atof(argv[++i]));
			}
			fixedCamera = true;
		} else if (arg == "--nofps" || arg == "-nf") { 
			drawFps = false;
		} else if (arg == "--compute-normals" || arg == "-cn") {
			stripNormals = true;
		} else if ( arg.substr( 0, 6 ) == "--dim=" ) {
			std::string dims_arg = arg.substr(6);
			if ( sutilParseImageDimensions( dims_arg.c_str(),
				&initialWindowWidth,
				&initialWindowHeight )
				!= RT_SUCCESS ) {
					std::cerr << "Invalid window dimensions: '" << dims_arg << "'" << std::endl;
					printUsageAndExit( argv[0] );
			}
		} else {
			fprintf( stderr, "Unknown option '%s'\n", argv[i] );
			printUsageAndExit( argv[0] );
		}
	}
	//dobenchmark = true;
	if( !dobenchmark ) printUsageAndExit( argv[0], false );

	if(filename.empty()) {
		filename = std::string( sutilSamplesDir() ) + "/isgReflections/table.obj";
	}
	if(cgfx_path.empty()) {
		cgfx_path = std::string( sutilSamplesDir() ) + "/isgReflections/isgReflections_new.cgfx";
	}

	CGFX_PATH = cgfx_path;
	GROUNDTEX_PATH = std::string( sutilSamplesDir() ) + "/isgReflections/ground.ppm";
	WALLTEX_PATH = std::string( sutilSamplesDir() ) + "/isgReflections/wall.ppm";

	glutInitWindowSize(1024, 1024);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("isgReflections");

	init_gl();
	init_scene(filename.c_str());
	init_optix();
	init_cuda(argc,argv);
	init_RefcletTex();

	CHECK_ERRORS();
	glutReportErrors();

	//MyCuda();
	glutDisplayFunc(display);
	// glutReshapeFunc(resize);
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
