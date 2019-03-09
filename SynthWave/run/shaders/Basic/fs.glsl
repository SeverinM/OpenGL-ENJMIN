#version 400

in vec4 worldPos_vs;
in vec4 normal_vs;
in vec4 view_vs;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 passColor;

vec4 lightColor = vec4(1, 1, 204.0 / 255.0, 1);
float ambiantStrength = 0.1;
float specularStrength = 0;

uniform vec3 sunPos;

void main()
{
	vec4 reflect = reflect(-view_vs, normal_vs);
	float spec = pow(max(dot(view_vs.xyz, reflect.xyz), 0.0), 256);
	vec3 toSun = normalize(sunPos - worldPos_vs.xyz);

	vec4 interColor;
	vec3 ambiant = ambiantStrength * lightColor.rgb;
	vec3 diffuse = lightColor.rgb * max(0, dot(toSun, normal_vs.xyz));
	vec3 specular = lightColor.rgb * spec * specularStrength;

	interColor = vec4(vec3(0.1, 0.1, 0.1) * (specular + diffuse + ambiant), 1);
	interColor.a = 1;

	color = interColor;
	passColor = vec4(0, 0, 0, 1);
}