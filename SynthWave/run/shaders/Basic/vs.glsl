#version 400

uniform mat4 mvp;

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec3 normal_in;

out vec4 color_vs;

void main()
{
	gl_Position = mvp * vec4(position_in, 1);
	color_vs = vec4(normal_in, 1);
}