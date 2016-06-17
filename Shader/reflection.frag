#version 430
in vec3 worldPos;
in vec3 worldNormal;
in vec2 tc;
in vec4 gl_FragCoord ;


layout( location = 0 ) out vec4 FragColor0;

uniform sampler2D objectTex;
uniform vec3 objectDiffuseColor;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform int hasTex;
uniform int objectId;
uniform vec2 windowsSize;

uniform sampler2D reflectMap;
uniform float reflectBlender;


void main()
{
	vec3 N = normalize(worldNormal);
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
	vec3 nodiffColor = diffuse*(kd+ka);
	if(reflectBlender<0.05)
		FragColor0.xyz =nodiffColor ;
	else
	{
		float alpha = reflectBlender;
		alpha = 1.0;
	    vec2 uv = gl_FragCoord.xy/windowsSize.xy;
		vec3 reflectValue =  texture2D(reflectMap,uv).xyz;
		FragColor0.xyz = nodiffColor*(1-alpha)+reflectValue*alpha;
		//FragColor0.xyz = reflectValue;
	}
	FragColor0.xyz =nodiffColor;
}
