#version 400

in vec2 uv;
uniform sampler2D TexColor;

uniform float screen_width;
uniform float screen_height;

float xstep = 1.0 / screen_width;
float ystep = 1.0 / screen_height;

uniform vec2 velocity;
int numSample = 10;

out vec4 color_out;

void main (void)
{
	vec2 copyVelocity = velocity;
	copyVelocity.x *= xstep;
	copyVelocity.y *= ystep;

	vec4 color = texture2D(TexColor, uv);
	vec2 uvCopy = uv;
	for (int i = 1; i < numSample; i++, uvCopy += copyVelocity)
	{
		uvCopy.x = clamp(uvCopy.x, 0, 1);
		uvCopy.y = clamp(uvCopy.y, 0, 1);
		color += texture2D(TexColor, uvCopy);
	}

	color_out = color / numSample;
}