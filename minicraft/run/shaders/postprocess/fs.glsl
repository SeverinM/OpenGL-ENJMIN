#version 400

in vec2 uv;

uniform sampler2D TexColor;
uniform sampler2D TexBlurred;
uniform float screen_width;
uniform float screen_height;
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
	float xstep = 1.0/screen_width;
	float ystep = 1.0/screen_height;
	float ratio = screen_width / screen_height;

	vec4 color = texture2D(TexBlurred, uv) + texture2D(TexColor, uv);
	color_out = color;

	if (length(vec2(0.5,0.5) - uv) < 0.002)
		color_out = vec4(1,1,1,1);

    //Gamma correction
    //color.r = pow(color.r,1.0/2.2);
    //color.g = pow(color.g,1.0/2.2);
    //color.b = pow(color.b,1.0/2.2);
}