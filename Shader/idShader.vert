#version 430
uniform mat4 MVP; // Projection * ModelView
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 VertexTc;
 in int gl_VertexID;
 out int vertexID;
void main()
{
	gl_Position = MVP * vec4(VertexPosition,1.0);
	vertexID =  gl_VertexID;
}