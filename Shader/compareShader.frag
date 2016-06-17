#version 430

layout(location = 0) in vec2 TexCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;
layout (location =  0) out vec4 color0;


bool colorEqual(vec3 soourceColor, vec3 targetColor)
{
	return length(soourceColor-targetColor)<0.1;
}
vec3 diffColor(vec3 color1, vec3 color2)
{
	return 1*abs(color1-color2);
}
void main()
{

	vec4 value1 = texture2D(tex1,TexCoord);
	vec4 value2 = texture2D(tex2,TexCoord); 

	/*if( colorEqual(value1.xyz,value2.xyz))
	{
		color0 = vec4(1,0,0,1);
	}
	else
	{
		color0 = vec4(0,1,0,1);
	}*/
	color0.xyz = diffColor(value1.xyz,value2.xyz);
	color0.w =1;
	
}