
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

//nv::GlutExamine manipulator;

nv::Model* model,*sofaModel;
GLuint modelVB;
GLuint modelIB;
GLuint sofaVB;
GLuint sofaIB;
nv::vec3f modelBBMin, modelBBMax, modelBBCenter;
nv::vec3f lightPos;
float glossiness = 0.0f;
GLuint reflectionMapTex;
GLuint groundTex;
GLuint wallTex;
//GLuint goldTex;
GLuint woodTex;
GLuint  sampleFbo;
GLuint sampleTex;
GLuint depthTex;
GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT,GL_COLOR_ATTACHMENT2_EXT,GL_COLOR_ATTACHMENT3_EXT,GL_COLOR_ATTACHMENT4_EXT,GL_COLOR_ATTACHMENT5_EXT,GL_COLOR_ATTACHMENT6_EXT};

CCamera g_Camera;
//MyGeometry teapot;
MyGeometry teapot3("./M/floor.obj");
MyMeterial m("book.jpg");
MyMeterial m1(make_float3(1,1,0));
MyMeterial a[] = {"wood.jpg",make_float3(0,1,0)} ;
MyMeterial mat[] = 
{
	"./model/floor.jpg",  //0
	"./model/wood.jpg",    //1
	"./model/redstone2.jpg",  //2
	"./model/book2.jpg",	//3
	"./model/book.jpg",     //4
	"./model/japen2.jpg",   //5
	"./wp/up.jpg",    //6
	"./model/photo.jpg",     //7
	"./model/picture.jpg",   //8
	"./model/sofa.jpg",      //9
	"./wp/west.jpg",         //10 west
	"./wp/east.jpg",      //11  esat
	"./wp/north.jpg" ,  //12   north
	"./wp/south.jpg",       //13  south
	"./model/sphere.jpg",      //14
	make_float3(0.8,0.81,0.8),      //15
	make_float3(0.5,0.14,0.09),     //16
	make_float3(0,0,0),             //17 black
	make_float3(1,1,1),             //18 white
	make_float3(0.5,0.5,0.5),        //19 greay
	make_float3(1.1,1.0,1.0),          //20 pink
	make_float3(0.5,0.4,0.3),           //21 lightLine
	make_float3(0.89,0.89,0.89),             //22 white
	make_float3(0.67,1.0,0.51) ,              //23 bunny
	"./vase/test3.jpg",      //24
	
};
#define  geoNumber  23
GeoPara p[geoNumber] = 
{
	{"./bathroom/toilet.obj",11,0,0.0f},
	{"./bathroom/wall.obj",10,0,0.0f},
	{"./bathroom/barrier.obj",10,0,0.0f},
	{"./bathroom/ceil.obj",10,0,0.0f},
	{"./bathroom/cock.obj",10,0,0.0f},
	{"./bathroom/cup.obj",10,0,0.0f},
	{"./bathroom/floor.obj",10,0,0.0f},
	{"./bathroom/littleBarrier.obj",10,0,0.0f},
	{"./bathroom/wall.obj",10,0,0.0f},
	{"./bathroom/masac.obj",10,0,0.0f},//10


	{"./bathroom/metal.obj",10,0,0.0f},//11
	{"./bathroom/mirror.obj",10,0,0.0f},//12
	{"./bathroom/mirrorFrame.obj",10,0,0.0f},//13
	{"./bathroom/paper.obj",10,0,0.0f},//14
	{"./bathroom/soapSupport.obj",10,0,0.0f},//15
	{"./bathroom/sofa.obj",10,0,0.0f},//16
	{"./bathroom/sphereOnSofa.obj",10,0,0.0f},//17
	{"./bathroom/support.obj",10,0,0.0f},//18
	{"./bathroom/toothBrush1.obj",10,0,0.0f},//19
	{"./bathroom/toothBrush2.obj",10,0,0.0f},//20
	{"./bathroom/toothPaste.obj",10,0,0.0f},//20
	{"./bathroom/wash.obj",10,0,0.0f},//20
	{"./bathroom/washBasin.obj",10,0,0.0f},//20

	
	
};
MyGeometry geo[geoNumber];

GLuint             normalTex;
GLuint             worldSpaceTex;
FramebufferObject* worldSpaceNormalFBO;

unsigned int warmup_frames = 10u, timed_frames = 10u;
unsigned int initialWindowWidth  = 1024;
unsigned int initialWindowHeight = 1024;
const unsigned int rasterWidth=1024, rasterHeight=1024;
int logReflectionSamplingRate = 0;

GLuint reflectionMapPBO;

bool fixedCamera = false;
nv::matrix4f fixedCameraMatrix;
bool drawFps = true;
bool stat_breakdown = true;

// CG
CGcontext cgContext;
CGeffect  cgEffect;
CGtechnique cgTechniqueWorldPosNormal;
CGtechnique cgTechniqueGlossyReflections;

// OptiX
unsigned int traceWidth, traceHeight;

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
optix::Context        rtContext;
optix::Buffer         rtReflectionBuffer;
optix::Buffer         rtLights;
optix::TextureSampler rtNormalTexture;
optix::TextureSampler rtWorldSpaceTexture;

std::string CGFX_PATH;
std::string GROUNDTEX_PATH;
std::string WALLTEX_PATH;

