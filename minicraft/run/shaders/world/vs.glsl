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

float specModifier;

out VertexAttrib
{
	vec3 normal;
	vec4 color;
	vec2 uv;
	vec4 posWorld;
	float specModifier;
    float type;
	vec3 directionToCam;
	vec4 colorBorder;
} vertex;

#define CUBE_HERBE 0.0
#define CUBE_TERRE 1.0
#define CUBE_EAU 4.0

vec4 positionGlobal;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	vertex.specModifier = 1;
	positionGlobal = m * vec4(vs_position_in,1.0);
	vertex.posWorld = positionGlobal;

	if(vs_type_in == CUBE_EAU && vs_position_in.z < 2)
	{
		positionGlobal.z -= 1;
		float value = (sin(elapsed + positionGlobal.x - (positionGlobal.y / 2.0f)) / 2.0f) + 0.5f;
		positionGlobal.z += value * 0.5f;
	}

	positionGlobal = v * positionGlobal;
	vertex.directionToCam = positionGlobal.xyz;
	//positionGlobal.y -= pow(length(positionGlobal.xyz)/30,3);

	gl_Position = p * positionGlobal;
	
	vertex.normal = (nmat * vec4(vs_normal_in,1.0)).xyz; 
	vertex.normal = normalize(vertex.normal);

	vertex.uv = vs_uv_in;

	//Couleur par défaut violet
	vertex.color = vec4(1.0,1.0,0.0,1.0);

	specModifier = 1;
	//Couleur fonction du type
	if(vs_type_in == CUBE_HERBE)
	{
		vertex.color = vec4(0,0,0,1);
		vertex.colorBorder = vec4(0,1,1,1);
	}
		
	if(vs_type_in == CUBE_TERRE)
	{
		vertex.color = vec4(0,1,0,1);
		vertex.colorBorder = vec4(255 / 255, 51 / 255, 204 / 255,1);
	}
		
	if(vs_type_in == CUBE_EAU)
	{
		vertex.specModifier = 0;
		vertex.color = vec4(0.0,0.0,1.0,0.7);
		vertex.colorBorder = vec4(1,0,0,1);
	}
}

