#version 130

// lok�lis v�ltoz�k: k�t t�mb
vec4 positions[4] = vec4[4](
	vec4(-1, -1, 0, 1),
	vec4( 1, -1, 0, 1),
	vec4(-1,  1, 0, 1),
	vec4( 1,  1, 0, 1)
);

vec4 colors[4] = vec4[4](
	vec4(1, 0, 0, 1),
	vec4(0, 1, 0, 1),
	vec4(0, 0, 1, 1),
	vec4(1, 1, 1, 1)
);

// kimeneti v�ltoz�: a cs�cspont sz�ne
out vec4 vs_out_col;
out vec2 vs_out_pos;

void main()
{
	// gl_VertexID: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_VertexID.xhtml
	gl_Position = positions[gl_VertexID] /* + vec4(0.5, 0.5, 0, 0)*/;
	vs_out_col	= colors[gl_VertexID];
	vs_out_pos = positions[gl_VertexID].xy;
}