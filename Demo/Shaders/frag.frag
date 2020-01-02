#version 410

uniform sampler2D texImg;

layout(location = 0) in vec2 fs_in_tex;
out vec4 fs_out_col;

void main()
{
	fs_out_col = texture(texImg, fs_in_tex);
}
