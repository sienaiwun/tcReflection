#version 430

layout(location = 0) in vec2 TexCoord;

uniform sampler2D reflectTex;
uniform sampler2D diffuseTex;
uniform sampler2D additionalReflectTex;
uniform int uesAdditionalTex;
layout (location =  0) out vec4 color0;


bool colorEqual(vec3 soourceColor, vec3 targetColor)
{
	return length(soourceColor-targetColor)<0.01;
}

void main()
{
	vec4 diffuseColor = texture2D(diffuseTex,TexCoord);
	float reflectBlender = diffuseColor.w;
	//diffuseColor.xyz = vec3(0,0,0);
	if(reflectBlender>0)
	{
	     //reflectBlender = 1;
		 vec4 reflectColor = texture2D(reflectTex,TexCoord);
		 if(bool(uesAdditionalTex)&&(colorEqual(reflectColor.xyz,vec3(1,0,0))||colorEqual(reflectColor.xyz,vec3(0,1,0))||reflectColor.x>1.01||reflectColor.y>1.01))  // fetch the color in additional tex
		 {
				reflectColor = texture2D(additionalReflectTex,TexCoord);
				//color0 = vec4(0,0,0,1);
			//return;
		 }
	     color0.xyz = diffuseColor.xyz*(1-reflectBlender)+reflectColor.xyz*(reflectBlender);
	}
	else
	{

		color0.xyz = diffuseColor.xyz;
	}
	color0.w = 1;
}