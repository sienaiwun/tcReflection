#version 430
uniform sampler2D RefelctTex;
uniform vec3 ClearColor;
uniform sampler2D TransVecTex;
uniform sampler2D IdMap;
//layout (location = 0) in vec2 Texcoord;
in float ReDisOut;
in vec2 Texcoordout;

//in vec2 Texcoordout;
in float ReDis;
in float isInValid;
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
void main()
{
	color0 = texture2D(RefelctTex,Texcoordout);
	//color0 = vec4(Texcoordout.x,Texcoordout.y,1,0);
	//color0.w = ReDis;
	//color0.xy = Texcoordout;
	//vec2 tc = vec2(1023.5/1024,1023.5/1024);
	//color0.xy = Texcoordout*1024;
	//color0.zw = pos1*1024;
	/*float indexId = texture2D(IdMap,Texcoordout).x;
	vec3 color = randowColor(indexId);
	color0.xyz = color;*/
	//color0.xyz = temp;
	if(isInValid>0.00001)
	    color0.xyz = ClearColor;	

	
}