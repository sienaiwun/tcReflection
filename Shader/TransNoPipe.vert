#version 430

layout (location = 0) in vec2  position;
out vec2 Texcoord;
out float isDisCard;
out vec2 TransVec;
out int mode;
uniform sampler2D TransVecTex;

uniform sampler2D WorldPosTex;

uniform sampler2D RePosTex;

uniform vec2 resolution;


void main(){

	vec4 TmpTex = texture2D(TransVecTex, position);
	//TmpTex.xy = position;
	TransVec = TmpTex.xy;
	//TmpTex.w = -0.1;
	isDisCard = 0.0f;
	if(TmpTex.x < 0)
		isDisCard = 10.0f;
	else
		isDisCard = 0.0f;
	
	//vec2 tmpPos = (position + TmpTex.xy - 0.5) * 2.0;
	vec2 tmpPos   = (TmpTex.xy - 0.5) * 2.0;
	gl_Position = vec4(tmpPos.x,tmpPos.y,TmpTex.z,1.0);
	//gl_Position.xy = (position.xy- 0.5) * 2.0;
	
	Texcoord = position;
	mode =  int(TmpTex.w);
	
}