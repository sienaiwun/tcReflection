#version 430

layout(location = 0) in vec2 TexCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D diffuseTex;


uniform float ratio1;
uniform float ratio2;

uniform int uesAdditionalTex;
layout (location =  0) out vec4 color0;


bool colorEqual(vec3 soourceColor, vec3 targetColor)
{
	return length(soourceColor-targetColor)<0.1;
}
bool isInvalid(vec3 reflectColor)
{
	return colorEqual(reflectColor.xyz,vec3(1,0,0))||colorEqual(reflectColor.xyz,vec3(0,1,0))||reflectColor.x>1.01||reflectColor.y>1.01;
}
void main()
{
	vec4 diffuseColor = texture2D(diffuseTex,TexCoord);
	float reflectBlender = diffuseColor.w;
	vec3 reflectColor;
	reflectBlender = 1;
	if(reflectBlender>0)
	{
		 vec4 reflectColor1 = texture2D(tex1,TexCoord);
		 vec4 reflectColor2 = texture2D(tex2,TexCoord);
		 bool isInValid1 = isInvalid(reflectColor1.xyz);
		 bool isInValid2 = isInvalid(reflectColor2.xyz);
		 if(!isInValid1&&!isInValid2)
		 {
			reflectColor = reflectColor1.xyz*ratio1+reflectColor2.xyz*ratio2;
		 }
		 else if(isInValid1&&!isInValid2)
		 {
		   reflectColor = reflectColor2.xyz;
		 }
		 else if(!isInValid1&&isInValid2)
		 {
		   reflectColor = reflectColor1.xyz;
		 }
		 else
		  reflectColor = reflectColor2.xyz;
	     color0.xyz = diffuseColor.xyz*(1-reflectBlender)+reflectColor.xyz*(reflectBlender);
	}
	else
		color0.xyz = diffuseColor.xyz;
	color0.w = 1;
}