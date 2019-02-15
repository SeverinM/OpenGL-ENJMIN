#version 400

out vec3 normal_gs;
out vec4 color_gs;
out vec2 uv_gs;
out vec4 posWorld_gs;
out float type_gs;
out vec3 directionToCam_gs;
out float specModifier_gs;
out vec3 bary;

in VertexAttrib
{
	vec3 normal;
	vec4 color;
	vec2 uv;
	vec4 posWorld;
	float specModifier;
    float type;
	vec3 directionToCam;
} vertex[];

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main() { 

	for (int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		normal_gs = vertex[i].normal;
		color_gs = vertex[i].color;
		uv_gs = vertex[i].uv;
		posWorld_gs = vertex[i].posWorld;
		type_gs = vertex[i].type;
		directionToCam_gs = vertex[i].directionToCam;
		specModifier_gs = vertex[i].specModifier;

		if (i == 0)
			bary = vec3(0,0,1);
		if (i == 1)
			bary = vec3(0,1,0);
		if (i == 2)
			bary = vec3(1,0,0);

		EmitVertex();
	}
    EndPrimitive();
}  