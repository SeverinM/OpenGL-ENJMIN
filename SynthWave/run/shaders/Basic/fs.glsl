#version 400

in vec4 worldPos_vs;
layout (location = 0) out vec4 color;
layout (location = 1) out vec4 passColor;

float toleranceLight = 0.4;

void main()
{
	vec4 interColor;

	interColor = vec4(1, 1, 1, 1);

	color = interColor;
	passColor = color;
}