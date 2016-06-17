#version 430

uniform vec3 ClearColor;
uniform sampler2D worldTex;
uniform mat4 MVP; // Projection * ModelView
uniform sampler2D colorTex;
uniform sampler2D normalTex;
//layout (location = 0) in vec2 Texcoord;
in vec2 Texcoordout;

//in vec2 Texcoordout;

layout (location =  0) out vec4 color0;
layout (location =  1) out vec4 color1;
layout (location =  2) out vec4 color2;
void main()
{
	color2 = texture2D(colorTex,Texcoordout);
	color1 = texture2D(normalTex,Texcoordout);
	color0 = texture2D(worldTex,Texcoordout);
}