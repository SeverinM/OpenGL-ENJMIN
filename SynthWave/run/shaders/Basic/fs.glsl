#version 400

in vec4 color_vs;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 passColor;

float toleranceLight = 0.4;

uniform sampler2D textBorder;

void main()
{
	vec4 interColor;

	interColor = color_vs;
	
	if (max(interColor.r,max(interColor.g, interColor.b)) > toleranceLight)
	{
		passColor = interColor;
	}
	else
	{
		passColor = vec4(0,0,0,1);
	}
	color = interColor;
}