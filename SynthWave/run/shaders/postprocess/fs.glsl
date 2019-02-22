#version 400

in vec2 uv;
uniform sampler2D TexColor;
uniform sampler2D TexBlurred;
uniform sampler2D Depth;

uniform float screen_width;
uniform float screen_height;

float xstep = 1.0 / screen_width;
float ystep = 1.0 / screen_height;

vec4 colorBottom = vec4(38.0 / 255, 0, 204.0 / 255, 1);
vec4 colorTop = vec4(0, 0, 0, 1);

uniform vec2 near_far;

out vec4 color_out;

float LinearizeDepth(float z)
{
	float n = near_far.x; // camera z near
	float f = near_far.y; // camera z far
	return (2.0 * n) / (f + n - z * (f - n));
}

void main (void)
{
	color_out = texture2D(TexColor, uv) + texture2D(TexBlurred, uv);
	float depth = texture2D(Depth, uv).r;
	if (depth == 0)
	{
		color_out = mix(colorBottom, colorTop, uv.y);
	}
}