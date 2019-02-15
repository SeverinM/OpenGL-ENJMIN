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
in vec4 colorBorder_gs;

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

	color_out = color_gs;
	bool ok = false;
	if (abs(bary.x) < 0.05)
	{
		color_out = mix(colorBorder_gs, color_gs ,bary.x * 20);
		ok = true;
	}

	if (abs(bary.y) < 0.05)
	{
		color_out = mix(colorBorder_gs, color_gs ,bary.y * 20);
		ok = true;
	}

	if (abs(bary.y) < 0.05 && abs(bary.y) < 0.05)
	{
		color_out = max(color_out,mix(colorBorder_gs, color_gs ,bary.x * 20));
	}

	if (!ok)
	{
		//color_out += spec * sunColor * specModifier_gs;
	}
}
