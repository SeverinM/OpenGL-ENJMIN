#version 400

in vec4 worldPos_vs;
in vec4 normal_vs;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 passColor;

vec4 ambient = vec4(0, 0, 1, 1);

uniform vec3 sunPos;

void main()
{
	vec4 interColor;
	interColor = vec4(normalize(sunPos - worldPos_vs.xyz),1);

	vec3 toSun = normalize(sunPos - worldPos_vs.xyz);
	interColor = vec4(vec3(1, 1, 1) * max(0,dot(toSun, normal_vs.xyz)),1);

	interColor = normalize(interColor + ambient);
	interColor.a = 1;

	color = interColor;
	passColor = color;
}