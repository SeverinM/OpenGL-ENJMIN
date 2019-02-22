#version 400

out vec4 color_border;
out vec4 color_fill;
out vec3 bary;

in VertexAttrib
{
	vec4 colorFill;
	vec4 colorBorder;
} vertex[];

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main()
{
	for (int i = 0; i < 3; i++)
	{
		color_border = vertex[i].colorBorder;
		color_fill = vertex[i].colorFill;
		gl_Position = gl_in[i].gl_Position;

		if (i == 0)
			bary = vec3(0, 0, 1);
		if (i == 1)
			bary = vec3(0, 1, 0);
		if (i == 2)
			bary = vec3(1, 0, 0);

		EmitVertex();
	}
	EndPrimitive();
}