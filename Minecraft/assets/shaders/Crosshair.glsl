#type vertex
#version 330 core

layout (location = 0) in vec2 a_Position;

uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * vec4(a_Position, 0.0, 1.0);
}

#type fragment
#version 330 core

layout (location = 0) out vec4 color;

void main()
{
	color = vec4(0.0, 1.0, 1.0, 1.0);
}