bool animate = false;
bool doISG  = true;
bool zUp = false;
bool doReflections = true;
bool stripNormals = false;
bool g_Recordpic = false;
float scene_epsilon = 1e-3f;

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
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
void SaveBMP(char* fileName,BYTE * buf,UINT width,UINT height)
{
	short res1=0;
	short res2=0;
	long pixoff=54;
	long compression=0;
	long cmpsize=0;
	long colors=0;
	long impcol=0;
	char m1='B';
	char m2='M';

	DWORD widthDW = WIDTHBYTES(width * 24);

	long bmfsize=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
		widthDW * height;	
	long byteswritten=0;

	BITMAPINFOHEADER header;
	header.biSize=40; 						
	header.biWidth=width;
	header.biHeight=height;
	header.biPlanes=1;
	header.biBitCount=24;					
	header.biCompression=BI_RGB;			
	header.biSizeImage=0;
	header.biXPelsPerMeter=0;
	header.biYPelsPerMeter=0;
	header.biClrUsed=0;
	header.biClrImportant=0;

	FILE *fp;	
	fp=fopen(fileName,"wb");
	if (fp==NULL)
	{
		//MessageBox("error","Can't open file for writing");
		return;
	}

	BYTE *topdown_pixel = (BYTE *) malloc( width*height*3*sizeof(BYTE) );
	for(int j=0; j<height; j++ )
		for(int k=0; k<width; k++)
		{
			memcpy( &topdown_pixel[(j*width+k)*3], &buf[(j*width+k)*3+2], sizeof(BYTE) );
			memcpy( &topdown_pixel[(j*width+k)*3+2], &buf[(j*width+k)*3], sizeof(BYTE) );
			memcpy( &topdown_pixel[(j*width+k)*3+1], &buf[(j*width+k)*3+1], sizeof(BYTE) );
		}
		buf = topdown_pixel;

		//填充BITMAPFILEHEADER
		fwrite((BYTE  *)&(m1),1,1,fp); byteswritten+=1;
		fwrite((BYTE  *)&(m2),1,1,fp); byteswritten+=1;
		fwrite((long  *)&(bmfsize),4,1,fp);	byteswritten+=4;
		fwrite((int  *)&(res1),2,1,fp); byteswritten+=2;
		fwrite((int  *)&(res2),2,1,fp); byteswritten+=2;
		fwrite((long  *)&(pixoff),4,1,fp); byteswritten+=4;

		//填充BITMAPINFOHEADER
		fwrite((BITMAPINFOHEADER *)&header,sizeof(BITMAPINFOHEADER),1,fp);
		byteswritten+=sizeof(BITMAPINFOHEADER);


		//填充位图数据
		long row=0;
		long rowidx;
		long row_size;
		row_size=header.biWidth*3;
		long rc;
		for (row=0;row<header.biHeight;row++) {
			rowidx=(long unsigned)row*row_size;						      

			// 写一行
			rc=fwrite((void  *)(buf+rowidx),row_size,1,fp);
			if (rc!=1) 
			{
				break;
			}
			byteswritten+=row_size;	

			for (DWORD count=row_size;count<widthDW;count++) {
				char dummy=0;
				fwrite(&dummy,1,1,fp);
				byteswritten++;							  
			}

		}

		fclose(fp);
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
  cgSetParameter1i(cgDoISGParam, (int)doISG);

  getEffectParam(cgEffect, "DoReflections", &cgDoReflectionsParam);
  cgSetParameter1i(cgDoReflectionsParam, (int)doReflections);

  getEffectParam(cgEffect, "IsModel", &cgIsModelParam);
  getEffectParam(cgEffect, "IsWall", &cgIsWallParam);

  // create PBO's
  glGenBuffers(1, &reflectionMapPBO);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, 32*32*sizeof(float4), 0, GL_STREAM_READ);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  // setup FBO's
  worldSpaceNormalFBO = new FramebufferObject();

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
  }

  // and reflection map texture
  glGenTextures(1, &reflectionMapTex);
  glBindTexture(GL_TEXTURE_2D, reflectionMapTex);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, 32, 32, 0, GL_RGBA, GL_FLOAT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  cgGLSetTextureParameter(cgReflectionMapParam, reflectionMapTex);

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
  CLoad3DS loader;
  loader.BuildTexture("book.jpg",woodTex);
}

