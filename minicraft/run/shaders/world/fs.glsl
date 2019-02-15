#version 400

//Variables en entree
in vec3 normal_gs;
in vec4 color_gs;
in vec2 uv_gs;
in vec4 posWorld_gs;
in float specModifier_gs;
in float type_gs;
in vec3 directionToCam_gs;
in vec3 bary;

out vec4 color_out;

uniform sampler2D ourTexture;
uniform float slider_0; //Diffuse
uniform float slider_1; //Ambiant
uniform float slider_2; //Spec
uniform float slider_3; //Fog
uniform float elapsed;

uniform vec3 skyColor;
uniform vec3 camPos;
uniform vec3 sunPos;


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
	lightDir = (sunPos - posWorld_gs.xyz);
	sunColor = vec4(1,1,1,1);
	vec4 color = texture(ourTexture,uv_gs);

	//Diffuse 
	color *= dot(normalize(lightDir),normalize(normal_gs));

	//Ambiant
	color += vec4(skyColor * slider_1, 1.0);

	//Specular
	vec3 halfVector = normalize(normalize(lightDir) + normalize(camPos - posWorld_gs.xyz));
    float spec = max(0, dot(normal_gs,halfVector));
    spec = pow(spec, 25 + (slider_2 * 200));
    color += spec * sunColor * specModifier_gs;

	//Fog
	vec4 colorFog = vec4(167.0 /255.0,167.0/255.0, 167.0 /255.0,1);
	float min = 0;
	float max = 10;
	float lerpValue = clamp(length(directionToCam_gs) / max,0,1);
	//color = mix(color,colorFog,lerpValue);

	if (bary.x < 0.001 || bary.y < 0.001)
	{
		color_out = vec4(0,0,0,1);
	}
	else
	{
		color_out = color;
	}
}
