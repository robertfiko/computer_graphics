#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_suzanneMesh = nullptr;
}

CMyApp::~CMyApp(void)
{
	std::cout << "Destructor!\n";
}

//Cube
void CMyApp::InitCube()
{
	std::vector<Vertex>vertices;
	
	//front									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 1) });
	//back
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 1) });
	//right									 
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 1) });
	//left									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });
	//top									 
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
	//bottom								 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(1, 1) });

	std::vector<int> indices(36);
	int index = 0;
	for (int i = 0; i < 6 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;
		indices[index + 3] = i + 1;
		indices[index + 4] = i + 3;
		indices[index + 5] = i + 2;
		index += 6;
	}

	m_CubeVertexBuffer.BufferData(vertices);
	m_CubeIndices.BufferData(indices);
	m_CubeVao.Init(
		{
			// 0-�s attrib�tum "l�nyeg�ben" glm::vec3-ak sorozata �s az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attrib�tum: 0
									glm::vec3,				// CPU oldali adatt�pus amit a 0-�s attrib�tum meghat�roz�s�ra haszn�ltunk <- az elj�r�s a glm::vec3-b�l kik�vetkezteti, hogy 3 darab float-b�l �ll a 0-�s attrib�tum
									0,						// offset: az attrib�tum t�rol� elej�t�l vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a k�vetkez� cs�cspont ezen attrib�tuma h�ny byte-ra van az aktu�list�l
								>, m_CubeVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
		},
		m_CubeIndices
	);
}



//Plane
glm::vec3 CMyApp::GetPosPlane(float u, float v)
{
	return glm::vec3(u, sin(u * 10), -v);
}

glm::vec3 CMyApp::GetNormPlane(float u, float v)
{
	glm::vec3 du = GetPosPlane(u + 0.01, v) - GetPosPlane(u - 0.01, v);
	glm::vec3 dv = GetPosPlane(u, v + 0.01) - GetPosPlane(u, v - 0.01);

	return glm::normalize(glm::cross(du, dv));
}

void CMyApp::InitPlane()
{
	std::vector<Vertex> vert((N + 1) * (M + 1));
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].t = glm::vec2(u, v);
			vert[i + j * (N + 1)].p = GetPosPlane(u, v);
			vert[i + j * (N + 1)].n = GetNormPlane(u, v);
		}

	std::vector<int> indices(3 * 2 * (N) * (M));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1) * (N + 1);
		}

	m_PlaneVbo.BufferData(vert);
	m_PlaneIndices.BufferData(indices);
	m_PlaneVao.Init(
		{
			// 0-�s attrib�tum "l�nyeg�ben" glm::vec3-ak sorozata �s az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attrib�tum: 0
									glm::vec3,				// CPU oldali adatt�pus amit a 0-�s attrib�tum meghat�roz�s�ra haszn�ltunk <- az elj�r�s a glm::vec3-b�l kik�vetkezteti, hogy 3 darab float-b�l �ll a 0-�s attrib�tum
									0,						// offset: az attrib�tum t�rol� elej�t�l vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a k�vetkez� cs�cspont ezen attrib�tuma h�ny byte-ra van az aktu�list�l
								>, m_PlaneVbo },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_PlaneVbo },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_PlaneVbo },
		},
		m_PlaneIndices
	);
}



