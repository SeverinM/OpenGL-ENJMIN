#version 400

uniform float elapsed;
uniform mat4 mvp;
uniform mat4 nmat;
uniform mat4 m;

layout(location=0) in vec3 vs_position_in;
layout(location=1) in vec3 vs_normal_in;
layout(location=2) in vec2 vs_uv_in;
layout(location=3) in float vs_type_in;

//Variables en sortie
out vec3 normal;
out vec4 color;
out vec2 uv;

void main()
{
	vec4 vecIn = vec4(vs_position_in,1.0);
	gl_Position = mvp * vecIn;
	
	normal = (nmat * vec4(vs_normal_in,1.0)).xyz; 
	normal = normalize(normal);

	uv = vs_uv_in;

	//Couleur par défaut violet
	color = vec4(1.0,1.0,0.0,1.0);
}