#version 430

layout (triangles) in ;
layout (triangle_strip, max_vertices = 3) out;
uniform vec2 resolution;

//uniform sampler2D IdMap;
in vec2 Texcoord[];
in vec2 vecCoord[];

out vec2 Texcoordout;

float THRESHOLD = 0.5;

#define CONVERGE 1
#define OUTRANGE 2
#define OUTOBJECT 3
#define FASTPROJT 4


#define MESHTRRESHOLD (3.5)
void main()
{		
	if(abs(vecCoord[0].y - vecCoord[1].y)>MESHTRRESHOLD/resolution.y ||abs(vecCoord[0].y - vecCoord[2].y)>MESHTRRESHOLD/resolution.y||abs(vecCoord[2].y - vecCoord[1].y)>MESHTRRESHOLD/resolution.y)
		return;
	if(abs(vecCoord[0].x - vecCoord[1].x)>MESHTRRESHOLD/resolution.x ||abs(vecCoord[0].x - vecCoord[2].x)>MESHTRRESHOLD/resolution.x ||abs(vecCoord[2].x - vecCoord[1].x)>MESHTRRESHOLD/resolution.x)
		return;

    for(int i = 0;i<gl_in.length();i++)
	{
		gl_Position = gl_in[i].gl_Position;
		Texcoordout = Texcoord[i];
		EmitVertex();
	}
	
	EndPrimitive();
}