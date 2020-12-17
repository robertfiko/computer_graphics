#version 330 core

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// ir�ny f�nyforr�s: f�ny ir�nya
uniform vec3 light_dir = vec3(-1,-1,-1);

//ha pont f�nyforr�s vil�g�t meg
uniform vec3 light_pos = vec3( 5, 5, 5 );

//f�ny tulajdons�gok
uniform vec4 La = vec4(0.50f, 0.1f, 0.1f, 1); 
uniform vec4 Ld = vec4(0.75f, 0.5f, 0.3f, 1);  //f�ny sz�ne
uniform vec4 Ls = vec4(1, 1, 1, 1);

// anyagtulajdons�gok
uniform vec4 Ka = vec4(1, 1, 1, 0); 
uniform vec4 Kd = vec4(0.3f, 1, 1, 1); //Anyag sz�ne (r,g,b,a)
uniform vec4 Ks = vec4(1, 0, 0, 0);

uniform sampler2D texImage;
uniform samplerCube texSkybox;

uniform float specular_power;

uniform vec3 eye_pos;

void main()
{
	vec3 LightWay;

	//Ir�ny f�nyforr�s
	LightWay = -light_dir;

	//Pont f�nyforr�s
	LightWay = light_pos - vs_out_pos;

	//Skybox t�kr�z�d�s sz�m�t�s
	vec3 camToFrag = normalize(vs_out_pos - eye_pos);
	vec3 reflected = reflect(camToFrag, normalize(vs_out_norm));
	vec4 skyboxColor = texture(texSkybox, reflected);

	//ambiens f�ny
	vec4 ambient = La * Ka;

	//diff�z f�ny pontf�nyforr�ssal
	vec3 normal = normalize(vs_out_norm);
	vec3 toLight = normalize(LightWay);
	float di = clamp( dot( toLight, normal), 0.0f, 1.0f );
	vec4 diffuse = vec4(Ld.rgb*Kd.rgb*di, Kd.a);

	//f�nyfolt k�pz�sz�n = spekul�ris csillan�s
	vec4 specular = vec4(0);
	//if ( di > 0 )
	//{
		vec3 e = normalize( eye_pos - vs_out_pos );
		vec3 r = reflect( -toLight, normal );
		float si = pow( clamp( dot(e, r), 0.0f, 1.0f ), specular_power );
		specular = Ls*Ks*si;
	//}


	//Egyszer� sz�n, spekul�ris csillan�ssal
	fs_out_col = (ambient + diffuse + specular);

	//Skybox t�kr�z�d�s n�lk�li text�ra megvil�g�t�s
	fs_out_col = (ambient + diffuse + specular) * texture(texImage, vs_out_tex);

	//Text�rz�zott, SkyBox-os t�kr�z�d�ssel, spekul�ris csillan�ssal
	fs_out_col = mix((ambient + diffuse + specular) * texture(texImage, vs_out_tex), skyboxColor, 0.2);
}