#version 400

in vec4 worldPos_vs;
in vec4 normal_vs;
in vec4 view_vs;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 passColor;

float ambiantStrength = 0.1;
float specularStrength = 0.5;

vec3 colorSpecular = vec3(0, 0, 1);
vec3 colorDiffuse = vec3(1, 1, 1);
vec3 colorAmbiant = vec3(0, 0, 0.1);

uniform vec3 sunPos;

void main()
{
	vec4 reflect = reflect(-view_vs, normal_vs);
	float spec = pow(max(dot(normalize(view_vs.xyz), normalize(reflect.xyz)), 0.0), 512);
	vec3 toSun = normalize(sunPos - worldPos_vs.xyz);

	vec4 interColor;
	vec3 ambiant = ambiantStrength * colorAmbiant;
	vec3 diffuse = colorDiffuse * max(0, dot(toSun, normal_vs.xyz));
	vec3 specular = colorSpecular * spec * specularStrength;

	interColor = vec4(vec3(1, 1, 1) * (specular + ambiant + diffuse), 1);

	color = interColor;
	passColor = vec4(0, 0, 0, 1);
}