#version 430

layout(location = 0) in vec2 TexCoord;

uniform sampler2D reflectTex;
uniform int isEqual;
uniform vec3 compareColor;
layout (location =  0) out vec4 color0;


bool colorEqual(vec3 soourceColor, vec3 targetColor)
{
	return length(soourceColor-targetColor)<0.01;
}
void main()
{

		vec4 reflectColor = texture2D(reflectTex,TexCoord);
		if(bool(isEqual))
		{
			if(colorEqual(reflectColor.xyz,compareColor))  // fetch the color in additional tex
			{
				color0 = vec4(1,0,0,1);
				return;
			}
			discard;
		}  
		else
		{
		    if(colorEqual(reflectColor.xyz,compareColor))  // fetch the color in additional tex
			{
				discard;
				
			}
			color0 = vec4(1,0,0,1);
			return;
		}
	
}