#include"CFbo.h"
#include "assert.h"

//#include "SaveBMP.h"

void mySaveBMP(char* fileName,BYTE * buf,UINT width,UINT height);

CFbo::CFbo(int _bufferWidth, int _bufferHeight, int _numBuffers)
:bufferWidth(_bufferWidth),bufferHeight(_bufferHeight),numBuffers(_numBuffers)
{
	texID = new GLuint[_numBuffers];
}

CFbo::~CFbo()
{
	glDeleteFramebuffersEXT(1,&fboID);
	glDeleteRenderbuffersEXT(1,&depthID);
	glDeleteTextures(numBuffers,texID);
	delete[] texID;
}

void CFbo::init()
{
	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	glGenTextures (numBuffers, texID);
	for(int i=0;i<numBuffers;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, bufferWidth, bufferHeight, 0,
			GL_RGBA, GL_FLOAT, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i/*COLOR_ATTACHMENT[i]*/, GL_TEXTURE_2D, texID[i], 0);
	}
	//申请Z BUFFER
	glGenRenderbuffersEXT(1,&depthID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,depthID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,bufferWidth,bufferHeight);
	//将深度缓冲与FBO绑定
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,depthID);

	//check status
	CheckFramebufferStatus();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void CFbo::init_ZBufferFree()
{
	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	glGenTextures (numBuffers, texID);
	for(int i=0;i<numBuffers;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, bufferWidth, bufferHeight, 0,
			GL_RGBA, GL_FLOAT, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i/*COLOR_ATTACHMENT[i]*/, GL_TEXTURE_2D, texID[i], 0);
	}
	//check status
	CheckFramebufferStatus();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void CFbo::init_Layered_ZBFree(GLint internalFormat)
{
	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	glGenTextures (numBuffers, texID);
	for(int i=0;i<numBuffers;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat/*GL_RGBA32F_ARB*//*GL_ALPHA32F_ARB*/, bufferWidth>>i, bufferHeight>>i, 0,
			GL_RGBA, GL_FLOAT, 0);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, /*GL_COLOR_ATTACHMENT0_EXT+i*/COLOR_ATTACHMENT[i], GL_TEXTURE_2D, texID[i], 0);
	}
	//check status
	CheckFramebufferStatus();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void CFbo::begin(int colorAttachment, int numDrawBuffers)
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&preFbo);//获取当前的FBO，用于还原
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID); 
	if(numDrawBuffers==1)
       glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + colorAttachment);
	else
	   glDrawBuffers(numDrawBuffers,COLOR_ATTACHMENT + colorAttachment);

	glPushAttrib(/*GL_ALL_ATTRIB_BITS*/GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0,0,bufferWidth,bufferHeight);
}
void CFbo::end()
{
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, preFbo);
}

/*
 *	glBindFramebufferEXT
 *	glPushAttrib(GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT)
 */
void CFbo::bind()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&preFbo);//获取当前的FBO，用于还原
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	glPushAttrib(/*GL_ALL_ATTRIB_BITS*/GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
 *	undo glBindFramebufferEXT
 *	glPopAttrib
 */
void CFbo::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, preFbo);
	glPopAttrib();
}

/*
 *	glViewport
 *	glDrawBuffer or glDrawBuffers
 */
void CFbo::draw( int x, int y, int width, int height, int colorAttachment /*= 0*/, int numDrawBuffers /*= 1*/ )
{
	glViewport(x,y,width,height);
	if(numDrawBuffers==1)
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + colorAttachment);
	else
		glDrawBuffers(numDrawBuffers,COLOR_ATTACHMENT + colorAttachment);
}

GLuint CFbo::getFboID()
{
	return this->fboID;
}
GLuint CFbo::getTexID(int texIdx)
{
	return this->texID[texIdx];
}
void CFbo::showTextureOnScreen( int x,int y,int width /*= 128*/,int height /*= 128*/,GLuint tID /*= 0*/,GLenum type /*= GL_TEXTURE_2D*/ )
{
//	glPushAttrib(GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
	glDisable(GL_DEPTH_TEST);
	glEnable(type);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(x,y,width,height);
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
	    glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0,1.0,0.0,1.0); 
    glBindTexture(type,texID[tID]);
    glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1,0);glVertex2f( 1.0f, -1.0f);
		glTexCoord2f(1,1);glVertex2f( 1.0f,  1.0f);
		glTexCoord2f(0,1);glVertex2f(-1.0f,  1.0f);
    glEnd();
   	glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
    glMatrixMode(GL_PROJECTION);
	    glPopMatrix();
	glPopAttrib();
	glDisable(type);
	glEnable(GL_DEPTH_TEST);
}

void CFbo::readPixel( int tIdx,int x, int y, int width, int height )
{
	float *pData = NULL;
	pData = new float[width * height * 4];
	//memset(pData, 0, width * height * 4 * sizeof(float));

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID); 
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
		GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, texID[tIdx], 0);
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0,0,width,height,GL_RGBA,GL_FLOAT,pData);

	//int pos = 0;
	//for (int i = 0;i < height; ++i)
	//{
	//	for (int j = 0;j < width; ++j)
	//	{
	//		printf(" <%f,%f,%f,%f> ",pData[pos++],pData[pos++],pData[pos++],pData[pos++]);
	//	}
	//}
	for (int i = 0;i < 4;++i)
	{
		printf("%f ",pData[i]);
	}
	printf("\n");

	delete [] pData;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//////////////////////////////////////////////////////////////////////////

	float aabb[4];
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,texID[tIdx], 0);

	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glReadPixels(0,0,1, 1,GL_RGBA,GL_FLOAT,aabb);
	//printf("aabb_min: %f %f %f %f\n",aabb[0],aabb[1],aabb[2],aabb[3]);
	for (int i = 0;i < 4;++i)
	{
		printf("%f ",aabb[i]);
	}
	printf("\n");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void CFbo::CheckFramebufferStatus()
{
	int status;
	status = (GLenum) glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			printf("Unsupported framebuffer format\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			printf("Framebuffer incomplete, missing attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			printf("Framebuffer incomplete, incomplete attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			printf("Framebuffer incomplete, attached images must have same dimensions\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			printf("Framebuffer incomplete, attached images must have same format\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			printf("Framebuffer incomplete, missing draw buffer\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			printf("Framebuffer incomplete, missing read buffer\n");
			break;
		default:
			exit(0);
	}
}

void CFbo::FboSaveBMP( char* filename, int id){

	glEnable(GL_TEXTURE_2D);
	BYTE *pTexture = NULL;
	pTexture = new BYTE[bufferWidth*bufferHeight * 3];
	memset(pTexture, 0, bufferWidth*bufferHeight * 3* sizeof(BYTE));

	glBindTexture(GL_TEXTURE_2D, texID[id]);//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	mySaveBMP(filename, pTexture, bufferWidth, bufferHeight);
	if (pTexture)
		delete[] pTexture;
	glBindTexture(GL_TEXTURE_2D, 0);//TexPosId   PboTex
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);




}

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
void mySaveBMP(char* fileName,BYTE * buf,UINT width,UINT height)
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

		free(buf);

		fclose(fp);
}