#include "scene.h"
#include <algorithm>
scene::scene()
{
		m_pObjectDis = 0;
		m_pMatDis = 0;
		m_posArray = 0;
		m_ptimeMesure = 0;
		m_hasGlossy = 0;
		
}
void scene::draw_model(glslShader& shader,CCamera *pCamera)
{
	CHECK_ERRORS();
	shader.begin();
	if(pCamera != NULL)	
	{
		pCamera->Look();
		CHECK_ERRORS();
		shader.setCamera(pCamera);
		CHECK_ERRORS();
	}
	CHECK_ERRORS();
	shader.setScene(this);
	for(int i =0;i<m_objectNum;i++)
	{
		shader.setGeometry(&m_geometryArray[i]);
		CHECK_ERRORS();
		shader.setGeomtryIndex(i);
		CHECK_ERRORS();
		m_geometryArray[i].drawGeometry(shader,0);
	}
	shader.end();
}

void scene::cameraControl(int currentTime,CCamera& NowCamera)
{
	static bool once = true;
	assert(m_timeSequence.size()>0);
	assert(m_posArray);
	
	std::vector<int>::iterator up;
	//low = std::lower_bound (m_timeSequence.begin(), m_timeSequence.end(), currentTime); //   m_timeSequence.
	up = std::upper_bound  (m_timeSequence.begin(), m_timeSequence.end(), currentTime); //   m_timeSequence.
	int lowId = std::distance(m_timeSequence.begin(),up)-1;
	int highId = lowId+1;
	//if(once)
	{
		NowCamera.navigate(m_posArray[lowId],m_posArray[highId],currentTime,m_timeSequence[lowId],m_timeSequence[highId]);
		once = false;
	}
	NowCamera.Look();
}
void scene::update()
{
	m_refCamera.Update();
	m_curCamera.Update();
}
void scene::draw_model(CGtechnique& tech,CCamera *pCamera)
{
	// to do
}

void scene::init()
{
	MyGeometry::setMatArray(m_pMatDis);
	m_geometryArray.resize(m_objectNum);
	for(int i = 0;i<m_objectNum;i++)
	{
		m_geometryArray[i].setId(i);
		m_geometryArray[i].initGeometry(m_pObjectDis[i]);
	}
	m_bbMin = MyGeometry::modelBBMin,m_bbMax = MyGeometry::modelBBMax;
	cameraControl(0,m_curCamera);
	cameraControl(0,m_refCamera);
}
void scene::optixInit()
{
	std::vector<optix::GeometryInstance> geoInstance;
	geoInstance.resize(m_objectNum);
	for(int i =0;i<m_objectNum;i++)
	{
		geoInstance[i] = m_geometryArray[i].getInstance();
	}
	m_geometrygroup = (*pContext)->createGeometryGroup();
	m_geometrygroup->setChildCount(m_objectNum);
	for(int i=0;i<m_objectNum;i++)
			m_geometrygroup->setChild(i, geoInstance[i]);
	m_geometrygroup->setAcceleration( (*pContext)->createAcceleration("Bvh","Bvh") );
	(*pContext)["reflectors"]->set(m_geometrygroup);
	(*pContext)["lightPos"]->set3fv((const float*)&make_float3(getLightPos().x,
			getLightPos().y,
			getLightPos().z));
	(*pContext)["hasGlossy"]->setInt(m_hasGlossy);
}
//
//void updateGeometry( )
//{
//	//We know that we have (only) one group in this example
//	for(int i =0;i<geoNumber;i++)
//	{
//		if(geo[i].actionNumber)
//		{
//			int actionNum = geo[i].actionNumber;
//			GeometryInstance geometryInstance = geometrygroup->getChild( i );
//			Geometry geometry = geometryInstance->getGeometry();
//			Buffer vertexBuffer = geometry["vertex_buffer"]->getBuffer();
//			float3* new_vertices = (float3*)vertexBuffer->map();
//
//			RTsize numVertices;
//			vertexBuffer->getSize( numVertices );
//
//			float* originData = (float* ) geo[i].m_model->getCompiledOptixVertices();
//			ActArray[actionNum].draw((float*)new_vertices,originData,numVertices);
//			vertexBuffer->unmap();
//
//
//			geometry->markDirty();
//		}
//	}
//	geometrygroup->getAcceleration()->markDirty();
//	//	 geo[i].drawGeometry(tech,geo[i].actionNumber);
//	return;
//	{
//		GeometryInstance geometryInstance = geometrygroup->getChild( 0 );
//		Geometry geometry = geometryInstance->getGeometry();
//
//
//
//		Buffer vertexBuffer = geometry["vertex_buffer"]->getBuffer();
//		float3* new_vertices = (float3*)vertexBuffer->map();
//
//		RTsize numVertices;
//		vertexBuffer->getSize( numVertices );
//
//		float* originData = (float* ) geo[0].m_model->getCompiledOptixVertices();
//
//		//We don't have to set x and z here in this example
//		/* for(unsigned int v = 0; v < numVertices; v++)
//		{
//		int posIndex = 3*v;
//		new_vertices[posIndex].y = originData[9*v+1]+ delta ;
//		// float4x4 matrix =
//		}*/
//
//		ActArray[1].draw((float*)new_vertices,originData,numVertices);
//		vertexBuffer->unmap();
//
//
//		geometry->markDirty();
//
//
//	}
//	{
//		GeometryInstance geometryInstance = geometrygroup->getChild( 1 );
//		Geometry geometry = geometryInstance->getGeometry();
//
//
//
//		Buffer vertexBuffer = geometry["vertex_buffer"]->getBuffer();
//		float3* new_vertices = (float3*)vertexBuffer->map();
//
//		RTsize numVertices;
//		vertexBuffer->getSize( numVertices );
//
//		float* originData = (float* ) geo[1].m_model->getCompiledOptixVertices();
//
//		//We don't have to set x and z here in this example
//		ActArray[1].draw((float*)new_vertices,originData,numVertices);
//
//
//
//		vertexBuffer->unmap();
//
//
//		geometry->markDirty();
//
//
//	}
//	geometrygroup->getAcceleration()->markDirty();
//}