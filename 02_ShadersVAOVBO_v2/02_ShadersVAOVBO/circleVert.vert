#version 130

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_col;

uniform float time;

void main()
{

	vec3 pos = vs_in_pos * abs(sin(time * 3.1415));

	gl_Position = vec4( pos, 1 );
	vs_out_pos = pos;
	vs_out_col = vec3(1, 0, 1);
}