//Sphere
glm::vec3 CMyApp::GetSpherePos(float u, float v)
{
	u *= float(2 * M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	float r = 1;

	return glm::vec3(r * cu * sv, r * cv, r * su * sv);
}

glm::vec3 CMyApp::GetSphereNorm(float u, float v)
{
	u *= float(2 * M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);

	return glm::vec3(cu * sv, cv, su * sv);
}

void CMyApp::InitSphere()
{
	std::vector<Vertex> vert((N + 1) * (M + 1));
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].t = glm::vec2(u, v);
			vert[i + j * (N + 1)].p = GetSpherePos(u, v);
			vert[i + j * (N + 1)].n = GetSphereNorm(u, v);
		}

	std::vector<int> indices(3 * 2 * (N) * (M));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1) * (N + 1);
		}

	m_SphereVbo.BufferData(vert);
	m_SphereIndices.BufferData(indices);
	m_SphereVao.Init(
		{
			// 0-�s attrib�tum "l�nyeg�ben" glm::vec3-ak sorozata �s az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attrib�tum: 0
									glm::vec3,				// CPU oldali adatt�pus amit a 0-�s attrib�tum meghat�roz�s�ra haszn�ltunk <- az elj�r�s a glm::vec3-b�l kik�vetkezteti, hogy 3 darab float-b�l �ll a 0-�s attrib�tum
									0,						// offset: az attrib�tum t�rol� elej�t�l vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a k�vetkez� cs�cspont ezen attrib�tuma h�ny byte-ra van az aktu�list�l
								>, m_SphereVbo },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVbo },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVbo },
		},
		m_SphereIndices
	);
}



//Skybox
void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// h�ts� lap
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// el�ls� lap
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);

	// �s a primit�veket alkot� cs�cspontok indexei (az el�z� t�mb�kb�l) - triangle list-el val� kirajzol�sra felk�sz�lve
	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// h�ts� lap
			0, 1, 2,
			2, 3, 0,
			// el�ls� lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// als�
			1, 0, 4,
			1, 4, 5,
			// fels�
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban val� regisztr�l�sa
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TextureFromFileAttach("assets/xpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFileAttach("assets/xneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFileAttach("assets/ypos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFileAttach("assets/yneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFileAttach("assets/zpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFileAttach("assets/zneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}
/*
void CMyApp::InitBackground()
{
	m_SkyboxPos.BufferData(
		std::vector<Vertex>{
			// h�ts� lap
			{glm::vec3(-1, -1, -1), },
			{glm::vec3(1, -1, -1), },
			{glm::vec3(1, 1, -1), },
			{glm::vec3(-1, 1, -1),},
			// el�ls� lap		 
			{glm::vec3(-1, -1, 1),},
			{glm::vec
			3(1, -1, 1), },
			{glm::vec3(1, 1, 1),  },
			{glm::vec3(-1, 1, 1), },
	}
	);
}*/

void CMyApp::InitShaders()
{
	// a shadereket t�rol� program l�trehoz�sa az OpenGL-hez hasonl� m�don:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
	});

	m_program.LinkProgram();

	// shader program r�vid l�trehoz�sa, egyetlen f�ggv�nyh�v�ssal a fenti h�rom:
	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
	);

	m_programAxes.Init(
		{
			{ GL_VERTEX_SHADER, "axes.vert"},
			{ GL_FRAGMENT_SHADER, "axes.frag"}
		},
		{}
	);
}

bool CMyApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)

	InitShaders();
	InitCube();
	InitSkyBox();
	InitPlane();
	InitSphere();

	// egy�b text�r�k bet�lt�se
	m_woodTexture.FromFile("assets/wood.jpg");
	m_suzanneTexture.FromFile("assets/marron.jpg");
	m_grassTexture.FromFile("assets/grass.jpg");

	// suzanne mesh betoltese
	m_suzanneMesh = ObjParser::parse("assets/Suzanne.obj");
	m_suzanneMesh->initBuffers();

	m_cylinderMesh = ObjParser::parse("assets/henger.obj");
	m_cylinderMesh->initBuffers();
	
	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);


	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			trees[i][j] = 0;
		}
	}

	trees[5][5] = 1;
	trees[10][4] = 0.4f;
	trees[16][15] = 0.8f;



	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);

	delete m_suzanneMesh;
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	time += delta_time * 0;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::DrawSphere(glm::mat4& world, ProgramObject& program)
{
	program.Use();
	glm::mat4 viewProj = m_camera.GetViewProj();

	m_SphereVao.Bind();
	glm::mat4 sphereWorld = world * glm::translate(glm::vec3(0, 4, 0));
	m_program.SetUniform("MVP", viewProj * sphereWorld);
	m_program.SetUniform("world", sphereWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(sphereWorld)));

	glDrawElements(GL_TRIANGLES, 3 * 2 * (N) * (M), GL_UNSIGNED_INT, 0);
	program.Unuse();


}

