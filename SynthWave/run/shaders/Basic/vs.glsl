#version 400

uniform mat4 mvp;
uniform mat4 m;

layout(location = 0) in vec3 position_in;

out vec4 worldPos_vs;

void main()
{
	gl_Position = mvp * vec4(position_in, 1);
	worldPos_vs = m * vec4(position_in,1);
}