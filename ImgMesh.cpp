#include "ImgMesh.h"
#include "glslShader.h"
ImgMesh::ImgMesh(int w,int h):m_w(w),m_h(h)
{

}
#define EDGEDELTA 0.0
void ImgMesh::init()
{
	int w = m_w,h = m_h;
	float2 *Vertices;
	Vertices = new float2[m_w *m_h * 6];
	int PointId = 0;
	for(int x = 0;x<w;x++)
		for(int y = 0;y<h;y++)
		{
			/*if(x==476&&y==326)
				;
			else
				continue;*/
			Vertices[PointId++] = make_float2((float)(x+0.5-EDGEDELTA)/w,(float)(y+0.5-EDGEDELTA)/h);
			Vertices[PointId++] =  make_float2((float)(x+1.5+EDGEDELTA)/w,(float)(y+0.5-EDGEDELTA)/h);
			Vertices[PointId++] =  make_float2((float)(x+1.5+EDGEDELTA)/w,(float)(y+1.5+EDGEDELTA)/h);
			
			Vertices[PointId++] = make_float2((float)(x+0.5-EDGEDELTA)/w,(float)(y+0.5-EDGEDELTA)/h);
			Vertices[PointId++] =  make_float2((float)(x+1.5+EDGEDELTA)/w,(float)(y+1.5+EDGEDELTA)/h);
			Vertices[PointId++] =  make_float2((float)(x+0.5-EDGEDELTA)/w,(float)(y+1.5+EDGEDELTA)/h);
			
		}
	// 		cout<<Vertices[1].x<<" "<<Vertices[1].y<<endl;
	glGenBuffers(1,&m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER,m_vboId);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float2)* w *h * 6,(GLvoid*)Vertices,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	delete [] Vertices;
}
void ImgMesh::drawImgMesh()
{
	int w = m_w,h = m_h;
	glEnableClientState(GL_VERTEX_ARRAY);;
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glVertexPointer  ( 2, GL_FLOAT, 0, (char *) 0);
	glDrawArrays(GL_TRIANGLES, 0, w*h*6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void ImgMesh::drawImgMesh(glslShader& shader)
{
	shader.begin();
	drawImgMesh();
	shader.end();
}