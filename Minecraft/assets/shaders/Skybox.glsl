#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 v_Position;

void main()
{
	v_Position = a_Position;
	vec4 pos = u_Projection * u_View * vec4(a_Position, 1.0);
	gl_Position = pos.xyww;
}

#type fragment
#version 330 core

out vec4 color;

uniform samplerCube u_Sample;

in vec3 v_Position;

void main()
{
	color = texture(u_Sample, v_Position);
}
