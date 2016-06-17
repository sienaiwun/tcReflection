#version 430

layout(location = 0) in vec2 TexCoord;

uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D posTex;
uniform sampler2D optixTex;
uniform mat4 MVP; // Projection * ModelView
uniform mat4 mvpInv;
uniform mat4 modeView;
uniform mat4 modeViewInv;
layout (location =  0) out vec4 color0;  //pos
layout (location =  1) out vec4 color1;  //normal
layout (location =  2) out vec4 color2;  //color.w change
layout (location =  3) out vec4 color3;  //normal
layout (location =  4) out vec4 color4;  //color.w change

uniform vec2 resolution;
uniform vec3 cameraPos;


uniform vec2 imageMin;
uniform vec2 imageMax;
vec3 getMirroPos(vec2 tc)
{
    vec3 worldPos = texture2D(posTex,tc).xyz;
	vec3 normal =  texture2D(normalTex,tc).xyz;
	float reflectDis =  texture2D(optixTex,tc).w;
	
	vec3 inComeDirection = worldPos - cameraPos;
	vec3 LookVec = normalize(inComeDirection);
		//计算反射光线方向
	vec3 ReflectVec = normalize(reflect(LookVec,normal));
	vec3 reflectPos  = worldPos + ReflectVec * reflectDis;	
	vec3 mirrorPos = abs(dot(worldPos - reflectPos,normal)) * (-2) * normal + reflectPos;
	return mirrorPos;
}
vec3 getWorldPos(vec2 tc, float depthR)
{
	float z = 1.0/depthR;
	vec2 xy = imageMin+ (imageMax-imageMin)*tc;
	xy = xy*(-z);
	vec4 temp = modeViewInv* vec4(xy,z,1);
	return temp.xyz/temp.w;
}
float getDepthRep(vec3 worldPos)
{
	vec4 tmp = modeView*vec4(worldPos,1);
	return 1.0/tmp.z;
}
float getDepthTx(vec2 tc)
{
   return getDepthRep(texture2D(posTex,tc).xyz);
}
float getReflectD(vec2 tc)
{
 return getDepthRep(getMirroPos(tc));
}
#define FISTDIS 0.0005
float fistDepth(float c,float  l,float r)
{
	if (abs(l+r-2*c)<FISTDIS)
		return 0;
	else
		return 1;
}
vec2 firstDisconect(vec2 tc)
{
vec2 step = 1.0/resolution;
    vec2 result;
	float c = getDepthTx(tc);
	float l = getDepthTx(tc-vec2(step.x,0));
	float r = getDepthTx(tc+vec2(step.x,0));
	float t = getDepthTx(tc+vec2(0,step.y));
	float b = getDepthTx(tc-vec2(0,step.y));
	result.x = fistDepth(c,l,r);
	result.y = fistDepth(c,t,b);
	return result;
}

vec2 secondDisconect(vec2 tc)
{
vec2 step = 1.0/resolution;
    vec2 result;
	float c = getReflectD(tc);
	float l = getReflectD(tc-vec2(step.x,0));
	float r = getReflectD(tc+vec2(step.x,0));
	float t = getReflectD(tc+vec2(0,step.y));
	float b = getReflectD(tc-vec2(0,step.y));
	result.x = fistDepth(c,l,r);
	result.y = fistDepth(c,t,b);
	return result;
}
void main()
{
		vec4 posValue = texture2D(posTex,TexCoord);	          //last as id
		color0 = posValue;
		vec4 normalValue = texture2D(normalTex,TexCoord);	  //last as reflective
		color1 = normalValue;
		vec4 value2 = texture2D(colorTex,TexCoord);	
		color2 = value2;
		vec2 first = firstDisconect(TexCoord);
		vec2 second = secondDisconect(TexCoord);
		color3.xy = first;
		color3.zw = second;
		if(any(second.x>0||second.y>0))
			color2.w = 1;
		else 
			color2.w  = 0;
		
}