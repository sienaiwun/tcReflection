#include "fboAnalyse.h"
void FboAnalyse::init()
{
	m_histogramPass.init();
	m_fbo.init();
	glGenQueries( 1,&m_query_id);
}

int FboAnalyse::analyseColorNum(Fbo& sourceFbo,int slotNum)
{
	unsigned int ret;

	//sourceFbo.SaveBMP("./test/test.bmp",0);
	glBeginQuery( GL_SAMPLES_PASSED ,m_query_id);
	m_fbo.begin();
	m_histogramPass.setReflectTex(sourceFbo.getTexture(slotNum));
	MyGeometry::drawQuad(m_histogramPass);
	//analyseFbo.debug(0,"analyseFbo.bmp");
	
	CHECK_ERRORS();
	glEndQuery(GL_SAMPLES_PASSED);
	GLint ready;
	int count = 1000;
	do {
		glGetQueryObjectiv( m_query_id, GL_QUERY_RESULT_AVAILABLE, &ready);
	} while ( !ready&& count-- );
	glGetQueryObjectuiv (m_query_id, GL_QUERY_RESULT, &ret);
	
	m_fbo.end();

	/*
	m_fbo.SaveBMP("test.bmp",0);
	
	int w = m_res.x;
	int h = m_res.y;
	glEnable(GL_TEXTURE_2D);
	BYTE *pTexture = NULL;
	pTexture = new BYTE[w*h * 3];
	memset(pTexture, 0, w*h * 3 * sizeof(BYTE));

	glBindTexture(GL_TEXTURE_2D, sourceFbo.getTexture(0));//TexPosId   PboTex

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture);

	int totalnum = 0;
	Fbo::SaveBMP("./test/123.bmp", pTexture, w, h);
	for(int y =0;y<h;y++)
	{
		for(int x=  0;x<w;x++)
		{
			int index = y*w+x;
			BYTE r = pTexture[3*index];
			BYTE g = pTexture[3*index+1];
			BYTE b = pTexture[3*index+2];
			if((r==0)&&(g==0)&&(b==255))
			{
				
			}
			else
				totalnum++;
		}
	}
	if (pTexture)
	   delete[] pTexture;
	glBindTexture(GL_TEXTURE_2D, 0);
	*/
//	m_fbo.SaveBMP("./test/histogram.bmp",0);
	return ret;
}