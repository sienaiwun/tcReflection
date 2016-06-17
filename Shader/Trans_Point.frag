#version 430
uniform sampler2D RefelctTex;

//layout (location = 0) in vec2 Texcoord;
in float ReDisOut;
in vec2 Texcoord;
//in vec2 Texcoordout;
in float ReDis;

layout (location =  0) out vec4 color0;

void main(){

	if(ReDisOut >0)
		;//discard;
	color0 = texture2D(RefelctTex,Texcoord);
	//color0.w = ReDis;
	//color0 = vec4(1,0,0,0);
	//color0 = vec4(isDisCard,1,0,1);


}