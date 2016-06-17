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

out vec2 Texcoordout;

float THRESHOLD = 0.5;

#define CONVERGE 1
#define OUTRANGE 2
#define OUTOBJECT 3
#define FASTPROJT 4


#define MESHTRRESHOLD (3.5)
void main()
{	//for(int i = 0;i<gl_in.length();i++)
		//if(isDisCard[i] > 0.5)
			;//return;
	//if(abs(ModelID[0] - ModelID[1]) > 0.2 || abs(ModelID[0] - ModelID[2])> 0.2 || abs(ModelID[1] - ModelID[2])>0.2)
		//return;
		
	for(int i = 0;i<3;i++)
		if(isDisCard[i] > 0)
			return;
	if((mode[0]==mode[1])&&(mode[1]==mode[2])&&(mode[2]==mode[0]))
	{
	}
	else 
		return;
	if(abs(TransVec[0].y - TransVec[1].y)>MESHTRRESHOLD/resolution.y ||abs(TransVec[0].y - TransVec[2].y)>MESHTRRESHOLD/resolution.y||abs(TransVec[2].y - TransVec[1].y)>MESHTRRESHOLD/resolution.y)
		return;
	if(abs(TransVec[0].x - TransVec[1].x)>MESHTRRESHOLD/resolution.x ||abs(TransVec[0].x - TransVec[2].x)>MESHTRRESHOLD/resolution.x ||abs(TransVec[2].x - TransVec[1].x)>MESHTRRESHOLD/resolution.x)
		return;
	/*
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