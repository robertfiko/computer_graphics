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
}

CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

void CMyApp::InitCube()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex>vertices;
	vertices.reserve(6 * 4);
	
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

	std::vector<GLuint> indices(36);
	size_t index = 0;
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

	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_CubeVertexBuffer konstruktora már létrehozott egy GPU puffer azonosítót és a most következő BufferData hívás ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_CubeVertexBuffer típusa ArrayBuffer) és
	2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/

	m_CubeVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_CubeIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_CubeVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			//{ CreateAttribute<	0,						// attribútum: 0
			//						glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
			//						0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
			//						sizeof(Vertex)			// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
			//					>, m_CubeVertexBuffer },
			{ CreateAttribute<0, glm::vec3, 0,						 sizeof(Vertex)>, m_CubeVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)),	 sizeof(Vertex)>, m_CubeVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
		},
		m_CubeIndices
	);
}

glm::vec3 GetHalfSpherePos(float u, float v)
{
	u *= 2 * float(M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	return glm::vec3(cu * sv, fabs(cv), su * sv);
}
glm::vec3 GetHalfSphereNorm(float u, float v)
{
	u *= 2 * float(M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	return glm::vec3(cu * sv, fabs(cv), su * sv);
}
glm::vec2 GetHalfSphereTex(float u, float v)
{
	return glm::vec2(1 - u, 1 - v);
}

glm::vec3 GetCylPos(float u, float v) {
	float r = 1;
	float h = 1;
	u *= 2 * M_PI;
	v *= h;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);

	return  glm::vec3(r * cu, v, -r * su);
}
glm::vec3 GetCylNorm(float u, float v) {
	glm::vec3 du = GetCylPos(u + 0.01, v) - GetCylPos(u - 0.01, v);
	glm::vec3 dv = GetCylPos(u, v + 0.01) - GetCylPos(u, v - 0.01);

	return glm::normalize(glm::cross(du, dv));

}
glm::vec2 GetCylTex(float u, float v)
{
	return glm::vec2(1 - u, 1 - v);
}


void CMyApp::InitSphere()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex>vertices((N + 1) * (M + 1));
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vertices[i + j * (N + 1)].p = GetHalfSpherePos(u, v);
			vertices[i + j * (N + 1)].n = GetHalfSphereNorm(u, v);
			vertices[i + j * (N + 1)].t = GetHalfSphereTex(u, v);
		}

	std::vector<GLuint> indices(3 * 2 * (N) * (M));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			indices[6*i + j*3*2*(N) + 0] = (i)		+ (j)*	(N+1);
			indices[6*i + j*3*2*(N) + 1] = (i+1)	+ (j)*	(N+1);
			indices[6*i + j*3*2*(N) + 2] = (i)		+ (j+1)*(N+1);
			indices[6*i + j*3*2*(N) + 3] = (i+1)	+ (j)*	(N+1);
			indices[6*i + j*3*2*(N) + 4] = (i+1)	+ (j+1)*(N+1);
			indices[6*i + j*3*2*(N) + 5] = (i)		+ (j+1)*(N+1);
		}

	m_SphereVertexBuffer.BufferData(vertices);
	m_SphereIndices.BufferData(indices);
	m_SphereVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0,						sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<1, glm::vec3,	(sizeof(glm::vec3)),	sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)),sizeof(Vertex)>, m_SphereVertexBuffer },
		},
		m_SphereIndices
		);
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
		glm::vec3( 1, -1, -1),
		glm::vec3( 1,  1, -1),
		glm::vec3(-1,  1, -1),
		// elülső lap
		glm::vec3(-1, -1,  1),
		glm::vec3( 1, -1,  1),
		glm::vec3( 1,  1,  1),
		glm::vec3(-1,  1,  1),
	}
	);

	m_SkyboxIndices.BufferData(
		std::vector<GLuint>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülső lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felső
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
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

void CMyApp::InitShaders()
{
	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_MeshProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_MeshProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
		});

	m_MeshProgram.LinkProgram();

	// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
	m_SkyboxProgram.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
		);

	// shader program létrehozása még rövidebben:
	// { GL_VERTEX_SHADER, "shader.vert" } helyett "shader.vert"_vs
	m_AxesProgram.Init({ "axes.vert"_vs, "axes.frag"_fs });

		m_seaProgram.Init(
			{
				{ GL_VERTEX_SHADER, "sea.vert" },
				{ GL_FRAGMENT_SHADER, "sea.frag" }
			},
		{
			{ 0, "vs_in_pos" },				
			{ 1, "vs_in_norm" },			
			{ 2, "vs_in_tex" },
		}
		);
}

