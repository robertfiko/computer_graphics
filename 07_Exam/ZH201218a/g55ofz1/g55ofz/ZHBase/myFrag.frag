#version 330 core

// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir = vec3(-1,-2, 0.5);

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.1, 0.1, 0.1);
uniform vec3 Ld = vec3(0.1, 0.1, 0.1);

uniform sampler2D texImage;

uniform vec3 color = vec3(0, 0, 0);

void main()
{

	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	
	float cosa = clamp(dot(normal, to_light), 0, 1);

	vec3 diffuse = cosa*Ld;
	
	if (color == vec3(0,0,0))  {
		fs_out_col = vec4(ambient + diffuse, 1) * texture(texImage, vs_out_tex);
	}
	else {
		fs_out_col = vec4(ambient + diffuse, 1) * vec4(color,1);

	}
}