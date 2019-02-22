#version 400

layout (location = 0) out vec4 BlurredText;
in vec2 uv;
uniform sampler2D image;
uniform float screen_width;
uniform float screen_height;


void main()
{
	float xstep = 1.0 / screen_width;
	float ystep = 1.0 / screen_height;
	vec4 sum = texture2D(image, uv);
	vec2 modifiedUV;
	int count = 1;

	for (int x = -7; x < 7; x++)
	{
		for (int y = -7; y < 7; y++)
		{
			if (x != 0 || y != 0)
			{
				modifiedUV = vec2(uv.x + (xstep * x), uv.y + (ystep * y));
				if (modifiedUV.x >= 0 && modifiedUV.x <= 1 && modifiedUV.y >= 0 && modifiedUV.y <= 1)
				{
					sum += texture2D(image, modifiedUV);
					count++;
				}				
			}
		}
	}
	BlurredText = sum / count;
}