#include "Fbo.h"
#include "geometry.h"
#include "macro.h"
#include "histogram.h"
#ifndef FBO_ANALYSE_H
#define FBO_ANALYSE_H
class FboAnalyse
{
public:
	FboAnalyse(int w, int h)
	{
		m_fbo.set(1,w,h);
		m_res = nv::vec2i(w,h);
	}
	void init();
	int analyseColorNum(Fbo& sourceFbo,int slotNum);
	inline void setCheckColro1(nv::vec3f color1)
	{
		m_checkColor1 = color1;
	}
	inline void setCheckColro2(nv::vec3f color2)
	{
		m_checkColor2 = color2;
	}
	inline nv::vec2i getRes()
	{
		return m_res;
	}
private:
	nv::vec3f m_checkColor1, m_checkColor2;
	nv::vec2i m_res;
	Histogram m_histogramPass;
	Fbo m_fbo;
	GLuint m_query_id;

};
#endif