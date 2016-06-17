#version 430

layout (triangles) in ;
layout (triangle_strip, max_vertices = 3) out;
uniform float m_total_vertex;

in int vertexID[];
out vec3 vetexGeometry;
void main()
{	
	int triangleId = 0;

   vetexGeometry = vec3(m_total_vertex+vertexID[0],m_total_vertex+vertexID[1],m_total_vertex+vertexID[2]);

	for(int i = 0;i<gl_in.length();i++)
	{
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
	

}