#version 400

in vec2 uv;

uniform sampler2D TexColor;
uniform sampler2D TexDepth;
uniform sampler2D TexNormal;
uniform float screen_width;
uniform float screen_height;
uniform vec2 near_far;

out vec4 color_out;

float LinearizeDepth(float z)
{
	float n = near_far.x; // camera z near
  	float f = near_far.y; // camera z far
  	return (2.0 * n) / (f + n - z * (f - n));
}

void main (void)
{
	float xstep = 1.0/screen_width;
	float ystep = 1.0/screen_height;
	float ratio = screen_width / screen_height;

	vec4 color = texture2D( TexColor , uv );
	vec4 colorNorm = texture2D( TexNormal, uv);
	float depth = texture2D( TexDepth , uv ).r;

	float depth2 = texture2D( TexDepth , vec2(uv.x,uv.y + ystep)).r;
	float depth3 = texture2D( TexDepth , vec2(uv.x,uv.y - ystep)).r;
	float depth4 = texture2D( TexDepth , vec2(uv.x + xstep,uv.y)).r;
	float depth5 = texture2D( TexDepth , vec2(uv.x - xstep,uv.y)).r;
	
	//Permet de scaler la profondeur
	depth = LinearizeDepth(depth);
	depth2 = LinearizeDepth(depth2);
	depth3 = LinearizeDepth(depth3);
	depth4 = LinearizeDepth(depth4);

	if (abs(depth - depth2) > 0.2f || abs(depth - depth2) > 0.2f || abs(depth - depth3) > 0.2f || abs(depth - depth4) > 0.2f)
		color_out =  vec4(color.rgb,1.0);
	else
		color_out = vec4(color.rgb,1.0);

	if (length(vec2(0.5,0.5) - uv) < 0.002)
		color_out = vec4(1,1,1,1);

    //Gamma correction
    //color.r = pow(color.r,1.0/2.2);
    //color.g = pow(color.g,1.0/2.2);
    //color.b = pow(color.b,1.0/2.2);

	color_out = colorNorm;

}