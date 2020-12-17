#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_vaoID = 0;
	m_vboID = 0;
	m_ibID = 0;

	m_programID = 0;

	m_loadedTextureID = 0;
}


CMyApp::~CMyApp(void)
{
}

//
// egy parametrikus fel�let (u,v) param�ter�rt�kekhez tartoz� pontj�nak
// kisz�m�t�s�t v�gz� f�ggv�ny
//
glm::vec3 CMyApp::GetPos(float u, float v)
{
	// orig� k�z�ppont�, r sugar� g�mb parametrikus alakja: http://hu.wikipedia.org/wiki/G%C3%B6mb#Egyenletek 
	// figyelj�nk:	matematik�ban sokszor a Z tengely mutat felfel�, de n�lunk az Y, teh�t a legt�bb k�plethez k�pest n�lunk
	//				az Y �s Z koordin�t�k felcser�lve szerepelnek
	u *= float(2 * M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	float r = 2;

	return glm::vec3(r*cu*sv, r*cv, r*su*sv);
}
//
// egy parametrikus fel�let (u,v) param�ter�rt�kekhez tartoz� norm�lvektor�nak
// kisz�m�t�s�t v�gz� f�ggv�ny
//
glm::vec3 CMyApp::GetNorm(float u, float v)
{
	// K�plettel
	u *= float(2 * M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	return glm::vec3(cu*sv, cv, su*sv);

	// Numerikusan (nem kell ismerni a k�pletet, el�g a poz�ci��t)
	
	/*glm::vec3 du = GetPos(u+0.01, v)-GetPos(u-0.01, v);
	glm::vec3 dv = GetPos(u, v+0.01)-GetPos(u, v-0.01);

	return glm::normalize(glm::cross(du, dv));*/
}

glm::vec2 CMyApp::GetTex(float u, float v)
{
	return glm::vec2(1 - u, 1 - v);
}

void CMyApp::InitSphere()
{
	// NxM darab n�gysz�ggel k�zel�tj�k a parametrikus fel�let�nket => (N+1)x(M+1) pontban kell ki�rt�kelni
	Vertex vert[(N + 1)*(M + 1)];
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].p = GetPos(u, v);
			vert[i + j * (N + 1)].n = GetNorm(u, v);
			vert[i + j * (N + 1)].t = GetTex(u, v);
		}

	// indexpuffer adatai: NxM n�gysz�g = 2xNxM h�romsz�g = h�romsz�glista eset�n 3x2xNxM index
	GLushort indices[3 * 2 * (N)*(M)];
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			// minden n�gysz�gre csin�ljunk kett� h�romsz�get, amelyek a k�vetkez� 
			// (i,j) indexekn�l sz�letett (u_i, v_i) param�ter�rt�kekhez tartoz�
			// pontokat k�tik �ssze:
			//
			//		(i,j+1)
			//		  o-----o(i+1,j+1)
			//		  |\    |			a = p(u_i, v_i)
			//		  | \   |			b = p(u_{i+1}, v_i)
			//		  |  \  |			c = p(u_i, v_{i+1})
			//		  |   \ |			d = p(u_{i+1}, v_{i+1})
			//		  |	   \|
			//	(i,j) o-----o(i+1, j)
			//
			// - az (i,j)-hez tart�z� 1D-s index a VBO-ban: i+j*(N+1)
			// - az (i,j)-hez tart�z� 1D-s index az IB-ben: i*6+j*6*(N+1) 
			//		(mert minden n�gysz�gh�z 2db h�romsz�g = 6 index tartozik)
			//
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j)*	(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j)*	(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1)*(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j)*	(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1)*(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1)*(N + 1);
		}


	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_vaoID);

	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
		sizeof(vert),		// ennyi b�jt nagys�gban
		vert,	// err�l a rendszermem�riabeli c�mr�l olvasva
		GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat


// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci�
	glVertexAttribPointer(
		0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
	);

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (norm�lis)
	glEnableVertexAttribArray(1); // ez lesz majd a norm�lis
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	// text�rakoordin�t�k bekapcsol�sa a 2-es azonos�t�j� attrib�tom csatorn�n
	glEnableVertexAttribArray(2); // ez lesz majd a text�ra
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));

	// index puffer l�trehoz�sa
	glGenBuffers(1, &m_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);








	Vertex plane[] = {
		{glm::vec3(-1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(0, 0)},
		{glm::vec3(1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1, 0)},
		{glm::vec3(1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(1, 1)},
		{glm::vec3(-1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1)}
	};

	GLushort planeIndices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_planeVAO);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_planeVAO);

	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_planeVBO); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
		sizeof(plane),		// ennyi b�jt nagys�gban
		plane,	// err�l a rendszermem�riabeli c�mr�l olvasva
		GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat


	// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci�
	glVertexAttribPointer(
		0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
	);

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (norm�lis)
	glEnableVertexAttribArray(1); // ez lesz majd a norm�lis
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	// text�rakoordin�t�k bekapcsol�sa a 2-es azonos�t�j� attrib�tom csatorn�n
	glEnableVertexAttribArray(2); // ez lesz majd a text�ra
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));

	// index puffer l�trehoz�sa
	glGenBuffers(1, &m_planeIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planeIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);



	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le


}

