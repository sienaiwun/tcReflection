#version 430

layout (location = 0) in vec2  position;
layout (location = 1) in vec2 texC;
//layout (location = 0) out vec2 OutPosition;
out vec2 Texcoord;
//out float isDisCard;
out float ReDis;
out vec2 TransVec;

uniform sampler2D TransVecTex;

uniform sampler2D WorldPosTex;

uniform sampler2D RePosTex;

float SHADEREDGETHRESHOLD = 0.0003;

//边界检验
bool IsEdge(vec2 TmpTexCoord){

	vec2 NowInfo = texture2D(TransVecTex,TmpTexCoord).zw;
	vec2 info1,info2;
	//上下
	vec3 tmpZ,tmpW;
	
	info1 = texture2D(TransVecTex,TmpTexCoord + vec2(0,1.0/1024.f)).zw;
	info2 = texture2D(TransVecTex,TmpTexCoord - vec2(0,1.0/1024.f)).zw;
	
	tmpZ = vec3(1.0/info1.x,1.0/NowInfo.x,1.0/info2.x);
	if(abs(2*tmpW.y - tmpW.x - tmpW.z) > SHADEREDGETHRESHOLD)
		;//return true;
		
	tmpW = vec3(1.0/info1.y,1.0/NowInfo.y,1.0/info2.y);
	if(abs(2*tmpZ.y - tmpZ.x - tmpZ.z) > SHADEREDGETHRESHOLD)
		return true;
		
	//左右
	info1 = texture2D(TransVecTex,TmpTexCoord + vec2(1.0/1024.0f,0)).zw;
	info2 = texture2D(TransVecTex,TmpTexCoord - vec2(1.0/1024.0f,0)).zw;
	tmpZ = vec3(1.0/info1.x,1.0/NowInfo.x,1.0/info2.x);
	if(abs(2*tmpZ.y - tmpZ.x - tmpZ.z) >SHADEREDGETHRESHOLD)
		;//return true;
		
	tmpW = vec3(1.0/info1.y,1.0/NowInfo.y,1.0/info2.y);
	if(abs(2*tmpW.y - tmpW.x - tmpW.z) > SHADEREDGETHRESHOLD)
		return true;
		
	//斜上下
	info1 = texture2D(TransVecTex,TmpTexCoord + vec2(1.0/1024.0f,1.0/1024.0f)).zw;
	info2 = texture2D(TransVecTex,TmpTexCoord - vec2(1.0/1024.0f,1.0/1024.0f)).zw;
	tmpZ = vec3(1.0/info1.x,1.0/NowInfo.x,1.0/info2.x);
	if(abs(2*tmpW.y - tmpW.x - tmpW.z) > SHADEREDGETHRESHOLD)
		;//return true;
		
	tmpW = vec3(1.0/info1.y,1.0/NowInfo.y,1.0/info2.y);
	if(abs(2*tmpW.y - tmpW.x - tmpW.z) > SHADEREDGETHRESHOLD)
		return true;;
		
	info1 = texture2D(TransVecTex,TmpTexCoord + vec2(-1.0/1024.0f,1.0/1024.0f)).zw;
	info2 = texture2D(TransVecTex,TmpTexCoord + vec2(1.0/1024.0f,-1.0/1024.0f)).zw;
	tmpZ = vec3(1.0/info1.x,1.0/NowInfo.x,1.0/info2.x);
	if(abs(2*tmpZ.y - tmpZ.x - tmpZ.z) >SHADEREDGETHRESHOLD)
		;//return true;
		
	tmpW = vec3(1.0/info1.y,1.0/NowInfo.y,1.0/info2.y);
	if(abs(2*tmpW.y - tmpW.x - tmpW.z) > SHADEREDGETHRESHOLD)
		return true;
		
	return false;

}




void main(){

	vec4 TmpTex = texture2D(TransVecTex, position);
	//isDisCard = 0.0f;
	if(abs(TmpTex.x) >= 0.03 || abs(TmpTex.y)>= 0.03)// || TmpTex.z < -5)
	//if(TmpTex.z < -5)
		;//isDisCard = 1.0;
	//if(IsEdge(position))
		//isDisCard = 1.0;
	
	vec2 tmpPos = ( TmpTex.xy - 0.5) * 2.0;
	//vec2 tmpPos   = (position + TmpTex.xy - 0.5) * 2.0;
	//gl_Position = vec4(tmpPos.x,tmpPos.y,TmpTex.w,1.0);
	gl_Position = vec4(tmpPos.x,tmpPos.y,TmpTex.w,1.0);
	
	//传递深度信息
	//ModelID = (int) texture2D(ModelID,position);
	
	
	TransVec = TmpTex.xy;
	
	
	Texcoord = position;
	
}