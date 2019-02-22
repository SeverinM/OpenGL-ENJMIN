#version 400

in vec4 color_border;
in vec4 color_fill;
in vec3 bary;

out vec4 finalColor;

void main()
{
	if ((bary.x) < 0.01 || (bary.y) < 0.01)
	{
		finalColor = color_border;
	}
	else
	{
		finalColor = color_fill;
	}
}