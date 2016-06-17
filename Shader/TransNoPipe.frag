#version 430
uniform sampler2D RefelctTex;
uniform vec3 ClearColor;
uniform sampler2D TransVecTex;
uniform sampler2D DiffuseTex;
uniform sampler2D IdMap;
in vec4 gl_FragCoord;
//layout (location = 0) in vec2 Texcoord;
in float ReDisOut;
in vec2 Texcoordout;
uniform  vec2 resolution;
in float isInValid;
/*
in vec2 pos1;
in vec2 pos2;
in vec2 pos3;
in float area;
*/
//in vec2 Texcoordout;
in float ReDis;
/*in vec2 pos1;
in vec2 pos2;
in vec2 pos3;*/
layout (location =  0) out vec4 color0;
vec3 randowColor(float i)
{
	 int aNumber = int(i)*346342;
	 int r = aNumber/256/256;
	 int g = (aNumber-r*256*256)/256;
	 int b = (aNumber-r*256*256-g*256);
	 return vec3(float(r),float(g),float(b))/255.0;
}
float triangleArea(vec2 pos1,vec2 pos2,vec2 pos3)
{
	vec2 AtoB = pos2 - pos1;
	vec2 AtoC=  pos3 - pos1;
	return abs(AtoB.y*AtoC.x-AtoB.x*AtoC.y)/2;
}
void main()
{
	vec2 tc = gl_FragCoord.xy/resolution;
	vec4 reflectedColor = texture2D(RefelctTex,Texcoordout);
	vec4 diffuseColor  = texture2D(DiffuseTex,tc.xy);
	float reflectValue = diffuseColor.w;
	vec3 color = reflectedColor.xyz*reflectValue+diffuseColor.xyz*(1-reflectValue);
	color0.xyz = color.xyz;
	//color0.x = area;
	/*color0.xy = pos2*1024;
	color0.zw = pos3*1024;*/
	if(isInValid>0.00001)
	    color0.xyz = ClearColor;	
}