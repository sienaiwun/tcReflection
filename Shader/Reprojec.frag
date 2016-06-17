#version 430

layout (location = 0) in vec2 TexCoord;

uniform sampler2D Reflection;
uniform sampler2D NewWorldPos;
uniform sampler2D LastWorldPos;
uniform sampler2D LastNormlTex;
uniform vec3 Clearcolor;

uniform mat4 LastMVP;

layout (location = 0) out vec4 color0;

void main()
{
	vec4 newWorldPos = texture2D(NewWorldPos,TexCoord);
	newWorldPos.w = 1.0;
	
	
	vec4 NewPro = LastMVP * newWorldPos;
	
	vec2 NewUV = NewPro.xy/NewPro.w;
	
	if(abs(NewUV.x )> 1 || abs(NewUV.y) >1)
	{
		color0 = vec4(Clearcolor,1);
		return;
	}
	
	
	NewUV = (NewUV * 0.5 ) + 0.5;
	
	//if(NewUV == TexCoord)
	//	color0 = vec4(0,1,0,1);
	//else 
		//color0 = vec4(0,0,1,0);
	vec4 lastWorldPos = texture2D(LastWorldPos,NewUV);
	if(length(lastWorldPos.xyz - newWorldPos.xyz) > 5.0f)
	{
		color0 = vec4(Clearcolor,1);
		return;
	}
	float reflectValue = texture2D(LastNormlTex,NewUV).w;
	if(reflectValue<0.01)
	{
		color0 = vec4(0,0,1,1);
		return;
	}
	color0 = texture2D(Reflection,NewUV);
	


}