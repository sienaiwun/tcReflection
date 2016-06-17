#version 430

layout(location = 0) in vec2 TexCoord;

uniform sampler2D tex1;

layout (location =  0) out vec4 color0;


bool colorEqual(vec3 soourceColor, vec3 targetColor)
{
	return length(soourceColor-targetColor)<0.1;
}
void main()
{
	vec4 value1 = texture2D(tex1,TexCoord);	
	value1.xy *= 1.5;
	value1.z = 0;
	color0 = value1;
}