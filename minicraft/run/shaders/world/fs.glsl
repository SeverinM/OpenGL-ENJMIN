#version 400

//Variables en entree
in vec3 normal;
in vec4 color;
in vec2 uv;
in vec3 posWorld;
in float specModifier;
in float type;

out vec4 color_out;

uniform sampler2D ourTexture;
uniform float slider_0; //Diffuse
uniform float slider_1; //Ambiant
uniform float slider_2; //Spec
uniform float elapsed;

uniform vec3 skyColor;
uniform vec3 camPos;
uniform vec3 sunPos;
uniform vec2 world_size;

vec4 sunColor;

#define CUBE_HERBE 0.0
#define CUBE_TERRE 1.0
#define CUBE_EAU 4.0


//Globales
const float ambientLevel = 0.4;

vec3 lightDir;

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

void main()
{
	lightDir = (sunPos - posWorld);
	sunColor = vec4(1,1,1,1);
	vec4 color = texture(ourTexture,uv);

	//Diffuse 
	color *= dot(normalize(lightDir),normalize(normal));

	//Ambiant
	color += vec4(skyColor * slider_1, 1.0);

	//Specular
	vec3 halfVector = normalize(normalize(lightDir) + normalize(camPos - posWorld));
    float spec = max(0, dot(normal,halfVector));
    spec = pow(spec, 25 + (slider_2 * 200));
    color += specModifier * spec * sunColor;

	color_out = color;
}
