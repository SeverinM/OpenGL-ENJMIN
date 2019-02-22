#version 400

in vec2 uv;
uniform sampler2D TexColor;
out vec4 color_out;

void main (void)
{
	color_out = texture2D(TexColor, uv);
}