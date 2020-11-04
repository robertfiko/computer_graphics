#version 130

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_col;

// shader k�ls� param�terei - most a h�rom transzform�ci�s m�trixot egyben vessz�k �t
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	vs_out_col = vs_in_col;
}