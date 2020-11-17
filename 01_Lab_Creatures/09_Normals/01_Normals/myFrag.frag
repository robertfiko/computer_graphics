#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir = vec3(-1,-1,-1);
uniform sampler2D image;

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.4, 0.6, 0.6);
vec3 Ls = vec3(0.2, 0.9, 0.2);

// anyagtulajdonságok: ambiens, diffúz, ...
uniform vec3 Ka = vec3(0.2, 0.4, 0.6);
uniform vec3 Kd = vec3(0.2, 0.4, 0.6);
vec3 Ks = vec3(1, 1, 1);

uniform vec3 eyePos = vec3(1,1,1);

//uniform sampler2D texImage;

void main()
{	
	//
	// ambiens szín számítása
	//

	vec3 ambient = La*Ka;

	//
	// diffúz szín számítása
	//	
	/* segítség:
	    - normalizálás: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skaláris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
	*/

	vec3 diffuse = Ld*Kd*clamp(dot(normalize(vs_out_norm),-light_dir),0.0f,1.0f);

	//
	// fényfoltképzõ szín
	//
	/* segítség:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/

	vec3 r = reflect(light_dir, normalize(vs_out_norm));
	vec3 c = normalize(eyePos-vs_out_pos);
	float si = pow(clamp(dot(r,c),0.0f, 1.0f),28);
	vec3 specular = r*c*si;
	
	//
	// a fragment végsõ színének meghatározása
	//

	//fs_out_col = vec4(ambient + diffuse + specular, 1);

	// felületi normális
	fs_out_col = vec4(vs_out_norm, 1);



	// textúrával
	vec4 textureColor = texture(image, vs_out_tex);
	//fs_out_col = textureColor;
	fs_out_col = vec4(ambient + diffuse  + specular, 1) * textureColor;
}

// Feladatok

// 1) Fényszámítás
// - ambiens
// - diffúz
// - spekuláris

// 2) Textúra
