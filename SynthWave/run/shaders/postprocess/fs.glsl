#version 400

in vec2 uv;
uniform sampler2D TexColor;
uniform sampler2D TexBlurred;
uniform float screen_width;
uniform float screen_height;

float xstep = 1.0 / screen_width;
float ystep = 1.0 / screen_height;

out vec4 color_out;

void main (void)
{
	color_out = texture2D(TexColor, uv) + texture2D(TexBlurred, uv);
}