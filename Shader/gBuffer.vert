#version 430
uniform mat4 MVP; // Projection * ModelView
//in int gl_VertexID;
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 VertexTc;

out vec3 worldPos;
out vec3 worldNormal;
out vec2 tc;
//out int out_vertexId;
void main()
{
	worldPos = VertexPosition;
	worldNormal = VertexNormal;
	tc = vec2(VertexTc.x,VertexTc.y);
	gl_Position = MVP * vec4(VertexPosition,1.0);
//	out_vertexId = VertexTc;
}