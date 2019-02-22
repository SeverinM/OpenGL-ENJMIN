#version 400

in vec4 color_border;
in vec4 color_fill;
in vec3 bary;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 passColor;
layout(location = 2) out vec4 depth;

float toleranceLight = 0.7;

void main()
{
	vec4 interColor;
	if ((bary.x) < 0.01 || (bary.y) < 0.01 || ((bary.z) < 0.01 && color_border.r == 0))
	{
		interColor = color_border;
	}
	else
	{
		interColor = color_fill;
	}
	
	if (max(interColor.r,max(interColor.g, interColor.b)) > toleranceLight)
	{
		passColor = interColor;
	}
	else
	{
		passColor = vec4(0,0,0,1);
	}
	color = interColor;
	depth = vec4(gl_FragCoord.zzz, 1.0);
}