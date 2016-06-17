#version 420

layout (location = 0) in vec2  position;


layout (location = 0 ) out vec2 Texcoord;

void main()
{

	gl_Position = vec4(position,0,1.0);
	
	Texcoord = (position /2.0) + 0.5;

}