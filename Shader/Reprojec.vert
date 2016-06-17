#version 420

layout (location = 0) in vec2  position;

layout (location = 0) out vec2 Texcoord;
void main()
{
	gl_Position = vec4(position.x,position.y,0,1);
	Texcoord = (position * 0.5) + 0.5;
}