#version 400

uniform mat4 mvp;
uniform mat4 mv;
uniform float elapsed;
uniform vec3 carPos;

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec4 color_border_in;
layout(location = 2) in vec4 color_fill_in;
layout(location = 3) in vec2 uv_in;

out	vec4 colorFill;
out	vec4 colorBorder;
out	vec2 uv;

void main()
{
	vec3 copyPos = position_in;
	vec3 camPos = (mv * vec4(position_in, 1)).xyz;

	vec3 carCam = (mv * vec4(carPos, 1)).xyz;
	copyPos.z -= pow(length(camPos.y) / 30, 3.5);

	gl_Position = mvp * vec4(copyPos, 1);
	colorBorder = color_border_in;
	colorFill = color_fill_in;
	uv = uv_in;
}