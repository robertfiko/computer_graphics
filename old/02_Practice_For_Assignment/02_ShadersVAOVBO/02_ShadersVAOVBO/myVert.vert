#version 130

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_col;

void main()
{
	gl_Position = vec4( vs_in_pos, 1 );
	vs_out_pos = vs_in_pos;
	vs_out_col = vs_in_col;
}