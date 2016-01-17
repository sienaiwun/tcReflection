#include <string>
#include <vector>
#include "Geometry.h"
#include "Camera.h"
#include "glslShader.h"
#include "MyMeterial .h"
#include <vector> 
#ifndef SCENEH
#define SCENEH
class TimeMesure;
class scene
{
	
public:
	scene();
	void optixInit() ;
	void init() ;
	void draw_model(glslShader& shader,CCamera* pCamera = 0);
	void draw_model(CGtechnique& tech,CCamera* pCamera = 0);
	virtual void cameraControl(int currentTime,CCamera& NowCamera);
	inline void setOptix(optix::Context * p)
	{
		pContext = p;
	}
	inline int getObjectNum()
	{
		return m_objectNum;
	}
	inline optix::GeometryGroup getOptixGroup()
	{
		return m_geometrygroup;
	}
	inline nv::vec3f getLightPos()
	{
		return m_lightPos;
	}
	inline void setMainLightPos(nv::vec3f pos)
	{
		m_lightPos = pos;
	}
	inline void setTimeMesure(TimeMesure * p)
	{
		m_ptimeMesure = p;
	}
	MyGeometry getObject(int index)
	{
		assert(index<m_objectNum);
		return m_geometryArray[index];
	}
	std::vector<MyGeometry >m_geometryArray;
	int m_objectNum;
	int m_texNum;
	nv::vec3f m_bbMin;
	nv::vec3f m_bbMax;
	nv::vec3f m_lightPos;
	optix::GeometryGroup m_geometrygroup;
	optix::Context *pContext;
	CCamera m_refCamera;
	CCamera m_curCamera;
	

protected:
	GeoPara * m_pObjectDis;
	MyMeterial * m_pMatDis;
	posPara * m_posArray;
	TimeMesure * m_ptimeMesure;
	std::vector<int> m_timeSequence;
	int m_timeSlice;
	
};
#endif