#include "ImgMesh.h"
#include "glslShader.h"
//#define TRIANGLE
ImgMesh::ImgMesh(int w,int h):m_w(w),m_h(h)
{

}
#define EDGEDELTA 0.0
#define INDEX(Y,X) (Y)*w+(X)
void ImgMesh::init()
{
	int w = m_w,h = m_h;
#ifdef TRIANGLE
	float2 *Vertices;
	Vertices = new float2[m_w *m_h];
	int PointId = 0;
	for(int y = 0;y<h;y++)
	{
		for(int x = 0;x<w;x++)
		{
			Vertices[PointId++] = make_float2((float)(x+0.5-EDGEDELTA)/w,(float)(y+0.5-EDGEDELTA)/h);
		}
	}
	glGenBuffers(1,&m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER,m_vboId);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float2)* w *h ,(GLvoid*)Vertices,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	delete [] Vertices;

	
	int * VerticexId;
	VerticexId = new int[m_h*(m_w)*6];	
	PointId = 0;
	for(int y = 0;y<h;y++)
	{
		
		for(int x = 0;x<w;x++)
		{
			VerticexId[PointId] = INDEX(y,x);
			VerticexId[PointId+1] = INDEX(y+1,x+1);

			VerticexId[PointId+2] = INDEX(y+1,x);
			VerticexId[PointId+3] = INDEX(y,x);

			VerticexId[PointId+4] = INDEX(y,x+1);
			VerticexId[PointId+5] = INDEX(y+1,x+1);
			PointId+=6;
		}
	}
	glGenBuffers(1, &m_vboindexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboindexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		6*w*h*sizeof(int),
		(GLvoid*)VerticexId, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#else
	float2 *Vertices;
	Vertices = new float2[m_w *m_h *2];
	int PointId = 0;
	for(int y = 0;y<h;y++)
	{
		for(int x = 0;x<w;x++)
		{
			Vertices[PointId++] = make_float2((float)(x+0.5-EDGEDELTA)/w,(float)(y+0.5-EDGEDELTA)/h);
			Vertices[PointId++] =  make_float2((float)(x+0.5+EDGEDELTA)/w,(float)(y+1.5+EDGEDELTA)/h);
		}
	}
	glGenBuffers(1,&m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER,m_vboId);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float2)* w *h * 2,(GLvoid*)Vertices,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	delete [] Vertices;

	
	int * VerticexId;
	VerticexId = new int[m_h*(m_w)*2];	
	PointId = 0;
	for(int y = 0;y<h;y++)
	{
		
		for(int x = 0;x<w;x++)
		{
			VerticexId[PointId] = PointId;PointId++;
			VerticexId[PointId] = PointId;PointId++;
		}
	}
	glGenBuffers(1, &m_vboindexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboindexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		2*w*h*sizeof(int),
		(GLvoid*)VerticexId, GL_STATIC_READ);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#endif

}
void ImgMesh::drawImgMesh()
{
	int w = m_w,h = m_h;
#ifdef TRIANGLE
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glEnableVertexAttribArray(0);
	//glVertexPointer  ( 2, GL_FLOAT, 0, (char *) 0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float2), (void*) 0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledNormalOffset()*sizeof(float)));
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledTexCoordOffset()*sizeof(float)));

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vboindexId);
	glDrawElements( GL_TRIANGLES, m_h*(m_w)*6, GL_UNSIGNED_INT, 0 );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	CHECK_ERRORS();
#else

	

	//   with id one
	/*
	glDisable(GL_CULL_FACE);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glEnableVertexAttribArray(0);
	//glVertexPointer  ( 2, GL_FLOAT, 0, (char *) 0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float2), (void*) 0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledNormalOffset()*sizeof(float)));
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledTexCoordOffset()*sizeof(float)));

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vboindexId);
	glDrawElements( GL_TRIANGLE_STRIP, m_h*(m_w)*2, GL_UNSIGNED_INT, 0 );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	CHECK_ERRORS();
	*/
	
	glDisable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);;
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glVertexPointer  ( 2, GL_FLOAT, 0, (char *) 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, w*h*2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	/*

	CHECK_ERRORS();
	glBindBuffer(GL_ARRAY_BUFFER, m_ModelVb);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledPositionOffset()*sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledNormalOffset()*sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledTexCoordOffset()*sizeof(float)));

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ModelIb);

	
	glDrawElements( GL_TRIANGLES, m_model->getCompiledIndexCount(), GL_UNSIGNED_INT, 0 );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glDisableVertexAttribArray(2);
	
	CHECK_ERRORS();
	*/
#endif

}
void ImgMesh::drawImgMesh(glslShader& shader)
{
	shader.begin();
	drawImgMesh();
	shader.end();
}