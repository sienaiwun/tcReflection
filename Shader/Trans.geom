#version 430

layout (triangles) in ;
layout (triangle_strip, max_vertices = 3) out;
uniform vec2 resolution;

//uniform sampler2D IdMap;
in vec2 Texcoord[];
in float isDisCard[];
in float  ReDis[];
in vec2 TransVec[];
in int mode[];
out float isInValid;
out vec2 Texcoordout;
in int isEdgeValue[];;
float THRESHOLD = 0.5;

/*
out vec2 pos1;
out vec2 pos2;
out vec2 pos3;
out float area;
*/
#define CONVERGE 1
#define OUTRANGE 2
#define OUTOBJECT 3
#define FASTPROJT 4


#define MESHTRRESHOLD (25.5)
#define TRIANGLETHRESHOLD (25.5)
float triangleArea(vec2 pos1,vec2 pos2,vec2 pos3)
{
	vec2 AtoB = pos2 - pos1;
	vec2 AtoC=  pos3 - pos1;
	return abs(AtoB.y*AtoC.x-AtoB.x*AtoC.y)/2;
}

bool isOutSide(vec2 tc)
{
	 return (abs(tc.x)>1||abs(tc.y)>1);
	   
}
bool isInBorder(vec2 tc)
{
	return (tc.x>(1.0-1.0/resolution.x)||tc.x<1.0/resolution.x||tc.y>(1.0-1.0/resolution.x)||tc.y<1.0/resolution.y);
		
}
bool isOutDig(vec2 tc1,vec2 tc2)
{
    if(length(tc1-tc2)>MESHTRRESHOLD)
		return true;
	else
		return false;
}
bool isDepthOut(vec3 tc1,vec3 tc2)
{
	float depth1 = tc1.z;
	float depth2 = tc2.z;
	return abs(depth1-depth2)>0.0005;
}
bool isEdgeF(vec2 value)
{
	return value.x>0.5||value.y>0.5;
}
void main()
{	//for(int i = 0;i<gl_in.length();i++)
		//if(isDisCard[i] > 0.5)
			;//return;
	//if(abs(ModelID[0] - ModelID[1]) > 0.2 || abs(ModelID[0] - ModelID[2])> 0.2 || abs(ModelID[1] - ModelID[2])>0.2)
		//return;
	isInValid =0 ;
	for(int i = 0;i<3;i++)
		if(isDisCard[i] > 0)
		{
			isInValid = 1;
		}
			//return;
	if(isOutSide(TransVec[0].xy)||isOutSide(TransVec[1].xy)||isOutSide(TransVec[2].xy))
		return;
	if(isInBorder(Texcoord[0].xy)||isInBorder(Texcoord[1].xy)||isInBorder(Texcoord[2].xy))
		return;
	if((mode[0]==mode[1])&&(mode[1]==mode[2])&&(mode[2]==mode[0]))
	{
	}
	else 
		return;
		
	if(isEdgeValue[0]+isEdgeValue[1]+isEdgeValue[2]>=1.9)
	{
		isInValid = 1;
		return;
	}
	vec2 scale = (resolution/2048);
	if(abs(TransVec[0].y - TransVec[1].y)>MESHTRRESHOLD/2048 ||abs(TransVec[0].y - TransVec[2].y)>MESHTRRESHOLD/2048||abs(TransVec[2].y - TransVec[1].y)>MESHTRRESHOLD/2048)
	{
		isInValid = 1;
		return;
	}
	if(abs(TransVec[0].x - TransVec[1].x)>MESHTRRESHOLD/2048||abs(TransVec[0].x - TransVec[2].x)>MESHTRRESHOLD/2048||abs(TransVec[2].x - TransVec[1].x)>MESHTRRESHOLD/2048)
	{
		isInValid = 1;
		return;
	}
	
	
	/*
	vec2 scale = resolution;
	area = triangleArea(TransVec[0].xy*scale,TransVec[1].xy*scale,TransVec[2].xy*scale);
	if(area>TRIANGLETHRESHOLD)
	{
		isInValid = 1;
		return;
	}
	*/
	/*if(isOutDig(TransVec[0].xy,TransVec[1].xy)||isOutDig(TransVec[0].xy,TransVec[2].xy)||isOutDig(TransVec[2].xy,TransVec[1].xy))
	{
		isInValid = 1;
	}
	if(isDepthOut(TransVec[0],TransVec[1])||isDepthOut(TransVec[0],TransVec[2])||isDepthOut(TransVec[2],TransVec[1]))
	{
		isInValid = 1;
	}
	*/

	
	/*
	pos1 = TransVec[0].xy;
	pos2 = TransVec[1].xy;
	pos3 = TransVec[2].xy;
	
	float Id0 = texture2D(IdMap,Texcoord[0]).x;
	float Id1 = texture2D(IdMap,Texcoord[1]).x;
	float Id2 = texture2D(IdMap,Texcoord[2]).x;

	if(int(Id0)!=18)
		return;
		
	if(int(Id0)==11)
		return;
		
	
	if(abs(Id0-Id1)+abs(Id1-Id2)>0.2)
		return;

	pos1 = Texcoord[0];
	pos2 = Texcoord[1];
	pos3 = Texcoord[2];
     */

    for(int i = 0;i<gl_in.length();i++)
	{
		gl_Position = gl_in[i].gl_Position;
		Texcoordout = Texcoord[i];
		EmitVertex();
	}
	
	EndPrimitive();

	/*if(DisNum <0.5){
		for(int i = 0; i<gl_in.length();i++){
			gl_Position = gl_in[i].gl_Position;
		
			Texcoordout = Texcoord[i];
			EmitVertex();
		}
		EndPrimitive();
	
	
	}*/


}