#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <time.h> 

glm::vec3 getSphere(float u, float v) { // u 0, 1
	u *= 2 * 3 * M_PI;
	v *= M_PI;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	float r = 7;
	return glm::vec3(r * cu * sv, r * cv, r * su * sv);
}

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
	indexID = 0;
	resize = false;

	restart = 0;
	sca = acosf(1 / 3.0);
	prevSca = sca;
	scalar = 1;

	srand(time(NULL));
	for (int i = 0; i < 6; i++) {
		randArray[i] = rand();
	}
}

CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok

	// geometria letrehozasa

	Vertex vert[] =
	{
		//1. Kocka - középsõ
		//          x,  y, z             R, G, B
		{glm::vec3(0, 0, 1), glm::vec3(0, 0, 0)}, //0
		{glm::vec3(1, 0, 1), glm::vec3(1, 0, 0)}, //1
		{glm::vec3(0, 1, 1), glm::vec3(0, 1, 0)}, //2
		{glm::vec3(1, 1, 1), glm::vec3(0, 0, 1)}, //3
		{glm::vec3(0, 0, 0), glm::vec3(1, 1, 0)}, //4
		{glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)}, //5
		{glm::vec3(0, 1, 0), glm::vec3(0, 1, 1)}, //6
		{glm::vec3(1, 1, 0), glm::vec3(1, 1, 1)}, //7

		//Kieg. 2. kocka - jobb alsó
		//          x,  y, z             R, G, B
		{glm::vec3(1, -1, 1), glm::vec3(1, 1, 0)}, //8
		{glm::vec3(2, -1, 1), glm::vec3(1, 0, 1)}, //9
		{glm::vec3(2,  0, 1), glm::vec3(0, 1, 1)}, //10
		{glm::vec3(1, -1, 0), glm::vec3(1, 1, 1)}, //11
		{glm::vec3(2,  0, 0), glm::vec3(1, 1, 0)}, //12
		{glm::vec3(2, -1, 0), glm::vec3(1, 0, 1)}, //13


		//Kieg. 3. kocka - bal alsó
		//          x,  y, z             R, G, B
		{glm::vec3(-1,  0, 1), glm::vec3(1, 1, 0)}, //14
		{glm::vec3(-1,  0, 0), glm::vec3(1, 0, 1)}, //15
		{glm::vec3(-1, -1, 1), glm::vec3(0, 1, 1)}, //16
		{glm::vec3( 0, -1, 1), glm::vec3(1, 1, 1)}, //17
		{glm::vec3( 0, -1, 0), glm::vec3(1, 1, 0)}, //18
		{glm::vec3(-1, -1, 0), glm::vec3(1, 0, 1)}, //19

		//Kieg. 4. kocka - bal felül
		//          x,  y, z             R, G, B
		{glm::vec3( 0, 2, 0), glm::vec3(1, 1, 0)}, //20
		{glm::vec3(-1, 1, 0), glm::vec3(1, 0, 1)}, //21
		{glm::vec3(-1, 2, 0), glm::vec3(0, 1, 1)}, //22
		{glm::vec3( 0, 2, 1), glm::vec3(1, 1, 1)}, //23 
		{glm::vec3(-1, 1, 1), glm::vec3(1, 1, 0)}, //24
		{glm::vec3(-1, 2, 1), glm::vec3(1, 0, 1)}, //25

		//Kieg. 5. kocka - jobb felül
		//          x,  y, z             R, G, B
		{glm::vec3(2, 1, 0), glm::vec3(1, 1, 0)}, //26
		{glm::vec3(1, 2, 0), glm::vec3(1, 0, 1)}, //27
		{glm::vec3(2, 2, 0), glm::vec3(0, 1, 1)}, //28
		{glm::vec3(2, 1, 1), glm::vec3(1, 1, 1)}, //29
		{glm::vec3(2, 2, 1), glm::vec3(1, 1, 0)}, //30
		{glm::vec3(1, 2, 1), glm::vec3(1, 0, 1)}, //31 

	};

	GLushort indices[] = {
		//KÖZÉPSÕ
		0, 1, 2, 2, 1, 3, 
		1, 5, 3, 3, 5, 7, 
		5, 4, 7, 7, 4, 6, 
		4, 0, 6, 6, 0, 2, 
		4, 5, 0, 0, 5, 1, 
		2, 3, 6, 6, 3, 7, 

		//JOBB ALSÓ
		8,  9,  1,  1,  9, 10,
		9, 13, 10, 10, 13, 12,
		1, 10,  5,  5, 10, 12,
		1,  5,  8,  8,  5, 11,
		5, 12, 11, 11, 12, 13,
		8, 11,  9,  9, 11, 13,

		//BAL ALSÓ
		16, 17, 14, 14, 17, 0, //elõlap
		17, 18, 0, 0, 18, 4, //jobb
		18, 19, 4, 4, 19, 15, //hátlap ??
		19, 16, 15, 15, 16, 14, //bal
		19, 18, 16, 16, 18, 17, //alsó
		14, 0, 15, 15, 0, 4, //felsõ

		//BAL FELSÕ
		24,  2, 25, 25,  2, 23, //elsõ
		2,  6, 23, 23, 6, 20, //jobb
		6, 21, 20, 20, 21, 22, //hátsó
		21, 24, 22, 22, 24, 25, //bal
		21, 6, 24, 24, 6, 2, //alsó
		25, 23, 22, 22, 23, 20, //felsõ

		//JOBB FELSÕ
		3, 29, 31, 31, 29, 30,
		29, 26, 30, 30, 26, 28,
		26, 7, 28, 28, 7, 27, //hátsó
		7, 3, 27, 27, 3, 31, //bal
		31, 30, 27, 27, 30, 28, //felsõ
		7, 26, 3, 3, 26, 29 //alsó



	};

	
	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); 
	glBufferData( GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	

	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);


	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); //  szín
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);						// feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// feltöltöttük a VBO-t is, ezt is vegyük le


	// shaderek betöltése
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	glBindAttribLocation( m_programID, 0, "vs_in_pos");
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( GL_FALSE == result )
	{
		std::vector<char> ProgramErrorMessage( infoLogLength );
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		
		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy( aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] Hiba a kovetkezo:: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// shader delete
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	// egyéb inicializálás
	// vetítési mátrix létrehozása
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_world = glGetUniformLocation( m_programID, "world");
	m_loc_view  = glGetUniformLocation( m_programID, "view" );
	m_loc_proj  = glGetUniformLocation( m_programID, "proj" );

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &indexID);
	glDeleteVertexArrays(1, &m_vaoID); 
	glDeleteProgram( m_programID );
	
}

