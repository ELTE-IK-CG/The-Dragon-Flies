#version 410

const vec2 pos[3] = vec2[3](vec2(-1), vec2(3,-1), vec2(-1,3));

layout(location = 0) out vec2 vs_out_tex;

void main()
{
	vec2 p = pos[gl_VertexID];
	gl_Position = vec4(p, 0, 1);
	vs_out_tex = p*0.5+0.5;
}
