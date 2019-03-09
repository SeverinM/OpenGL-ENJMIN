#version 400

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 nmat;

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec3 normal_in;

out vec4 worldPos_vs;
out vec4 normal_vs;

void main()
{
	gl_Position = mvp * vec4(position_in, 1);
	worldPos_vs = m * vec4(position_in,1);
	normal_vs = vec4(normal_in, 1);
}