void init_scene(const char* model_filename)
{
 /* model = new nv::Model();
  char deskName[] = "table.obj";
  if(!model->loadModelFromFile(deskName)) {
    printf("Unable to load model\n");
    exit(-1);
  }

  model->removeDegeneratePrims();

  if(stripNormals) {
    model->clearNormals();
  }

  model->computeNormals();

 // model->clearTexCoords();
  model->clearColors();
  model->clearTangents();

  if(zUp) 
{
    nv::vec3f* vertices = (nv::vec3f*)model->getPositions();
    nv::vec3f* normals  = (nv::vec3f*)model->getNormals();
    for(int i = 0; i < model->getPositionCount(); ++i) {
      std::swap(vertices[i].y, vertices[i].z);
      vertices[i].z *= -1;

      std::swap(normals[i].y, normals[i].z);
      normals[i].z *= -1;
    }
  }

  model->compileModel();


  glGenBuffers(1, &modelVB);
  glBindBuffer(GL_ARRAY_BUFFER, modelVB);
  glBufferData(GL_ARRAY_BUFFER,
               model->getCompiledVertexCount()*model->getCompiledVertexSize()*sizeof(float),
               model->getCompiledVertices(), GL_STATIC_READ);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &modelIB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
               model->getCompiledIndexCount()*sizeof(int),
               model->getCompiledIndices(), GL_STATIC_READ);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


  sofaModel = new nv::Model();
  char sofaName[] = "table.obj";
  if(!sofaModel->loadModelFromFile(sofaName)) {
	  printf("Unable to load sofaModel\n");
	  exit(-1);
  }

  sofaModel->removeDegeneratePrims();

  if(stripNormals) {
	  sofaModel->clearNormals();
  }

  sofaModel->computeNormals();

  // sofamodel->clearTexCoords();
  sofaModel->clearColors();
  sofaModel->clearTangents();

  if(zUp) 
  {
	  nv::vec3f* vertices = (nv::vec3f*)sofaModel->getPositions();
	  nv::vec3f* normals  = (nv::vec3f*)sofaModel->getNormals();
	  for(int i = 0; i < sofaModel->getPositionCount(); ++i) {
		  std::swap(vertices[i].y, vertices[i].z);
		  vertices[i].z *= -1;

		  std::swap(normals[i].y, normals[i].z);
		  normals[i].z *= -1;
	  }
  }

  sofaModel->compileModel();

  glGenBuffers(1, &sofaVB);
  glBindBuffer(GL_ARRAY_BUFFER, sofaVB);
  glBufferData(GL_ARRAY_BUFFER,
	  sofaModel->getCompiledVertexCount()*sofaModel->getCompiledVertexSize()*sizeof(float),
	  sofaModel->getCompiledVertices(), GL_STATIC_READ);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &sofaIB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sofaIB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
	  sofaModel->getCompiledIndexCount()*sizeof(int),
	  sofaModel->getCompiledIndices(), GL_STATIC_READ);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
*/
  //table.initGeometry("teapot2.obj");
 // sofa.initGeometry("sofa.obj");
 // teapot.initGeometry("teapot.obj");
 // teapot3.initGeometry();
  //geo.initGeometry(p[0]);
  for(int i =0;i<geoNumber;i++)
  {
	  geo[i].initGeometry(p[i]);
  }
  float diag;
  //model->computeBoundingBox(modelBBMin, modelBBMax);
  modelBBMin = MyGeometry::modelBBMin,modelBBMax = MyGeometry::modelBBMax;
  modelBBCenter = (modelBBMin + modelBBMax) * 0.5;
  diag = nv::length(modelBBMax - modelBBMin);
  cgSetParameter1f(cgInvSceneScaleParam, 1.f/diag);
  lightPos = nv::vec3f(0.0f, 200.0f, 0.0f);

  cgSetParameter3f(   cgLightPosParam,
                      lightPos.x,
                      lightPos.y,
                      lightPos.z );

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
  g_Camera.PositionCamera(-40, 54, -151,    -22.8827, 42.5, -180.212,     0, 1, 0);

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

void init_optix()
{
  try 
  {
    rtContext = optix::Context::create();
    rtContext->setRayTypeCount(2);
    rtContext->setEntryPointCount(1);

    rtContext["radiance_ray_type"]->setUint(0u);
    rtContext["shadow_ray_type"]->setUint(1u);
    rtContext["scene_epsilon"]->setFloat(scene_epsilon);

    rtWorldSpaceTexture = rtContext->createTextureSamplerFromGLImage(worldSpaceTex, RT_TARGET_GL_TEXTURE_2D);
    rtWorldSpaceTexture->setWrapMode(0, RT_WRAP_CLAMP_TO_EDGE);
    rtWorldSpaceTexture->setWrapMode(1, RT_WRAP_CLAMP_TO_EDGE);
    rtWorldSpaceTexture->setWrapMode(2, RT_WRAP_CLAMP_TO_EDGE);
    rtWorldSpaceTexture->setIndexingMode(RT_TEXTURE_INDEX_ARRAY_INDEX);
    rtWorldSpaceTexture->setReadMode(RT_TEXTURE_READ_ELEMENT_TYPE);
    rtWorldSpaceTexture->setMaxAnisotropy(1.0f);
    rtWorldSpaceTexture->setFilteringModes(RT_FILTER_NEAREST, RT_FILTER_NEAREST, RT_FILTER_NONE);
    rtContext["request_texture"]->setTextureSampler(rtWorldSpaceTexture);

    rtNormalTexture = rtContext->createTextureSamplerFromGLImage(normalTex, RT_TARGET_GL_TEXTURE_2D);
    rtNormalTexture->setWrapMode(0, RT_WRAP_CLAMP_TO_EDGE);
    rtNormalTexture->setWrapMode(1, RT_WRAP_CLAMP_TO_EDGE);
    rtNormalTexture->setWrapMode(2, RT_WRAP_CLAMP_TO_EDGE);
    rtNormalTexture->setIndexingMode(RT_TEXTURE_INDEX_ARRAY_INDEX);
    rtNormalTexture->setReadMode(RT_TEXTURE_READ_ELEMENT_TYPE);
    rtNormalTexture->setMaxAnisotropy(1.0f);
    rtNormalTexture->setFilteringModes(RT_FILTER_NEAREST, RT_FILTER_NEAREST, RT_FILTER_NONE);
    rtContext["normal_texture"]->setTextureSampler(rtNormalTexture);

    rtReflectionBuffer = rtContext->createBufferFromGLBO(RT_BUFFER_OUTPUT, reflectionMapPBO);
    rtReflectionBuffer->setSize(32,32);
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
		
		optix::Material glossy = rtContext->createMaterial();
		glossy->setClosestHitProgram(0, rtContext->createProgramFromPTXFile( ptxpath("glossy_isg.cu"), "closest_hit_radiance") );
		glossy->setAnyHitProgram(1, rtContext->createProgramFromPTXFile( ptxpath("glossy_isg.cu"), "any_hit_shadow") );
		glossy["diffuse_Color"]->setFloat( 1.0f,0.0f,1.0f );
/*
		optix::Material glossy2 = rtContext->createMaterial();
		glossy2->setClosestHitProgram(0, rtContext->createProgramFromPTXFile( ptxpath("glossy_isg.cu"), "closest_hit_radiance") );
		glossy2->setAnyHitProgram(1, rtContext->createProgramFromPTXFile( ptxpath("glossy_isg.cu"), "any_hit_shadow") );
		glossy2["diffuse_Color"]->setFloat( 1.0f,1.0f,1.0f );
		rtContext->createGeometry();
    sofaGemetry->setPrimitiveCount( sofaModel->getCompiledIndexCount()/3 );
    sofaGemetry->setIntersectionProgram( rtContext->createProgramFromPTXFile( ptxpath("triangle_mesh_fat.cu"), "mesh_intersect" ) );
    sofaGemetry->setBoundingBoxProgram( rtContext->createProgramFromPTXFile( ptxpath("triangle_mesh_fat.cu"), "mesh_bounds" ) );

    int num_vertices = sofaModel->getCompiledVertexCount();
    optix::Buffer vertex_buffer = rtContext->createBufferFromGLBO(RT_BUFFER_INPUT, modelVB);
    vertex_buffer->setFormat(RT_FORMAT_USER);
    vertex_buffer->setElementSize(3*2*sizeof(float));
    vertex_buffer->setSize(num_vertices);
    sofaGemetry["vertex_buffer"]->setBuffer(vertex_buffer);

    optix::Buffer index_buffer = rtContext->createBufferFromGLBO(RT_BUFFER_INPUT, modelIB);
    index_buffer->setFormat(RT_FORMAT_INT3);
    index_buffer->setSize(sofaModel->getCompiledIndexCount()/3);
    sofaGemetry["index_buffer"]->setBuffer(index_buffer);

    optix::Buffer material_buffer = rtContext->createBuffer(RT_BUFFER_INPUT);
    material_buffer->setFormat(RT_FORMAT_UNSIGNED_INT);
    material_buffer->setSize(sofaModel->getCompiledIndexCount()/3);
    void* material_data = material_buffer->map();
    memset(material_data, 0, sofaModel->getCompiledIndexCount()/3*sizeof(unsigned int));
    material_buffer->unmap();
    sofaGemetry["material_buffer"]->setBuffer(material_buffer);*/
////////////////////////////////////////////////////////////////
	/*optix::Geometry tableGeometry = rtContext->createGeometry();
	sofaGemetry->setPrimitiveCount( model->getCompiledIndexCount()/3 );
	sofaGemetry->setIntersectionProgram( rtContext->createProgramFromPTXFile( ptxpath("triangle_mesh_fat.cu"), "mesh_intersect" ) );
	sofaGemetry->setBoundingBoxProgram( rtContext->createProgramFromPTXFile( ptxpath("triangle_mesh_fat.cu"), "mesh_bounds" ) );

	int num_vertices = sofaModel->getCompiledVertexCount();
	optix::Buffer vertex_buffer = rtContext->createBufferFromGLBO(RT_BUFFER_INPUT, modelVB);
	vertex_buffer->setFormat(RT_FORMAT_USER);
	vertex_buffer->setElementSize(3*2*sizeof(float));
	vertex_buffer->setSize(num_vertices);
	sofaGemetry["vertex_buffer"]->setBuffer(vertex_buffer);

	optix::Buffer index_buffer = rtContext->createBufferFromGLBO(RT_BUFFER_INPUT, modelIB);
	index_buffer->setFormat(RT_FORMAT_INT3);
	index_buffer->setSize(sofaModel->getCompiledIndexCount()/3);
	sofaGemetry["index_buffer"]->setBuffer(index_buffer);

	optix::Buffer material_buffer = rtContext->createBuffer(RT_BUFFER_INPUT);
	material_buffer->setFormat(RT_FORMAT_UNSIGNED_INT);
	material_buffer->setSize(sofaModel->getCompiledIndexCount()/3);
	void* material_data = material_buffer->map();
	memset(material_data, 0, sofaModel->getCompiledIndexCount()/3*sizeof(unsigned int));
	material_buffer->unmap();
	sofaGemetry["material_buffer"]->setBuffer(material_buffer);*/

  // optix::GeometryInstance sofaInstance = tea.getInstance(rtContext,glossy);
	// optix::GeometryInstance tableInstance = table.getInstance(rtContext,glossy);
/*	
    PPMLoader ground_ppm(GROUNDTEX_PATH);
    if(ground_ppm.failed()) {
      std::cerr << "Could not load PPM file " << GROUNDTEX_PATH << '\n';
      exit(1);
    }

    optix::Buffer floor_data = rtContext->createBuffer(RT_BUFFER_INPUT);
    floor_data->setFormat(RT_FORMAT_FLOAT4);
    floor_data->setSize(ground_ppm.width(), ground_ppm.height());
    float4* tex_data = (float4*)floor_data->map();
    uchar3* pixel_data = (uchar3*)ground_ppm.raster();
    for(unsigned int i = 0; i < ground_ppm.width() * ground_ppm.height(); ++i) {
      tex_data[i] = make_float4(static_cast<float>(pixel_data[i].x)/255.99f,
                                static_cast<float>(pixel_data[i].y)/255.99f,
                                static_cast<float>(pixel_data[i].z)/255.99f,
                                1.f);
    }
    floor_data->unmap();
    

    optix::TextureSampler floor_tex = rtContext->createTextureSampler();
    floor_tex->setWrapMode( 0, RT_WRAP_REPEAT );
    floor_tex->setWrapMode( 1, RT_WRAP_REPEAT );
    floor_tex->setIndexingMode( RT_TEXTURE_INDEX_NORMALIZED_COORDINATES );
    floor_tex->setReadMode( RT_TEXTURE_READ_NORMALIZED_FLOAT );
    floor_tex->setMaxAnisotropy( 1.0f );
    floor_tex->setMipLevelCount( 1u );
    floor_tex->setArraySize( 1u );
    floor_tex->setFilteringModes( RT_FILTER_LINEAR, RT_FILTER_LINEAR, RT_FILTER_NONE );
    floor_tex->setBuffer(0,0, floor_data);

    PPMLoader wall_ppm(WALLTEX_PATH);
    if(wall_ppm.failed()) {
      std::cerr << "Could not load PPM file " << GROUNDTEX_PATH << '\n';
      exit(1);
    }

    optix::Buffer wall_data = rtContext->createBuffer(RT_BUFFER_INPUT);
    wall_data->setFormat(RT_FORMAT_FLOAT4);
    wall_data->setSize(wall_ppm.width(), wall_ppm.height());
    tex_data = (float4*)wall_data->map();
    pixel_data = (uchar3*)wall_ppm.raster();
    for(unsigned int i = 0; i < wall_ppm.width() * wall_ppm.height(); ++i) {
      tex_data[i] = make_float4(static_cast<float>(pixel_data[i].x)/255.99f,
                                static_cast<float>(pixel_data[i].y)/255.99f,
                                static_cast<float>(pixel_data[i].z)/255.99f,
                                1.f);
    }
    wall_data->unmap();
    

    optix::TextureSampler wall_tex = rtContext->createTextureSampler();
    wall_tex->setWrapMode( 0, RT_WRAP_REPEAT );
    wall_tex->setWrapMode( 1, RT_WRAP_REPEAT );
    wall_tex->setIndexingMode( RT_TEXTURE_INDEX_NORMALIZED_COORDINATES );
    wall_tex->setReadMode( RT_TEXTURE_READ_NORMALIZED_FLOAT );
    wall_tex->setMaxAnisotropy( 1.0f );
    wall_tex->setMipLevelCount( 1u );
    wall_tex->setArraySize( 1u );
    wall_tex->setFilteringModes( RT_FILTER_LINEAR, RT_FILTER_LINEAR, RT_FILTER_NONE );
    wall_tex->setBuffer(0,0, wall_data);

    optix::Material flat_tex = rtContext->createMaterial();
    flat_tex->setClosestHitProgram(0, rtContext->createProgramFromPTXFile( ptxpath("flat_tex_isg.cu"), "closest_hit_radiance"));

    float diag = nv::length(modelBBMax - modelBBMin);
    optix::Geometry floor = createParallelogram(rtContext, 
                                                 nv::vec3f(-diag*.5f, modelBBMin.y, -diag*.5f),
                                                 nv::vec3f(diag, 0, 0),
                                                 nv::vec3f(0, 0, diag));

    optix::GeometryInstance floor_instance = rtContext->createGeometryInstance();
    floor_instance->setMaterialCount(1);
    floor_instance->setMaterial(0, flat_tex);
    floor_instance->setGeometry(floor);
    floor_instance["diffuse_texture"]->setTextureSampler(floor_tex);
	 
    optix::Geometry wall0 = createParallelogram(rtContext,
                                                 nv::vec3f(-diag*.5f, modelBBMax.y*2, diag*.5f),
                                                 nv::vec3f(0,0,  -diag),
                                                 nv::vec3f(0, -(modelBBMax.y*2-modelBBMin.y*1), 0));

    optix::GeometryInstance wall0_instance = rtContext->createGeometryInstance();
    wall0_instance->setMaterialCount(1);
    wall0_instance->setMaterial(0, flat_tex);
    wall0_instance->setGeometry(wall0);
    wall0_instance["diffuse_texture"]->setTextureSampler(wall_tex);

    optix::Geometry wall1 = createParallelogram(rtContext,
                                                 nv::vec3f( -diag*.5f, modelBBMax.y*2, -diag*.5f),
                                                 nv::vec3f(diag, 0, 0),
                                                 nv::vec3f(0, -(modelBBMax.y*2-modelBBMin.y),  0));

    optix::GeometryInstance wall1_instance = rtContext->createGeometryInstance();
    wall1_instance->setMaterialCount(1);
    wall1_instance->setMaterial(0, flat_tex);
    wall1_instance->setGeometry(wall1);
    wall1_instance["diffuse_texture"]->setTextureSampler(teapot3.initMaterial());
*/
//	teapot.setMaterial(&a[1]);
//	teapot.initGeometry("teapot.obj");
//	table.setMaterial(&a[1]);
	//geo[0].setMaterial(&mat[1]);
	//teapot3.setMaterial(&mat[0]);
//	optix::GeometryInstance teapotInstance = teapot.getInstance();
	//optix::GeometryInstance tableInstance = table.getInstance();
	//optix::GeometryInstance teapot3Instance = teapot3.getInstance();
    optix::GeometryInstance geoInstance[geoNumber];
	for(int i =0;i<geoNumber;i++)
	{
		geoInstance[i] = geo[i].getInstance();
	}

    optix::GeometryGroup geometrygroup = rtContext->createGeometryGroup();
    geometrygroup->setChildCount(geoNumber);
	for(int i=0;i<geoNumber;i++)
		 geometrygroup->setChild(i, geoInstance[i]);
  // geometrygroup->setChild(geoNumber, teapotInstance);
   // geometrygroup->setChild(1, floor_instance);
   
   // geometrygroup->setChild(3, wall1_instance);
	//geometrygroup->setChild(4, tableInstance);
	//geometrygroup->setChild(0, teapot3Instance);
	//geometrygroup->setChild(1, geoInstance[1]);
   // geometrygroup->setChild(0, geoInstance[2]);
    geometrygroup->setAcceleration( rtContext->createAcceleration("Bvh","Bvh") );

    rtContext["reflectors"]->set(geometrygroup);

    rtContext["max_depth"]->setUint(2u);
	rtContext["lightPos"]->set3fv((const float*)&make_float3(lightPos.x,
		lightPos.y,
		lightPos.z));

    rtContext->setStackSize(1850);
    rtContext->validate();

  } 
  catch ( optix::Exception& e )
 {
    sutilReportError( e.getErrorString().c_str() );
    exit(-1);
  }
}

void draw_ground(CGtechnique& tech)
{
  float diag = nv::length(modelBBMax - modelBBMin);
  float dim = diag*.5f;

  CGpass pass = cgGetFirstPass(tech);
  while(pass) {
    cgSetPassState(pass);
  
    glBegin(GL_TRIANGLES);
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(-dim, modelBBMin.y, -dim);
    glTexCoord2f(0,1); glVertex3f(-dim, modelBBMin.y, dim);
    glTexCoord2f(1,0); glVertex3f(dim, modelBBMin.y, -dim);

    glTexCoord2f(0,1); glVertex3f(-dim, modelBBMin.y, dim);
    glTexCoord2f(1,0); glVertex3f(dim, modelBBMin.y, -dim);
    glTexCoord2f(1,1); glVertex3f(dim, modelBBMin.y, dim);

    glEnd();

    cgResetPassState(pass);
    pass = cgGetNextPass(pass);
  }
}

void draw_walls(CGtechnique& tech)
{
  float diag = nv::length(modelBBMax - modelBBMin);
  float dim = diag*.5f;

  CGpass pass = cgGetFirstPass(tech);
  while(pass) {
    cgSetPassState(pass);
  
    glBegin(GL_TRIANGLES);

    // left wall
    glNormal3f(1,0,0);
    glTexCoord2f(1,0); glVertex3f(-dim, modelBBMax.y*2, -dim);
    glTexCoord2f(0,0); glVertex3f(-dim, modelBBMax.y*2,  dim);
    glTexCoord2f(0,1); glVertex3f(-dim, modelBBMin.y*1,  dim);

    glTexCoord2f(0,1); glVertex3f(-dim, modelBBMin.y*1, dim);
    glTexCoord2f(1,1); glVertex3f(-dim, modelBBMin.y*1, -dim);
    glTexCoord2f(1,0); glVertex3f(-dim, modelBBMax.y*2, -dim);

    // right wall

    // front wall
    glNormal3f(0,0,1);
    glTexCoord2f(1,1); glVertex3f( dim, modelBBMin.y*1,  -dim);
    glTexCoord2f(1,0); glVertex3f( dim, modelBBMax.y*2,  -dim);
    glTexCoord2f(0,0); glVertex3f(-dim, modelBBMax.y*2,  -dim);

    glTexCoord2f(0,0); glVertex3f(-dim, modelBBMax.y*2,  -dim);
    glTexCoord2f(0,1); glVertex3f(-dim, modelBBMin.y*1,  -dim);
    glTexCoord2f(1,1); glVertex3f( dim, modelBBMin.y*1,  -dim);

    // back wall

    glEnd();

    cgResetPassState(pass);
    pass = cgGetNextPass(pass);
  }
}

void draw_model(CGtechnique& tech)
{

	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

 /* glBindBuffer(GL_ARRAY_BUFFER, sofaVB);
  glEnableClientState(GL_VERTEX_ARRAY);

  glVertexPointer(  sofaModel->getPositionSize(),
                    GL_FLOAT,
                    sofaModel->getCompiledVertexSize()*sizeof(float),
                    (void*) (sofaModel->getCompiledPositionOffset()*sizeof(float)));

  if ( sofaModel->hasNormals() ) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(    GL_FLOAT,
                        sofaModel->getCompiledVertexSize()*sizeof(float),
                        (void*) (sofaModel->getCompiledNormalOffset()*sizeof(float)));
  }
  if (sofaModel->hasTexCoords()) {
	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	  glTexCoordPointer(   sofaModel->getPositionSize(),
		  GL_FLOAT,
		  sofaModel->getCompiledVertexSize()*sizeof(float),
		  (void*) (sofaModel->getCompiledTexCoordOffset()*sizeof(float)));
  }

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, sofaIB);

  CGpass pass = cgGetFirstPass(tech);
  while(pass) {
    cgSetPassState(pass);
    glDrawElements( GL_TRIANGLES, sofaModel->getCompiledIndexCount(), GL_UNSIGNED_INT, 0 );
    cgResetPassState(pass);
    pass = cgGetNextPass(pass);
  }
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_TEXTURE_COORD_ARRAY );


  CHECK_ERRORS();*/
	//teapot.drawGeometry(tech);
//	table.drawGeometry(tech);
	//teapot3.drawGeometry(tech);
	for(int i =0;i<geoNumber;i++)
		geo[i].drawGeometry(tech);
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, (int)512, (int)512);
	glBindTexture(GL_TEXTURE_2D, sampleTex);
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D); 
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

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
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);*/

	
}

