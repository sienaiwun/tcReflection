#include <string>
#include <vector>
#include "Geometry.h"
#include "MyMeterial .h"
#ifndef SCENEH
#define SCENEH
class scene
{
	
public:
	scene()
	{
		
	}
	 void optixInit() ;
	 void init() ;
	void draw_model(glslShader& shader);
	void draw_model(CGtechnique& tech);
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
protected:
	GeoPara * m_pObjectDis;
	MyMeterial * m_pMatDis;
};
#endif