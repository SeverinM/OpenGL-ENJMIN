#version 400

uniform float elapsed;
uniform mat4 mvp;
uniform mat4 nmat;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform float world_size;

layout(location=0) in vec3 vs_position_in;
layout(location=1) in vec3 vs_normal_in;
layout(location=2) in vec2 vs_uv_in;
layout(location=3) in float vs_type_in;

//Variables en sortie
out vec3 normal;
out vec4 color;
out vec2 uv;
out vec3 posWorld;
out float specModifier;
out vec3 type;
out vec3 directionToCam;

#define CUBE_HERBE 0.0
#define CUBE_TERRE 1.0
#define CUBE_EAU 4.0

vec4 positionGlobal;

void main()
{
	positionGlobal = m * vec4(vs_position_in,1.0);
	posWorld = positionGlobal.xyz;

	if(vs_type_in == CUBE_EAU && vs_position_in.z < 2)
	{
		positionGlobal.z -= 1;
		float value = (sin(elapsed + positionGlobal.x - (positionGlobal.y / 2.0f)) / 2.0f) + 0.5f;
		positionGlobal.z += value * 0.5f;
	}

	positionGlobal = v * positionGlobal;
	directionToCam = positionGlobal.xyz;
	//positionGlobal.y -= pow(length(positionGlobal.xyz)/30,3);

	gl_Position = p * positionGlobal;
	
	normal = (nmat * vec4(vs_normal_in,1.0)).xyz; 
	normal = normalize(normal);

	uv = vs_uv_in;

	//Couleur par défaut violet
	color = vec4(1.0,1.0,0.0,1.0);

	specModifier = 1;
	//Couleur fonction du type
	if(vs_type_in == CUBE_HERBE)
		color = vec4(0,1,0,1);
	if(vs_type_in == CUBE_TERRE)
		color = vec4(0.2,0.1,0,1);
	if(vs_type_in == CUBE_EAU)
	{
		color = vec4(0.0,0.0,1.0,0.7);
		specModifier = 0;
	}
}