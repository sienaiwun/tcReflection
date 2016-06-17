#version 430

layout (location = 0) in vec2  position;
out vec2 Texcoord;
out vec2 vecCoord;


uniform sampler2D worldTex;
uniform mat4 MVP; // Projection * ModelView
 
void main()
{
	vec4 TmpTex = texture2D(worldTex, position);
	vec3 worldPos = TmpTex.xyz;
	vec4 ndcValue =  MVP * vec4(worldPos,1.0);
	gl_Position = ndcValue;
	ndcValue/= ndcValue.w;
	vecCoord = ndcValue.xy;
	//vec2 tmpPos   = (ndcValue.xy - 0.5) * 2.0;
	Texcoord = position;
}