#version 430

layout(location = 0) in vec2 TexCoord;

uniform sampler2D OptixTex;
uniform sampler2D ComputeTex;
uniform int FrameCount;

layout (location =  0) out vec4 color0;
void main(){

	vec4 ComputeTexTmp = texture2D(ComputeTex,TexCoord);

	//color0 = texture2D(OptixTex,TexCoord);
	//return;
	if (FrameCount % 10 == 0){
		color0 = texture2D(OptixTex,TexCoord);
		return;
	}
	
	if(ComputeTexTmp.x>=-0.5)
		//color0 = vec4(1,1,0,0);
		color0 = ComputeTexTmp;
	else
		color0 = texture2D(OptixTex,TexCoord);
		



}