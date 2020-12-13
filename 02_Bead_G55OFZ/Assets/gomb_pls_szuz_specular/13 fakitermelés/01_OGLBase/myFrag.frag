#version 330 core

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// ir�ny f�nyforr�s: f�ny ir�nya
//uniform vec3 light_dir = vec3(-1,-1,-1);

// f�nytulajdons�gok: ambiens, diff�z, ...
//uniform vec3 La = vec3(0.4, 0.4, 0.4);
//uniform vec3 Ld = vec3(0.6, 0.6, 0.6);



uniform vec3 camPos = vec3(0, 15, 15);




// sz�nt�r tulajdons�gok
uniform vec3 eye_pos ;

// f�nytulajdons�gok
uniform vec3 light_pos = vec3( 5, 5, 5 );

uniform vec4 La = vec4(0.1f, 0.1f, 0.1f, 1);
uniform vec4 Ld = vec4(0.75f, 0.5f, 0.3f, 1);
uniform vec4 Ls = vec4(1, 1, 1, 1);

// anyagtulajdons�gok
uniform vec4 Ka = vec4(1, 1, 1, 0);
uniform vec4 Kd = vec4(0.3f, 1, 1, 1);
uniform vec4 Ks = vec4(0, 1, 0, 0);
uniform float specular_power = 30;




void main()
{

	vec4 ambient = La * Ka;

	//diff�z
	vec3 normal = normalize(vs_out_norm);
	vec3 toLight = normalize(light_pos - vs_out_pos);
	float di = clamp( dot( toLight, normal), 0.0f, 1.0f );
	vec4 diffuse = vec4(Ld.rgb*Kd.rgb*di, Kd.a);

	//f�nyfolt k�pz�sz�n
	vec4 specular = vec4(0);
	if ( di > 0 )
	{
		vec3 e = normalize( eye_pos - vs_out_pos );
		vec3 r = reflect( -toLight, normal );
		float si = pow( clamp( dot(e, r), 0.0f, 1.0f ), specular_power );
		specular = Ls*Ks*si;
	}



	
	//float cosa = clamp(dot(normal, toLight), 0, 1);



	//vec3 camToFrag = normalize(vs_out_pos - camPos);
	//vec3 reflected = reflect(camToFrag, normalize(vs_out_norm));




	


	

	//fs_out_col = mix(vec4(ambient + diffuse, 1) * vec4(0,1,0,0), vec4(0,0,0,0), 0.2);
	fs_out_col = (ambient + diffuse + specular); //* vec4(0,1,0,0);
}