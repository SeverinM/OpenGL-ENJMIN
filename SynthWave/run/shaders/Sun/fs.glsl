#version 400

out vec4 color;
in vec3 TexCoords;

uniform samplerCube skybox;

vec4 top = vec4(25.0 / 255.0, 25.0 / 255.0, 77.0 / 255.0, 1);
vec4 bottom = vec4(143.0 / 255.0, 0, 179.0 / 255.0, 1);

void main()
{
	vec4 sampledColor = texture(skybox, TexCoords);
	vec3 normCoords = normalize(TexCoords);
	color = mix(top, bottom, clamp(1 - ((normCoords.z + 1) / 2), 0, 1)) / 1.5;
	color.a = 1;
}