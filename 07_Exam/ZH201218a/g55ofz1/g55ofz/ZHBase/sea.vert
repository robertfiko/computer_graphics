#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader külsõ paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

uniform float time;


void main()
{
	vec3 position = vs_in_pos;
	vec2 text = vs_in_tex;

	position.y += sin(position.x + time) * 0.5;
	
	text.x += time / 10.0;




	gl_Position = MVP * vec4( position, 1 );
	
	vs_out_pos = (world * vec4(position, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = text;

}