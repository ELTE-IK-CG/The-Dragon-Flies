#version 450

in vec2 vs_in_pos;

layout(location = 0) out vec2 vs_out_tex;

void main()
{
	vec2 p = vs_in_pos;
	gl_Position = vec4(p, 0, 1);
	vs_out_tex = p*0.5+0.5;
}
