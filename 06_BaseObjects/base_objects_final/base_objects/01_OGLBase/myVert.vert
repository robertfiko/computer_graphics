#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader k�ls� param�terei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;


//Ha hullm�zni kell
//CPP: oldalon, UNIFORM-k�nt adj �t mindent az render()-ben
uniform float time = 0;

//Cser�ld ki amire kell
vec3 GetPos(float u, float v)
{
	return vec3(u, sin(u*v*100 + time) / 8, -v);
}

//Cser�ld ki amire kell
vec3 GetNorm(float u, float v)
{
	vec3 du = GetPos(u+0.01, v)-GetPos(u-0.01, v);
	vec3 dv = GetPos(u, v+0.01)-GetPos(u, v-0.01);

	return normalize(cross(du, dv));
}

void main()
{
	vec3 position;
	vec3 normal;
	//Ha shaderben sz�molunk parametrikust
	position = GetPos(vs_in_tex.x, vs_in_tex.y);
	normal = GetNorm(vs_in_tex.x, vs_in_tex.y);

	//Ha k�szen kapjuk a pozici�t �s a norm�lt a CPU-r�l
	position = vs_in_pos;
	normal = vs_in_norm;

	gl_Position = MVP * vec4( position, 1 );
	
	vs_out_pos = (world * vec4(position, 1)).xyz;
	vs_out_norm = (worldIT * vec4(normal, 0)).xyz;
	vs_out_tex = vs_in_tex;
}