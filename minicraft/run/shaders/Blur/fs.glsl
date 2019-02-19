#version 400

out vec4 BlurredText;

in vec2 TexCoords;

uniform sampler2D image;

bool horizontal;
float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);


void main()
{
	//vec2 tex_offset = 1.0 / textureSize(image, 0); //Size texel
	//vec3 result = texture(image, TexCoords).rgb * weight[0];
	//if (horizontal)
	//{
	//	for (int i = 1; i < 5; i++)
	//	{
	//		result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
	//		result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
	//	}
	//}
	//else
	//{
	//	for (int i = 1; i < 5; i++)
	//	{
	//		result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rbg * weight[i];
	//		result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rbg * weight[i];
	//	}
	//}
	//BlurredText = vec4(result, 1.0);
	BlurredText = texture(image, TexCoords);
}