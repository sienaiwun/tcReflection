#version 430

layout(location = 0) in vec2 TexCoord;

uniform sampler2D tex;
layout (location =  0) out vec4 color0;
void main()
{
	color0 = texture2D(tex,TexCoord);
	color0.w = 1;
	//color0.xy = TexCoord;
}