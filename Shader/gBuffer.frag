#version 430
in vec3 worldPos;
in vec3 worldNormal;
in vec2 tc;
in int out_vertexId;

layout( location = 0 ) out vec4 FragColor0;
layout( location = 1 ) out vec4 FragColor1;
layout( location = 2 ) out vec4 FragColor2;
layout( location = 3 ) out vec4 FragColor3;

uniform sampler2D objectTex;
uniform vec3 objectDiffuseColor;
uniform mat4 modelView; // Projection * ModelView

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform int hasTex;
uniform int objectId;
uniform float reflectFactor;
vec2 getNormalizedUV(vec3 uv_in)
{
  vec2 uv_out = uv_in.xy*.5 + .5;
  return uv_out;
}

vec3 getIdColor(int id)
{
	int totalId = id*99999999;
	int b = totalId%255;
	int g = (totalId - b*255)/255%255;
	int r = (totalId - g*255*255 - b*255)/(255*255)%255;
	return vec3(r,g,b)/255.0;
}
float getDepth(vec3 pos)
{
	vec4 dcPos = modelView * vec4(pos,1.0); 
	return abs(dcPos.z/100.0);
}

void main()
{
	float reflectValue = reflectFactor;
	FragColor0 = vec4(worldPos,objectId);
	vec3 N = normalize(worldNormal);
	FragColor1 = vec4(N,reflectValue);
	vec3 L = normalize(lightPos-worldPos);
	float kd=max(dot(N,L),0);
	vec3 diffuse;
	if(bool(hasTex))
	{
		diffuse = texture2D(objectTex,tc).xyz;
	}
	else
	{
		diffuse = objectDiffuseColor;
	}
	float ka = 0.2;
	FragColor2.xyz = diffuse*(kd+ka);
	/*FragColor2.xyz = (kd).xxx;
	if(reflectValue>0)
		FragColor2.xyz = vec3(1,0,0);
	else
		FragColor2.xyz = vec3(1,1,0);
	
	*/float shading = (kd+ka);
	//FragColor2.xyz = L;
	FragColor2.w = reflectValue;
	//FragColor2.xyz =getDepth(worldPos.xyz).xxx;
	//FragColor3.xyz = getIdColor(out_vertexId);
}
