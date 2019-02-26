#version 400
in vec3 TexCoords;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 passColor;
layout(location = 2) out vec4 depth;

uniform samplerCube skybox;

vec4 top = vec4(25.0 / 255.0, 25.0 / 255.0, 77.0 / 255.0, 1);
vec4 bottom = vec4(143.0 / 255.0, 0, 179.0 / 255.0, 1);

float toleranceLight = 0.7;

void main()
{
	vec4 sampledColor = texture(skybox, TexCoords);
	vec3 normCoords = normalize(TexCoords);
	color = mix(top, bottom, clamp(1 - ((normCoords.z + 1) / 2), 0, 1)) / 1.5;
	if (sampledColor.a == 1)
	{
		color = sampledColor / 2;
	}
	color.a = 1;

	if (max(color.r, max(color.g, color.b)) > toleranceLight)
	{
		passColor = color;
	}
	else
	{
		passColor = vec4(0, 0, 0, 1);
	}
	depth = vec4(gl_FragCoord.zzz, 1.0);
}