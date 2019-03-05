#version 400

in vec4 colorBorder;
in vec4 colorFill;
in vec2 uv;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 passColor;

float toleranceLight = 0.4;

uniform sampler2D textBorder;

void main()
{
	vec4 interColor;
	vec4 sampledColor = texture2D(textBorder, uv);

	interColor = mix(colorFill, colorBorder, sampledColor.r);
	
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