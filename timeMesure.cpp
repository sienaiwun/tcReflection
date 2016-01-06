#include "timeMesure.h"



static void drawText( const std::string& text, float x, float y, void* font )
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

static char fps_text[32];


static char text1[32];
static char text2[32];
static char text3[32];
static char text4[32];
static char text5[32];
static char text6[32];

void TimeMesure::printOptix()
{
	float fps;
	fps = 1000/(m_frameEndTime - m_lastFrameEndTime);
	m_fcount.insertTime(m_frame,fps);
	sprintf( fps_text, "%d fps: %f",m_frame,fps);
	sprintf( text1, "frame time   : %6.5f", (m_frameEndTime - m_lastFrameEndTime));
	sprintf( text2, "first pass   : %6.5f", m_traceBeginTime-m_frameBeginTime);
	sprintf( text3, "raytrace pass: %6.5f", m_finalRenderingBeginTime-m_traceBeginTime);
	sprintf( text4, "blur pass    : %6.5f", m_frameEndTime-m_finalRenderingBeginTime); 

	float offset=12.f;
	float start=10.f;
	int index=0;
	drawText( fps_text, 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
	drawText( text1   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
    drawText( text4   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
	drawText( text3   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
    drawText( text2   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
}
void TimeMesure::printTC()
{
	float fps;
	float offset=12.f;
	float start=10.f;
	int index=0;
	fps = 1000/(m_frameEndTime - m_lastFrameEndTime);
	m_fcount.insertTime(m_frame,fps);
	sprintf( fps_text, "%d fps: %f",m_frame,fps);
	sprintf( text1, "frame time   : %6.5f", (m_frameEndTime - m_lastFrameEndTime));
	sprintf( text2, "first pass   : %6.5f", m_cudaBeginTime-m_frameBeginTime);
	sprintf( text3, "cuda pass   : %6.5f",m_forwardTime -m_cudaBeginTime);
	sprintf( text4, "projection pass    : %6.5f", m_secondTraceTime-m_forwardTime); 
	sprintf( text5, "second optix pass: %6.5f ",m_finalRenderingBeginTime- m_secondTraceTime);
	sprintf( text6, "final rendering pass: %6.5f",m_frameEndTime - m_finalRenderingBeginTime);

	drawText( fps_text, 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
	drawText( text1   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
	drawText( text6   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
    drawText( text5   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
    drawText( text4   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13 );
    drawText( text3   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13);
	drawText( text2   , 10.0f, start+offset*index++, GLUT_BITMAP_8_BY_13);
	

}
void TimeMesure::print()
{
	if(m_type == optixRenderingType)
	{
		printOptix();
	}
	else if (m_type == tcRenderingType)
	{
		printTC();
	}
}
void TimeMesure::updateLastTime()
{
	m_lastFrameEndTime = m_frameEndTime;
	if(m_frame>= m_totalTime)
	{
		finishFuc();
	}
}
void TimeMesure::previousFrame()
{
	m_frame--;
	m_frame = std::max<int>(0,m_frame);
	
}
void TimeMesure::nextFrame()
{
	//m_frame++;
}
void TimeMesure::finishFuc()
{
	m_fcount.countResult();
	float avg = m_fcount.getAverge();
	float maxFps = m_fcount.getMaximum();
	float minFps = m_fcount.getMinimum();
	sprintf( fps_text, "max%f min: %6.1f",maxFps,minFps);
	sprintf( text1, "avg time   : %6.5f", avg);
	printf(  "max%f min: %6.1f\n",maxFps,minFps);
	printf(  "avg time   : %6.5f\n", avg);
	exit(0);
}