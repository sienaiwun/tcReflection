#include "scene.h"
void scene::draw_model(glslShader& shader)
{
	for(int i =0;i<m_objectNum;i++)
	{
		m_geometryArray[i].drawGeometry(shader,0);
	}
}
void scene::draw_model(CGtechnique& tech)
{
	extern CGparameter cgModelIdParam;
	for(int i =0;i<m_objectNum;i++)
	{

		cgSetParameter1f(cgModelIdParam, 1.0 * i);
		m_geometryArray[i].drawGeometry(tech,0);
	}
}

void scene::init()
{
	MyGeometry::setMatArray(m_pMatDis);
	m_geometryArray.resize(m_objectNum);
	for(int i = 0;i<m_objectNum;i++)
	{
		m_geometryArray[i].initGeometry(m_pObjectDis[i]);
	}
	m_bbMin = MyGeometry::modelBBMin,m_bbMax = MyGeometry::modelBBMax;
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