void CMyApp::InitSea() {
	std::vector<Vertex> vert((201) * (201));



	for (int i = 0; i <= 200; ++i)
		for (int j = 0; j <= 200; ++j)
		{
			
			float u =( i % 24 )/ 25.0f ;
			float v =( j % 24 )/ 25.0f ;

			vert[i + j * (201)].t = glm::vec2(v, u);
			vert[i + j * (201)].p = glm::vec3(j ,0, i);	
			vert[i + j * (201)].n = glm::vec3(0, 1, 0);
		}

	std::vector<int> indices(3 * 2 * (200) * (200));
	for (int i = 0; i < 200; ++i)
		for (int j = 0; j < 200; ++j)
		{
			indices[6 * i + j * 3 * 2 * (200)+0] = (i)+(j) * (200 + 1);
			indices[6 * i + j * 3 * 2 * (200)+1] = (i + 1) + (j) * (200 + 1);
			indices[6 * i + j * 3 * 2 * (200)+2] = (i)+(j + 1) * (200 + 1);
			indices[6 * i + j * 3 * 2 * (200)+3] = (i + 1) + (j) * (200 + 1);
			indices[6 * i + j * 3 * 2 * (200)+4] = (i + 1) + (j + 1) * (200 + 1);
			indices[6 * i + j * 3 * 2 * (200)+5] = (i)+(j + 1) * (200 + 1);
		}

	m_seaPos.BufferData(vert);
	m_seaIndices.BufferData(indices);
	m_seaVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_seaPos },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_seaPos },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_seaPos },
		},
		m_seaIndices
	);
}

void CMyApp::InitCylinder() {

	int N = 30;
	int M = 5;
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex>vertices((N + 1) * (M + 1));
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vertices[i + j * (N + 1)].p = GetCylPos(u, v);
			vertices[i + j * (N + 1)].n = GetCylNorm(u, v);
			vertices[i + j * (N + 1)].t = GetCylTex(u, v);
		}

	std::vector<GLuint> indices(3 * 2 * (N) * (M));
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

	m_CylinderPos.BufferData(vertices);
	m_CylinderIndices.BufferData(indices);
	m_CylinderVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0,						sizeof(Vertex)>, m_CylinderPos },
			{ CreateAttribute<1, glm::vec3,	(sizeof(glm::vec3)),	sizeof(Vertex)>, m_CylinderPos },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)),sizeof(Vertex)>, m_CylinderPos },
		},
		m_CylinderIndices
		);


}


void CMyApp::InitTriangle() {
	std::vector<Vertex> v(3);

	v[0].p = glm::vec3(1, 0, 0);
	v[1].p = glm::vec3(-1, 0, 0);
	v[2].p = glm::vec3(0, 2, 0);

	for (int i = 0; i < 3; i++) {
		v[i].n = glm::vec3(0, 0, 1);
		v[i].t = glm::vec2(0, 0);
	}


	m_TriVbo.BufferData(v);

	std::vector<int> indices(6);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 1;

	m_TriIndices.BufferData(indices);
	m_TriVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_TriVbo },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_TriVbo },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_TriVbo },
		},
		m_TriIndices
	);
}


bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.75f,0.38, 0.05, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafelé nező lapok eldobását
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	InitShaders();
	InitCube();
	InitSphere();
	InitSkyBox();
	InitSea();
	InitCylinder();
	InitTriangle();

	//Boat pos
	for (int i = 0; i < 5; i++) {
		boats[i].x = (rand() % 61 + 10) * ((rand() % 2 == 1) ? (-1) : 1);
		boats[i].y = (rand() % 61 + 10) * ((rand() % 2 == 1) ? (-1) : 1);

		boats[i].r = abs((float)(rand() % 101) / 100.0f);
		boats[i].g = abs((float)(rand() % 101) / 100.0f);
		boats[i].b = abs((float)(rand() % 101) / 100.0f);
	}

	

	// egyéb textúrák betöltése
	m_WoodTexture.FromFile("assets/wood.jpg");
	m_MarronTexture.FromFile("assets/marron.jpg");
	m_EarthTexture.FromFile("assets/earth.jpg");
	seaText.FromFile("assets/water.jpg");
	toronyImage.FromFile("assets/lighthouse.jpg");

	// mesh betöltése
	m_SuzanneMesh = ObjParser::parse("assets/Suzanne.obj");
	m_SuzanneMesh->initBuffers();
	
	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);
	m_camera.SetView(glm::vec3(40, 40, 60), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);

	delete m_SuzanneMesh;
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// töröljük a framebuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z buffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	// tengelyek
	m_AxesProgram.Use();
	m_AxesProgram.SetUniform("mvp", viewProj);
	glDrawArrays(GL_LINES, 0, 6);
	m_AxesProgram.Unuse();
	
	/*
	m_MeshProgram.Use();
	// Suzanne
	glm::mat4 world = glm::translate(glm::vec3(-3, 0, 0));
	m_MeshProgram.SetTexture("texImage", 0, m_MarronTexture);
	m_MeshProgram.SetUniform("MVP", viewProj * world);
	m_MeshProgram.SetUniform("world", world);
	m_MeshProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));*/
	//m_SuzanneMesh->draw();

	// gömb
	//float alpha = SDL_GetTicks() / 1000.0f / 2;
	//world = glm::rotate(alpha, glm::vec3(0, 1, 0));


	m_MeshProgram.Use();
	m_MeshProgram.SetUniform("color", glm::vec3(0, 0, 0));

	//sziget
	glm::mat4 world = glm::scale(glm::vec3(5,5,5));
	m_MeshProgram.SetUniform("MVP", viewProj * world);
	m_MeshProgram.SetUniform("world", world);
	m_MeshProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

	m_MeshProgram.SetUniform("color", glm::vec3(0, 0.3f, 0));
	m_SphereVao.Bind();
	glDrawElements(GL_TRIANGLES, N * M * 6, GL_UNSIGNED_INT, nullptr);
	m_SphereVao.Unbind();

	//torony
	m_MeshProgram.SetUniform("color", glm::vec3(0, 0, 0));
	world = glm::translate(glm::vec3(0, 4, 0))  * glm::scale(glm::vec3(1,8,1));
	m_MeshProgram.SetTexture("texImage", 0, toronyImage);
	m_MeshProgram.SetUniform("MVP", viewProj * world);
	m_MeshProgram.SetUniform("world", world);
	m_MeshProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

	m_CylinderVao.Bind();
	glDrawElements(GL_TRIANGLES, 5*6*30, GL_UNSIGNED_INT, nullptr);
	m_CylinderVao.Unbind();

	//kupola
	world = glm::translate(glm::vec3(0,12,0));
	m_MeshProgram.SetUniform("MVP", viewProj * world);
	m_MeshProgram.SetUniform("world", world);
	m_MeshProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

	m_MeshProgram.SetUniform("color", glm::vec3(1.0, 0, 0));
	m_SphereVao.Bind();
	glDrawElements(GL_TRIANGLES, N * M * 6, GL_UNSIGNED_INT, nullptr);
	m_SphereVao.Unbind();
	

	for (int i = 0; i < boatNum; i++) {
	
		//vitorlás
		//test
		world = glm::translate(glm::vec3(boats[i].x, 1, boats[i].y)) *
			glm::scale(glm::vec3(2, 1, 1)) *
			glm::rotate((float)M_PI, glm::vec3(1, 0, 0));
		m_MeshProgram.SetUniform("MVP", viewProj * world);
		m_MeshProgram.SetUniform("world", world);
		m_MeshProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

		m_MeshProgram.SetUniform("color", glm::vec3(0.35f, 0.2f, 0));
		m_SphereVao.Bind();
		glDrawElements(GL_TRIANGLES, N * M * 6, GL_UNSIGNED_INT, nullptr);
		m_SphereVao.Unbind();

		//arboc
		world = glm::translate(glm::vec3(boats[i].x, 0, boats[i].y)) *
			glm::scale(glm::vec3(0.05f, 4, 0.05f));
		m_MeshProgram.SetUniform("MVP", viewProj * world);
		m_MeshProgram.SetUniform("world", world);
		m_MeshProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

		m_MeshProgram.SetUniform("color", glm::vec3(0.35f, 0.2f, 0));

		m_CylinderVao.Bind();
		glDrawElements(GL_TRIANGLES, 5 * 6 * 30, GL_UNSIGNED_INT, nullptr);
		m_CylinderVao.Unbind();




		//háromszog vitorla
		world = glm::translate(glm::vec3(boats[i].x, 0, boats[i].y)) *
			glm::translate(glm::vec3(0, 2, 0));
		m_MeshProgram.SetUniform("MVP", viewProj * world);
		m_MeshProgram.SetUniform("world", world);
		m_MeshProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

		m_MeshProgram.SetUniform("color", glm::vec3(boats[i].r, boats[i].g, boats[i].b));

		m_TriVao.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		m_TriVao.Unbind();

	}
	
	//sea drawing
	m_seaProgram.Use();
	world = glm::translate(glm::vec3(-100, 0, -100));
	m_seaProgram.SetTexture("texImage", 0, seaText);
	m_seaProgram.SetUniform("MVP", viewProj * world);
	m_seaProgram.SetUniform("world", world);
	m_seaProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));
	m_seaVao.Bind();
	glDrawElements(GL_TRIANGLES, 200*200*3*2, GL_UNSIGNED_INT, nullptr);
	m_seaVao.Unbind();
	m_seaProgram.SetUniform("time", SDL_GetTicks() / 1000.0f);

	m_seaProgram.Unuse();
	

	ImGui::SetNextWindowPos(ImVec2(300, 400), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin("Beach Controller"))
	{
		ImGui::Text("Number of Boats");
		ImGui::SliderInt("Boats", &(boatNum), 1, 5);

		if (ImGui::Button("Change Boat Direction")) {
			boatDirection *= (-1);
		}

		ImGui::Text("Light House");
		ImGui::SliderFloat("Radius", &(radius), 5, 50);

		
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

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}
