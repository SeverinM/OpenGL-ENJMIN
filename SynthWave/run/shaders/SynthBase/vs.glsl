#version 400

uniform mat4 mvp;
uniform float elapsed;

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec4 color_border_in;
layout(location = 2) in vec4 color_fill_in;

out VertexAttrib
{
	vec4 colorFill;
	vec4 colorBorder;
} vertex;

void main()
{
	gl_Position = mvp * vec4(position_in, 1);
	vertex.colorBorder = color_border_in;
	vertex.colorFill = color_fill_in;
}