void CMyApp::DrawMesh(Mesh* mesh, Texture2D& texture, ProgramObject& program, glm::mat4 meshWorld = glm::mat4(1.0f)) {
	glm::mat4 viewProj = m_camera.GetViewProj();

	program.Use();
	m_program.SetTexture("texImage", 0, texture);
	m_program.SetUniform("MVP", viewProj * meshWorld);
	m_program.SetUniform("world", meshWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(meshWorld)));

	m_program.SetCubeTexture("texSkybox", 1, m_skyboxTexture);
	m_program.SetUniform("eye_pos", m_camera.GetEye());

	mesh->draw();
	program.Unuse();
}

void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	//Suzannec = MESH Objektum kirajzol�sa
	DrawMesh(m_suzanneMesh, m_suzanneTexture, m_program);


	m_program.Use();
	m_PlaneVao.Bind();
	glm::mat4 planeWorld = glm::translate(glm::vec3(-10, 0, 10)) * glm::scale(glm::vec3(20, 1, 20));
	m_program.SetUniform("MVP", viewProj * planeWorld);
	m_program.SetUniform("world", planeWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(planeWorld)));
	m_program.SetTexture("texImage", 0, m_grassTexture);

	glDrawElements(GL_TRIANGLES, 3 * 2 * (N) * (M), GL_UNSIGNED_INT, 0);
	

	DrawSphere(glm::translate(glm::vec3(1,1,1)), m_program);

	

	// kock�k
	m_program.Use();
	m_CubeVao.Bind();
	m_program.SetTexture("texImage", 0, m_woodTexture);
	glm::mat4 cubeWorld;

	float boxtime = time * 2 * float(M_PI) / 10;
	for (int i = 0; i < 10; ++i)
	{
		cubeWorld =
			glm::rotate(boxtime + 2 * glm::pi<float>() / 10 * i, glm::vec3(0, 1, 0))*
			glm::translate(glm::vec3(10 + 5 * sin(boxtime), 0, 0))*
			glm::rotate((i + 1)*boxtime, glm::vec3(0, 1, 0));
		m_program.SetUniform("MVP", viewProj * cubeWorld);
		m_program.SetUniform("world", cubeWorld);
		m_program.SetUniform("worldIT", glm::inverse(glm::transpose(cubeWorld)));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}
	m_program.Unuse();

	m_programAxes.Use();
	m_programAxes.SetUniform("MVP", viewProj * glm::translate(glm::vec3(1, 0, 0)));
	glDrawArrays(GL_LINES, 0, 6);

	// skybox
	// ments�k el az el�z� Z-test eredm�nyt, azaz azt a rel�ci�t, ami alapj�n update-elj�k a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenl�t haszn�ljunk, mert mindent kitolunk a t�voli v�g�s�kokra
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", viewProj * glm::translate( m_camera.GetEye()) );
	
	// cube map text�ra be�ll�t�sa 0-�s mintav�telez�re �s annak a shaderre be�ll�t�sa
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);
	// az el�z� h�rom sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	// v�g�l �ll�tsuk vissza
	glDepthFunc(prevDepthFnc);




	//ImGui Testwindow
	ImGui::ShowTestWindow();


	//My Imgui
	
	ImGui::SetNextWindowPos(ImVec2(300, 400), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin("title"))
	{
		ImGui::Text("subtitle");
		ImGui::SliderFloat("text", &(value), 1.0f, 40);

		ImGui::Checkbox("text2", &b);

		if (ImGui::Button("Btn text")) {
			//Ha megnyomj�k
			value++;
		}
	}
	ImGui::End();

}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
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
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}
