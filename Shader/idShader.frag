#version 430
in vec3 vetexGeometry;

layout( location = 0 ) out vec4 FragColor0;
layout( location = 1 ) out vec4 FragColor1;
layout( location = 2 ) out vec4 FragColor2;

vec3 getIdColor(vec3 id)
{
	int totalId = int(25*id.x+125*id.y+12*id.z);
	int b = totalId%255;
	int g = (totalId - b*255)/255%255;
	int r = (totalId - g*255*255 - b*255)/(255*255)%255;
	return vec3(r,g,b)/255.0;
}

void main()
{
	FragColor0 = vec4(getIdColor(vetexGeometry),1);
}