void draw_scene(CGtechnique& current_technique)
{
  glLoadIdentity();
  if(fixedCamera) {
    glMultMatrixf(fixedCameraMatrix.get_value());
  } else {
   // manipulator.applyTransform();
	  glMatrixMode(GL_MODELVIEW);
	  glLoadIdentity();
	  g_Camera.Look();
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
  cgSetParameter3f(cgCameraPosition,g_Camera.Position().x,g_Camera.Position().y,g_Camera.Position().z);
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
  CHECK_ERRORS();
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
posPara posArray[] = 
{
	/*{make_float3(-54.354118 ,31.377062 ,-98.295815 ),make_float3(-56.967339 ,31.277061 ,-88.643814  )},

	{make_float3(-46.382030 ,32.773746 ,-60.686626),make_float3(-38.806332 ,30.865511 ,-54.444244 )},
	{make_float3(-70.253433 ,34.631981 ,-34.510601),make_float3(-61.109886 ,32.625679 ,-30.993509)},
	{make_float3(-82.555748 ,36.439411 ,24.676952),make_float3(-73.150719,34.004147,22.307531 )},
	{make_float3(-69.982262 ,37.326599 ,62.231113 ),make_float3(-62.214798 ,35.812447 ,56.117764)}*/
	//{make_float3(-133.917435,69.117668,48.299770  ),make_float3(-128.349808,70.135864,40.055748 )},
make_float3(-6.127516,38.713776,36.427696  ),make_float3(-3.888615,28.782856,2.150034 )
}
;
int currentTime;
float k,kk[12]={70,110,170,230,300,360,430,500,560,610,680,760};
void display()
{
  sutilFrameBenchmark("isgReflections", warmup_frames, timed_frames);

  double display_start;
  if (stat_breakdown) 
  {
    glFinish();
    sutilCurrentTime(&display_start);
  }
  // get eye position
 /* nv::matrix4f xform = fixedCamera ? fixedCameraMatrix : manipulator.getTransform();
  nv::matrix4f inv_xform = inverse(xform);
  nv::vec4f eye_pos = inv_xform.get_column(3);

  rtContext["eye_pos"]->setFloat(eye_pos.x, eye_pos.y, eye_pos.z);
*/

	if(currentTime<=kk[0]){
	 g_Camera.navigate(posArray[0],posArray[1],currentTime,0,kk[0]);
// currentTime++;
  }/*
  else if(currentTime<=kk[1])
  {
	  g_Camera.navigate(posArray[1],posArray[2],currentTime,kk[0],kk[1]);
  }
  else if(currentTime<=kk[2])
  {
	  g_Camera.navigate(posArray[2],posArray[3],currentTime,kk[1],kk[2]);
  }/*
  else if(currentTime<=kk[3])
  {
	  g_Camera.navigate(posArray[3],posArray[4],currentTime,kk[2],kk[3]);
  }

  else if(currentTime<=kk[4])
  {
	  g_Camera.navigate(posArray[4],posArray[5],currentTime,kk[3],kk[4]);
  }*/
 // currentTime++;
  rtContext["eye_pos"]->setFloat(g_Camera.Position().x, g_Camera.Position().y, g_Camera.Position().z);
//  std::cout<<g_Camera.Position().x<<' '<< g_Camera.Position().y<<' ' g_Camera.Position().z;
 // g_Camera.Position().x>>std::cout; 
  //printf("%f %f %f\n",g_Camera.Position().x, g_Camera.Position().y, g_Camera.Position().z);
  cgSetParameter1f(cgGlossinessParam, glossiness);

  BasicLight* lightData = static_cast<BasicLight*>(rtLights->map());
  lightData[0].pos = make_float3(lightPos.x, lightPos.y, lightPos.z);
  lightData[0].color.x = 1;
  lightData[0].color.y = 1;
  lightData[0].color.z = 1;
  lightData[0].casts_shadow = 0;
  rtLights->unmap();

  // render world space position buffer
  glViewport(0,0, traceWidth, traceHeight);
  worldSpaceNormalFBO->Bind();

  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  float NaN = std::numeric_limits<float>::quiet_NaN();
  glClearColor( NaN, NaN, NaN, 0 );
  glClear(GL_COLOR_BUFFER_BIT);

  GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT};
  glDrawBuffers(2, drawBuffers);
  glClear(GL_DEPTH_BUFFER_BIT);

  draw_scene(cgTechniqueWorldPosNormal);

  double trace_start;

  // ray trace shadows from world space buffer
  if(doReflections) {
    try {
      if (stat_breakdown) 
      {
        glFinish();
        sutilCurrentTime(&trace_start);
      }
      rtContext->launch(0, traceWidth, traceHeight);
    } catch (optix::Exception& e) {
      sutilReportError( e.getErrorString().c_str() );
      exit(1);
    }
  }
  double blur_pass_start;
  if (stat_breakdown) 
  {
    glFinish();
    sutilCurrentTime(&blur_pass_start);
  }

  // copy reflection map data from rt buffer to texture
  glPushAttrib(GL_PIXEL_MODE_BIT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, reflectionMapPBO);
  glBindTexture(GL_TEXTURE_2D, reflectionMapTex);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, traceWidth, traceHeight,
                  GL_RGBA, GL_FLOAT, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  glPopAttrib();

  // draw final colors
  glViewport(0,0, rasterWidth, rasterHeight);

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glDrawBuffer(GL_BACK);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,sampleFbo);
  float clear_value = 1.f;
  glClearColor(clear_value, clear_value, clear_value, clear_value);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  draw_scene(cgTechniqueGlossyReflections);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, (int)512, (int)512);
  glBindTexture(GL_TEXTURE_2D, sampleTex);
  glGenerateMipmapEXT(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D); 
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

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
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,0);

  double finish_start;
  if (stat_breakdown) 
  {
    glFinish();
    sutilCurrentTime(&finish_start);
  }

  // draw fps
  if(drawFps && animate) {
    static double last_frame_time = 0;
    static int frame_count = 0;
    static char fps_text[32];
    static char text1[32];
    double current_time;
    sutilCurrentTime(&current_time);
    double dt = current_time - last_frame_time;
    ++frame_count;
    if(dt > 0.5) {
      sprintf( fps_text, "fps: %6.1f", frame_count / ( current_time - last_frame_time ) );
      sprintf( text1, "frame time   : %6.5f", (current_time-last_frame_time)/frame_count);
   
      last_frame_time = current_time;
      frame_count = 0;
    }

    static char text2[32];
    static char text3[32];
    static char text4[32];
    if(dt > 0.5 && stat_breakdown) {
      sprintf( text2, "first pass   : %6.5f", trace_start-display_start);
      sprintf( text3, "raytrace pass: %6.5f", blur_pass_start-trace_start);
      sprintf( text4, "blur pass    : %6.5f", finish_start-blur_pass_start); 
    }

    float offset=12.f;
    float start=10.f;
    int index=0;
    drawText( fps_text, 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
    if (stat_breakdown) {
      drawText( text1   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
      drawText( text4   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
      drawText( text3   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
      drawText( text2   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
      drawText( "Press 'i' to toggle frame break down timings", 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
    }

  }
  if(g_Recordpic){
	  g_Recordpic=false;
	  BYTE *pTexture = NULL;											
	  pTexture = new BYTE[512 * 512 * 3];
	  memset(pTexture, 0, 512 * 512 * 3 * sizeof(BYTE));
	  glReadPixels(0,0,512,512,GL_RGB,GL_UNSIGNED_BYTE,pTexture);
	  char str[100];
	  sprintf(str,"optix.bmp");	
	  SaveBMP(str,pTexture,512,512);
	  delete [] pTexture;	
  }
  glutSwapBuffers();

  CHECK_ERRORS();
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
  g_Camera.Update();
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
	 g_Recordpic = !g_Recordpic;
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
	  printf("{make_float3(%f,%f,%f  ),make_float3(%f,%f,%f )},\n",g_Camera.Position().x,g_Camera.Position().y,g_Camera.Position().z,g_Camera.View().x,g_Camera.View().y,g_Camera.View().z);
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
 // manipulator.motion(x,y);
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
  glutInit(&argc, argv);
  freopen("stdout.txt","w",stdout);
 freopen("stderr.txt","w",stderr);
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
    cgfx_path = std::string( sutilSamplesDir() ) + "/isgReflections/isgReflections.cgfx";
  }

  CGFX_PATH = cgfx_path;
  GROUNDTEX_PATH = std::string( sutilSamplesDir() ) + "/isgReflections/ground.ppm";
  WALLTEX_PATH = std::string( sutilSamplesDir() ) + "/isgReflections/wall.ppm";

  glutInitWindowSize(512, 512);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("isgReflections");

  init_gl();
  init_scene(filename.c_str());
  init_optix();
  	
  CHECK_ERRORS();
  glutReportErrors();

  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
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
