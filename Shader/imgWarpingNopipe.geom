#version 430

layout (triangles) in ;
layout (triangle_strip, max_vertices = 3) out;
uniform vec2 resolution;
uniform bool isNaive;
in int isEdgeValue[];;
//uniform sampler2D IdMap;
in vec2 Texcoord[];
in vec2 vecCoord[];
in float isRed[];
out vec2 Texcoordout;
out float isInValid;

float THRESHOLD = 0.5;

#define CONVERGE 1
#define OUTRANGE 2
#define OUTOBJECT 3
#define FASTPROJT 4

#define MESHTRRESHOLD (7.5)
bool isOutSide(vec2 tc)
{
	 return (abs(tc.x)>1||abs(tc.y)>1);
	   
}
bool isInBorder(vec2 tc)
{
	return (tc.x>(1.0-1.0/resolution.x)||tc.x<1.0/resolution.x||tc.y>(1.0-1.0/resolution.x)||tc.y<1.0/resolution.y);
		
}
void main()
{		
    if(isOutSide(vecCoord[0])||isOutSide(vecCoord[1])||isOutSide(vecCoord[2]))
		return;
	if(isInBorder(Texcoord[0])||isInBorder(Texcoord[1])||isInBorder(Texcoord[2]))
		return;
    isInValid = 0;
	if(abs(vecCoord[0].y - vecCoord[1].y)>MESHTRRESHOLD/resolution.y ||abs(vecCoord[0].y - vecCoord[2].y)>MESHTRRESHOLD/resolution.y||abs(vecCoord[2].y - vecCoord[1].y)>MESHTRRESHOLD/resolution.y)
	{
     	isInValid = 1;
		if(isNaive)
			return;
	}
	if(isEdgeValue[0]+isEdgeValue[1]+isEdgeValue[2]>=1.9)
	{
		isInValid = 1;
		if(isNaive)
			return;
	}
	if(abs(vecCoord[0].x - vecCoord[1].x)>MESHTRRESHOLD/resolution.x ||abs(vecCoord[0].x - vecCoord[2].x)>MESHTRRESHOLD/resolution.x ||abs(vecCoord[2].x - vecCoord[1].x)>MESHTRRESHOLD/resolution.x)	
	{ 
	    isInValid = 1;
		if(isNaive)
			return;
	}
    for(int i = 0;i<gl_in.length();i++)
	{
		gl_Position = gl_in[i].gl_Position;
		Texcoordout = Texcoord[i];
		EmitVertex();
	}
	EndPrimitive();
}