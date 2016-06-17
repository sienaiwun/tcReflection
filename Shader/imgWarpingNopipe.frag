#version 430

uniform vec3 ClearColor;
uniform sampler2D worldTex;
uniform mat4 MVP; // Projection * ModelView
uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D blendReflectTex;
//layout (location = 0) in vec2 Texcoord;
in vec2 Texcoordout;
in float isInValid;
//in vec2 Texcoordout;





layout (location =  0) out vec4 color0;
layout (location =  1) out vec4 color1;
layout (location =  2) out vec4 color2;
bool colorEqual(vec3 soourceColor, vec3 targetColor)
{
	return length(soourceColor-targetColor)<0.1;
}

void main()
{
	color1 = texture2D(normalTex,Texcoordout);
	color0 = texture2D(worldTex,Texcoordout);
	vec4 outputColor;;
	vec4 diffuseColor = texture2D(colorTex,Texcoordout);
	
	vec4 reflectColor =  texture2D(blendReflectTex,Texcoordout);
	if(diffuseColor.w>0.01)
	{
		outputColor = reflectColor;
	}
	else
	    outputColor = diffuseColor;
	color0 = outputColor;
	if(isInValid>0.00001)
	    color0.xyz = ClearColor;	

	
}