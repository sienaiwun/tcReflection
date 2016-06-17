#version 430

layout(location = 0) in vec2 TexCoord;

uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D posTex;
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
//f = 1/z;
float neiboghoodPixel(vec2 tex, vec2 step,vec4 sourceValue)
{
    float f0 = getDepthRep(sourceValue.xyz);
	bool minusValid = true,plusValid = true;
	vec4 temp;
	float plusValue,minusValue;
	temp =  texture2D(posTex,tex+step);
	if(temp.w == sourceValue.w)
	{
		plusValue = getDepthRep(temp.xyz);
		return  plusValue;
	}
	temp =  texture2D(posTex,tex-step);
	if(temp.w == sourceValue.w)
	{
		minusValue = getDepthRep(temp.xyz);
		return f0 + f0 - minusValue;
	}
	else
	{
		plusValid = false;
	}
	return 0;
	

}
float getIntersection(vec3 pos1, vec3 normal1, vec3 pos2,vec3 normal2)
{
	vec3 v = normalize(cross(normal1,normal2));
	vec3 palaneNormal = normalize(cross(v,normal2));
	vec3 D = pos2 - pos1;
	//vec3 hitPoint = pos1 + dot( pos2 - pos1,palaneNormal)/dot(normal1,palaneNormal) * normal1;
	if(abs(dot(normal1,palaneNormal)) <0.00000001)
		return 0;
	float t = dot(pos2-pos1,palaneNormal)/dot(normal1,palaneNormal);
	return t;
}
vec3 getIntersectPosition(vec3 position, vec3 normal, vec2 tc)
{
	vec2 ndc = tc *2.0 - 1.0;
	vec4 temp = vec4(ndc,0,1);
	vec4 imgPoint = mvpInv *temp;
	vec3 pointOnPlane = imgPoint.xyz/imgPoint.w;
	vec3 D = pointOnPlane - cameraPos;
	vec3 hitPoint = cameraPos + dot( position - cameraPos,normal)/dot(D,normal) * D;
	return hitPoint;
}
vec3 showColor(vec3 input)
{
	vec3 value = vec3(0,0,0);
	if(input.x>0)
	{
	value.x = 1.0;
	}
	if(input.y>0)
	{
	value.y = 1.0;
	}
	if(input.z>0)
	{
	value.z = 1.0;
	}
	return value;
}
float neiboghoodConcave(vec2 tex, vec2 step,vec4 sourcePosition,vec4 sourceNormal )
{	
	vec4 neighborPos = texture2D(posTex,tex+step);
	vec4 neighborNormal;
	if(neighborPos.w == sourcePosition.w)  // using right value
	{
		neighborNormal = texture2D(normalTex,tex+step);
		float concave = getIntersection(neighborPos.xyz,neighborNormal.xyz,sourcePosition.xyz,sourceNormal.xyz);
		return sign(concave)*0.5+0.5;
	}
	neighborPos = texture2D(posTex,tex-step);
	if(neighborPos.w == sourcePosition.w)
	{
		neighborNormal = texture2D(normalTex,tex-step);
		float concave = getIntersection(neighborPos.xyz,neighborNormal.xyz,sourcePosition.xyz,sourceNormal.xyz);
		return sign(concave)*0.5+0.5;
	}
	return 0;

	
}
void main()
{
		vec4 posValue = texture2D(posTex,TexCoord);	          //last as id
		color0 = posValue;
		vec4 normalValue = texture2D(normalTex,TexCoord);	  //last as reflective
		color1 = normalValue;
		vec4 value2 = texture2D(colorTex,TexCoord);	
		float ddx,ddy,dxdy;
		color2 = value2;
		vec2 step = 1.0/resolution;
		{
			
			float rightCurvature = neiboghoodConcave(TexCoord,vec2(step.x,0),posValue,normalValue);
			ddx = rightCurvature;
		}
		{
			float upCurvature = neiboghoodConcave(TexCoord,vec2(step.x,0),posValue,normalValue);
			ddy = upCurvature;
			
		}
		{
			float uprightCurvature = neiboghoodConcave(TexCoord,vec2(step.x,step.y),posValue,normalValue);
		
			dxdy = uprightCurvature;
		}
		float f0 = 0;
		vec3 xyz = vec3(ddx,ddy,dxdy);
		/*xyz = showColor(xyz);
		if(length(xyz)==0)
		{
			color3 = vec4(0,0,0,f0);
			return ;
		}*/
		color3 = vec4(xyz,f0);
}
void mainnonsence()
{
		vec4 posValue = texture2D(posTex,TexCoord);	          //last as id
		color0 = posValue;
		vec4 normalValue = texture2D(normalTex,TexCoord);	  //last as reflective
		color1 = normalValue;
		vec4 value2 = texture2D(colorTex,TexCoord);	
		float ddx,ddy,dxdy;
		color2 = value2;
		vec2 step = 1.0/resolution;
		{
			
			vec3 rightPosPredict = getIntersectPosition(posValue.xyz,normalValue.xyz,TexCoord+vec2(step.x,0));
			float rightProdict = getDepthRep(rightPosPredict);
			float rightValue = neiboghoodPixel(TexCoord,vec2(step.x,0),posValue);
			ddx = 2*(rightValue-rightProdict);
		}
		{
			vec3 upPosPredict = getIntersectPosition(posValue.xyz,normalValue.xyz,TexCoord+vec2(0,step.y));
			float upPredictValue = getDepthRep(upPosPredict);
			float upValue = neiboghoodPixel(TexCoord,vec2(0,step.y),posValue);
			ddy = 2*(upValue-upPredictValue);

			/*vec4 sourceValue = posValue;
			vec2 tex = TexCoord;
			vec2 step = vec2(0,step.y);
			float f0 = getDepthRep(sourceValue.xyz);
			bool minusValid = true,plusValid = true;
			vec4 temp;
			float plusValue,minusValue;
			temp =  texture2D(posTex,tex+step);
			if(temp.w == sourceValue.w)
			{
				plusValue = getDepthRep(temp.xyz);
			}
			else
			{
				plusValid = false;
			}
			temp =  texture2D(posTex,tex-step);
			if(temp.w == sourceValue.w)
			{
				minusValue = getDepthRep(temp.xyz);
			}
			else
			{
				plusValid = false;
			}
			temp =  texture2D(posTex,tex-step);
			if(minusValid&&plusValid)
			{
					color3  = vec4(f0,plusValue,minusValue,f0 - (plusValue + minusValue)/2.0);
					return;
			}*/
			
		}
		{
			vec3 upRightPosPredict = getIntersectPosition(posValue.xyz,normalValue.xyz,TexCoord+vec2(step.x,step.y));
			float upRightPredictValue = getDepthRep(upRightPosPredict);
			float upRightValue = neiboghoodPixel(TexCoord,vec2(step.x,step.y),posValue);
			/*vec3 test = getWorldPos(TexCoord+vec2(step.x,step.y),upRightValue);
			color3.xyz  = upRightPosPredict;
			return;*/
			dxdy = upRightValue-upRightPredictValue-ddx-ddy;
		}
		float f0 = getDepthRep(posValue.xyz);
		vec3 xyz = vec3(ddx,ddy,dxdy);
		/*xyz = showColor(xyz);
		if(length(xyz)==0)
		{
			color3 = vec4(0,0,0,f0);
			return ;
		}*/
		color3 = vec4(xyz,f0);
		
		
}