
#include<stdlib.h>
#include<GL/glew.h>
#include<GL/glut.h>
#include<GL/wglew.h>
#include<fstream>
//#include<glm/vec2.hpp>

#define CHECK_ERRORS()         \
	do {                         \
	GLenum err = glGetError(); \
	if (err) {                                                       \
	printf( "GL Error %d at line %d of FILE %s\n", (int)err, __LINE__,__FILE__);       \
	exit(-1);                                                      \
	}                                                                \
	} while(0)
#ifndef TEXSTATE_H
#define TEXSTATE_H

class TexState{
public:
	TexState(){};

	TexState(int w, int h, int f = GL_LINEAR, int c = GL_CLAMP) :width(w), height(h), filterMode(f), clampMode(c){

	};
	inline int getWidth(){ return width; };
	inline int getHeight(){ return height; };
	inline int getClamp(){ return clampMode; };
	inline int getFilterMode(){ return filterMode; };

private:
	int width, height;
	int filterMode, clampMode;
};
#endif

#ifndef FBO_H
#define FBO_H
static GLenum mybuffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
                           GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT, GL_COLOR_ATTACHMENT6_EXT };
class Fbo{
private:

	GLuint DepthTex;
	GLuint depthbuffer;
	int beforeFboId;
	TexState texDescript;
	       //绑定纹理的数量

public:
	int num;    
	//存储纹理
	GLuint TexId[8];
	GLuint fboId;   // 这个fbo的ID

	
	inline GLuint getFboId()
	{
		return fboId;
	}
	Fbo(){beforeFboId = 0;};
	Fbo(int num, int width, int height);
	void set(int num, int width,int height);
	~Fbo();
	inline TexState getTexState(){
		return texDescript;
	}
	//初始化 分配内存
	void init( GLenum FiterModel = GL_NEAREST);
	//启动fbo
	void attachId();
	void begin();

	//指定渲染buffer
	void BindForWrite(int i);
	void BindForRead(int i, GLenum TextureUnit);
	void copyFrom(Fbo & source,int i =0);
	void copyFromBuffer(Fbo & source,int i=0);
	//停用fbo

	void end();
	//static void SaveBMP(char *filename);
	void destory();
	//返回纹理指针
	GLuint getTexture(int id);
	//静态函数 存储bmp
	static void SaveBMP(const char *fileName, BYTE *buf, UINT width, UINT height);
	static void SaveBMP(GLuint texId, const char * fileName,UINT width,UINT height);
	void debugPixel(int id, int x,int y,int scale = 1);
	void SaveBMP(const char *fileName, int id);

	void SaveBuffToBMP(const char *fileName, int id);

	void SaveFloat(const char *fileName, int id);

	void CoutFloat(int);

	int ComputNum(int id, int lastNum);
	static void drawScreenBackBuffer(int w, int h);
	static void saveScreen(std::string fileName,int width,int height);
	static void mipMaptoSceen(GLuint texid,int widht,int height);
	//glm::vec2 ComputError(int);
};

#endif