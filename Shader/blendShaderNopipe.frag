#version 430

uniform sampler2D originColorTex;
uniform sampler2D additionColorTex;
//layout (location = 0) in vec2 Texcoord;
in vec2 Texcoordout;

uniform vec3 viewDependColor;
uniform vec3 viewIndependColor;



layout (location =  0) out vec4 color0;
bool colorEqual(vec3 soourceColor, vec3 targetColor)
{
	return length(soourceColor-targetColor)<0.01;
}

void main()
{
	vec4 outputColor;;
	vec4 originColor = texture2D(originColorTex,Texcoordout);
	vec4 additionColor =  texture2D(additionColorTex,Texcoordout);
	if(colorEqual(originColor.xyz,vec3(1,0,0))||colorEqual(originColor.xyz,vec3(0,1,0))||originColor.x>1.01||originColor.y>1.01)
	{
		outputColor = additionColor;
	}
	else
	    outputColor = originColor;
	color0 = outputColor;	
}