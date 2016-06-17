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
	
	inline nv::vec2i getRes()
	{
		return m_res;
	}
	inline void setEuqal(int equal)
	{
		m_histogramPass.setEqual(equal);
	}
	inline void setCompareColor(nv::vec3f color)
	{
		m_histogramPass.setCompareColor(color);
	}
private:
	nv::vec2i m_res;
	Histogram m_histogramPass;
	Fbo m_fbo;
	GLuint m_query_id;

};
#endif