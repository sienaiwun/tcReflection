#version 430

in vec2 Texcoord;
in float isDisCard;
in vec2 TransVec;

layout (location =  0) out vec4 color0;

uniform sampler2D TransVecTex;
void main()
{
	//color0 = texture2D(RefelctTex,Texcoordout);
	vec2 tc = vec2(1023.5/1024,1023.5/1024);
	color0.xy = texture2D(TransVecTex,tc).xy;;
}