void CMyApp::InitShaders()
{
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attrib�tumok hozz�rendel�se a shader v�ltoz�khoz
	// FONTOS: linkel�s el�tt kell ezt megtenni!
	glBindAttribLocation(m_programID,	// shader azonos�t�ja, amib�l egy v�ltoz�hoz szeretn�nk hozz�rendel�st csin�lni
		0,				// a VAO-beli azonos�t� index
		"vs_in_pos");	// a shader-beli v�ltoz�n�v
	glBindAttribLocation(m_programID, 1, "vs_in_col");
	glBindAttribLocation(m_programID, 2, "vs_in_tex0");

	// illessz�k �ssze a shadereket (kimen�-bemen� v�ltoz�k �sszerendel�se stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength != 0)
	{
		std::vector<char> error_massage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, error_massage.data());

		std::cout << "Hiba a shader letrehozasakor: " << error_massage.data() << std::endl;
	}
	// mar nincs ezekre szukseg
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);
}

void CMyApp::InitTextures()
{
	// f�jlb�l bet�lt�s
	m_loadedTextureID = TextureFromFile("earth.jpg");
	// mintav�telez�s be�ll�t�sai
	glBindTexture(GL_TEXTURE_2D, m_loadedTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// mi legyen az eredm�ny, ha a text�r�n k�v�lr�l pr�b�lunk mint�t venni?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // v�zszintesen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // f�gg�legesen

	glBindTexture(GL_TEXTURE_2D, 0);
}

bool CMyApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)
	glCullFace(GL_BACK); // GL_BACK: a kamer�t�l "elfel�" n�z� lapok, GL_FRONT: a kamera fel� n�z� lapok

	InitSphere();
	InitShaders();
	InitTextures();

	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);
	
	// shaderbeli uniform v�ltoz�k hely�nek lek�rdez�se
	m_loc_mvp = glGetUniformLocation(m_programID, "MVP");
	m_loc_world = glGetUniformLocation(m_programID, "world");
	m_loc_worldIT = glGetUniformLocation(m_programID, "worldIT");
	//m_loc_tex = glGetUniformLocation(m_programID, "texImage");
	m_loc_spot_pos = glGetUniformLocation(m_programID, "light_spot_pos");
	m_loc_spot_dir = glGetUniformLocation(m_programID, "light_spot_dir");

	m_mesh = ObjParser::parse("assets/Suzanne.obj");
	m_mesh->initBuffers();
	
	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_loadedTextureID);

	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram(m_programID);
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.SetView(m_camera.GetEye(), m_camera.GetAt(), glm::vec3(0.3, 1, 0));
	m_camera.Update(delta_time);

	if (move_light)
	{
		cam_pos = m_camera.GetEye();
		cam_dir = m_camera.GetAt() - cam_pos;
	}

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
	glUseProgram(m_programID);

	//
	// - G�mb kirajzol�sa
	//

	glm::mat4 viewProj = m_camera.GetViewProj();
	glm::mat4 world = glm::mat4(1.0f);
	glm::mat4 wolrdIT = glm::inverse(glm::transpose(world));
	glm::mat4 mvp = viewProj * world;
	glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &world[0][0]);
	glUniformMatrix4fv(m_loc_worldIT, 1, GL_FALSE, &wolrdIT[0][0]);

	glUniform3fv(m_loc_spot_pos, 1, &cam_pos[0]);
	glUniform3fv(m_loc_spot_dir, 1, &cam_dir[0]);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_loadedTextureID);
	glUniform1i(glGetUniformLocation(m_programID, "texImage"), 0);

	glm::vec3 cam_pos = m_camera.GetEye();
	glUniform3fv(glGetUniformLocation(m_programID, "cam_pos"), 1, &cam_pos[0]);

	// feladat - text�ra

	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_vaoID);

	// kirajzol�s
	//A draw h�v�sokhoz a VAO �s a program bindolva kell legyenek (glUseProgram() �s glBindVertexArray())

	glDrawElements(GL_TRIANGLES,	// primit�v t�pus
		3 * 2 * (N)*(M),			// hany csucspontot hasznalunk a kirajzolashoz
		GL_UNSIGNED_SHORT,			// indexek tipusa
		0);							// indexek eltol�sa


	// Suzanne

	world = glm::translate(glm::vec3(10, 0, 0));
	wolrdIT = glm::inverse(glm::transpose(world));
	mvp = viewProj * world;
	glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &world[0][0]);
	glUniformMatrix4fv(m_loc_worldIT, 1, GL_FALSE, &wolrdIT[0][0]);
	m_mesh->draw();


	// Plane

	world = glm::translate(glm::vec3(0, -5, 0)) * glm::scale(glm::vec3(30, 1, 30));
	wolrdIT = glm::inverse(glm::transpose(world));
	mvp = viewProj * world;
	glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &world[0][0]);
	glUniformMatrix4fv(m_loc_worldIT, 1, GL_FALSE, &wolrdIT[0][0]);

	glBindVertexArray(m_planeVAO);
	glDrawElements(GL_TRIANGLES,
		6,
		GL_UNSIGNED_SHORT,
		0);


	// VAO kikapcsolasa
	glBindVertexArray(0);

	// text�ra kikapcsol�sa
	glBindTexture(GL_TEXTURE_2D, 0);

	// shader kikapcsolasa
	glUseProgram(0);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);

	if (key.keysym.sym == SDLK_SPACE) move_light = !move_light;
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}
