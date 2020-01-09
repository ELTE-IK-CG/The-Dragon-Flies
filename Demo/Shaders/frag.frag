#version 410

uniform sampler2D myTexUniform;

layout(location = 0) in vec2 fs_in_tex;
out vec4 fs_out_col;

void main()
{
	fs_out_col = texture(myTexUniform, fs_in_tex);
}
