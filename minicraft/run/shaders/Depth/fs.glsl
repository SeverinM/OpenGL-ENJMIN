#version 400

//Variables en entree
in vec3 normal;
in vec4 color;
in vec2 uv;

out vec4 color_out;

float near = 0.1; 
float far  = 800.0; 

uniform sampler2D ourTexture;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	color_out = vec4(vec3(depth), 1.0);
}