#ifndef __CFBO_H__
#define __CFBO_H__
#include<stdio.h>
#include<windows.h>
// #ifndef __glew_h__
// #include<GL\glew.h>
// #endif
#include "gl/GLee.h"
#include <gl/gl.h>
#include <gl/GLU.h>

//#include "SaveBMP.h"

//#include <gl/glext.h>



static const GLenum COLOR_ATTACHMENT[] = {
	GL_COLOR_ATTACHMENT0_EXT,
	GL_COLOR_ATTACHMENT1_EXT,
	GL_COLOR_ATTACHMENT2_EXT,
	GL_COLOR_ATTACHMENT3_EXT,
	GL_COLOR_ATTACHMENT4_EXT,
	GL_COLOR_ATTACHMENT5_EXT,
	GL_COLOR_ATTACHMENT6_EXT,
	GL_COLOR_ATTACHMENT7_EXT,
	GL_COLOR_ATTACHMENT8_EXT,
	GL_COLOR_ATTACHMENT9_EXT,
	GL_COLOR_ATTACHMENT10_EXT,
	GL_COLOR_ATTACHMENT11_EXT,
	GL_COLOR_ATTACHMENT12_EXT,
	GL_COLOR_ATTACHMENT13_EXT,
	GL_COLOR_ATTACHMENT14_EXT,
	GL_COLOR_ATTACHMENT15_EXT
};

////最多支持十六张纹理
class CFbo
{
public:
	CFbo(int _bufferWidth,int _bufferHeight,int _numBuffers=1);
	~CFbo();
	void init();
	void init_ZBufferFree();
	void init_Layered_ZBFree(GLint internalFormat = /*GL_ALPHA32F_ARB*/GL_RGBA32F_ARB);
	void init_Custom();
	GLuint getFboID();
	GLuint getTexID(int texIdx=0);
	int getWidth() { return bufferWidth; }
	int getHeight() { return bufferHeight; }
	void begin(int colorAttachment=0, int numDrawBuffers=1);
	void end();
	void bind();
	void unbind();
	void draw(int x, int y, int width, int height, int colorAttachment = 0, int numDrawBuffers = 1);
	void showTextureOnScreen(int x,int y,int width = 128,int height = 128,GLuint tID = 0,GLenum type = GL_TEXTURE_2D);//using for DEBUG
	void readPixel(int tIdx,int x, int y, int width, int height);
	void FboSaveBMP (char* filename,int id);
private:
	void CheckFramebufferStatus();

	int bufferWidth,bufferHeight;
	GLuint fboID,depthID;
	GLint preFbo;
	GLuint *texID;
public:
	int numBuffers;
};
#endif