#version 400

//Variables en entree
in vec3 normal;
in vec4 color;
in vec2 uv;

out vec4 color_out;

uniform sampler2D ourTexture;

//Globales
const float ambientLevel = 0.4;

void main()
{
	color_out = texture(ourTexture,uv);
}