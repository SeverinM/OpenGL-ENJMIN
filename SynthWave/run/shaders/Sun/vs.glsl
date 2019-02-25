#version 400

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;
uniform mat4 mvp;
uniform mat4 im;

layout(location = 0) in vec3 position_in;

out vec3 TexCoords;

void main()
{
	TexCoords = position_in;

	vec4 pos = mvp * vec4(position_in, 1);
	gl_Position = pos.xyww;
}