void CMyApp::Update()
{
	m_matView = glm::lookAt(glm::vec3(0,20, 30),		// honnan nézzük a színteret
							glm::vec3(0, 0, 0),		// a színtér melyik pontját nézzük
							glm::vec3(0, 1, 0));	// felfelé mutató irány a világban

}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
	glUseProgram( m_programID );

	// shader parameterek beállítása
	glUniformMatrix4fv(m_loc_view, 1, GL_FALSE, &(m_matView[0][0]));
	glUniformMatrix4fv(m_loc_proj, 1, GL_FALSE, &(m_matProj[0][0]));

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);

	float time = SDL_GetTicks() / 1000.0;
	m_matWorld = glm::mat4(1.0f);

	if (resize) {
		float diff = (SDL_GetTicks() / 1000.0) - restart;
		float localTime = ((fabs(diff) < 0.00001) ? 0 : diff);

		scalar = fabs(cosf(sca)) * 1.5 + 0.5;
		sca = prevSca + localTime;
	}

	
	for (int i = 0; i < 6; i++) {

		//egység négyzet ellenõrzéséhez skalár kiiratás
		//std::cout << "Scalar: " << scalar << std::endl;
		m_matWorld =

			//glm::rotate<float>(M_PI * time * 2 / 5, glm::vec3(1, 0, 0))* //Alsó forgatás, az összes lap ellenõrzéséhez
			glm::rotate<float>(time * 2 * M_PI / 5, glm::vec3(0, 1, 0)) *						//Forgatok
			glm::translate<float>(glm::vec3(9,0,0)) *											//Eltolom 
			glm::rotate<float>(time * 2 * M_PI / 5, glm::vec3(0, -1, 0))*						//Ellenforgatok

			glm::translate<float>(getSphere(fabs(sin(randArray[i])), fabs(sin(randArray[i])))) * //gomb felszínen elhelyzés
			glm::scale<float>(glm::vec3(scalar, 1, 1)) *										//SPACE hatására skálázás
			glm::translate<float>(glm::vec3(-0.5f, -0.5f, -0.5f)) ;								//origoba tol

		glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
		glDrawElements(GL_TRIANGLES, 180, GL_UNSIGNED_SHORT, 0);

	};

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_SPACE) {
		if (!resize) {
			restart = SDL_GetTicks() / 1000.0;
		}
		resize = true;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_SPACE) {
		resize = false;
		prevSca = sca;
	}
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse) {}
void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse) {}
void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse) {}
void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel) {}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,			// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f);		// tavoli vagosik
}