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
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)
	glCullFace(GL_BACK); // GL_BACK: a kamer�t�l "elfel�" n�z� lapok, GL_FRONT: a kamera fel� n�z� lapok

	// geometria letrehozasa

	Vertex vert[] =
	{
		//1. Kocka - k�z�ps�
		//          x,  y, z             R, G, B
		{glm::vec3(0, 0, 1), glm::vec3(0, 0, 0)}, //0
		{glm::vec3(1, 0, 1), glm::vec3(1, 0, 0)}, //1
		{glm::vec3(0, 1, 1), glm::vec3(0, 1, 0)}, //2
		{glm::vec3(1, 1, 1), glm::vec3(0, 0, 1)}, //3
		{glm::vec3(0, 0, 0), glm::vec3(1, 1, 0)}, //4
		{glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)}, //5
		{glm::vec3(0, 1, 0), glm::vec3(0, 1, 1)}, //6
		{glm::vec3(1, 1, 0), glm::vec3(1, 1, 1)}, //7

		//Kieg. 2. kocka - jobb als�
		//          x,  y, z             R, G, B
		{glm::vec3(1, -1, 1), glm::vec3(1, 1, 0)}, //8
		{glm::vec3(2, -1, 1), glm::vec3(1, 0, 1)}, //9
		{glm::vec3(2,  0, 1), glm::vec3(0, 1, 1)}, //10
		{glm::vec3(1, -1, 0), glm::vec3(1, 1, 1)}, //11
		{glm::vec3(2,  0, 0), glm::vec3(1, 1, 0)}, //12
		{glm::vec3(2, -1, 0), glm::vec3(1, 0, 1)}, //13


		//Kieg. 3. kocka - bal als�
		//          x,  y, z             R, G, B
		{glm::vec3(-1,  0, 1), glm::vec3(1, 1, 0)}, //14
		{glm::vec3(-1,  0, 0), glm::vec3(1, 0, 1)}, //15
		{glm::vec3(-1, -1, 1), glm::vec3(0, 1, 1)}, //16
		{glm::vec3( 0, -1, 1), glm::vec3(1, 1, 1)}, //17
		{glm::vec3( 0, -1, 0), glm::vec3(1, 1, 0)}, //18
		{glm::vec3(-1, -1, 0), glm::vec3(1, 0, 1)}, //19

		//Kieg. 4. kocka - bal fel�l
		//          x,  y, z             R, G, B
		{glm::vec3( 0, 2, 0), glm::vec3(1, 1, 0)}, //20
		{glm::vec3(-1, 1, 0), glm::vec3(1, 0, 1)}, //21
		{glm::vec3(-1, 2, 0), glm::vec3(0, 1, 1)}, //22
		{glm::vec3( 0, 2, 1), glm::vec3(1, 1, 1)}, //23 
		{glm::vec3(-1, 1, 1), glm::vec3(1, 1, 0)}, //24
		{glm::vec3(-1, 2, 1), glm::vec3(1, 0, 1)}, //25

		//Kieg. 5. kocka - jobb fel�l
		//          x,  y, z             R, G, B
		{glm::vec3(2, 1, 0), glm::vec3(1, 1, 0)}, //26
		{glm::vec3(1, 2, 0), glm::vec3(1, 0, 1)}, //27
		{glm::vec3(2, 2, 0), glm::vec3(0, 1, 1)}, //28
		{glm::vec3(2, 1, 1), glm::vec3(1, 1, 1)}, //29
		{glm::vec3(2, 2, 1), glm::vec3(1, 1, 0)}, //30
		{glm::vec3(1, 2, 1), glm::vec3(1, 0, 1)}, //31 

	};

	GLushort indices[] = {
		//K�Z�PS�
		0, 1, 2, 2, 1, 3, 
		1, 5, 3, 3, 5, 7, 
		5, 4, 7, 7, 4, 6, 
		4, 0, 6, 6, 0, 2, 
		4, 5, 0, 0, 5, 1, 
		2, 3, 6, 6, 3, 7, 

		//JOBB ALS�
		8,  9,  1,  1,  9, 10,
		9, 13, 10, 10, 13, 12,
		1, 10,  5,  5, 10, 12,
		1,  5,  8,  8,  5, 11,
		5, 12, 11, 11, 12, 13,
		8, 11,  9,  9, 11, 13,

		//BAL ALS�
		16, 17, 14, 14, 17, 0, //el�lap
		17, 18, 0, 0, 18, 4, //jobb
		18, 19, 4, 4, 19, 15, //h�tlap ??
		19, 16, 15, 15, 16, 14, //bal
		19, 18, 16, 16, 18, 17, //als�
		14, 0, 15, 15, 0, 4, //fels�

		//BAL FELS�
		24,  2, 25, 25,  2, 23, //els�
		2,  6, 23, 23, 6, 20, //jobb
		6, 21, 20, 20, 21, 22, //h�ts�
		21, 24, 22, 22, 24, 25, //bal
		21, 6, 24, 24, 6, 2, //als�
		25, 23, 22, 22, 23, 20, //fels�

		//JOBB FELS�
		3, 29, 31, 31, 29, 30,
		29, 26, 30, 30, 26, 28,
		26, 7, 28, 28, 7, 27, //h�ts�
		7, 3, 27, 27, 3, 31, //bal
		31, 30, 27, 27, 30, 28, //fels�
		7, 26, 3, 3, 26, 29 //als�



	};

	
	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);
	
	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); 
	glBufferData( GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	

	// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);


	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(1); //  sz�n
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);						// felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// felt�lt�tt�k a VBO-t is, ezt is vegy�k le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// felt�lt�tt�k a VBO-t is, ezt is vegy�k le


	// shaderek bet�lt�se
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attrib�tumok hozz�rendel�se a shader v�ltoz�khoz
	glBindAttribLocation( m_programID, 0, "vs_in_pos");
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illessz�k �ssze a shadereket
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

	// egy�b inicializ�l�s
	// vet�t�si m�trix l�trehoz�sa
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzform�ci�s m�trixok c�m�nek lek�rdez�se
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
	m_matView = glm::lookAt(glm::vec3(0,20, 30),		// honnan n�zz�k a sz�nteret
							glm::vec3(0, 0, 0),		// a sz�nt�r melyik pontj�t n�zz�k
							glm::vec3(0, 1, 0));	// felfel� mutat� ir�ny a vil�gban

}

void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
	glUseProgram( m_programID );

	// shader parameterek be�ll�t�sa
	glUniformMatrix4fv(m_loc_view, 1, GL_FALSE, &(m_matView[0][0]));
	glUniformMatrix4fv(m_loc_proj, 1, GL_FALSE, &(m_matProj[0][0]));

	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
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

		//egys�g n�gyzet ellen�rz�s�hez skal�r kiirat�s
		//std::cout << "Scalar: " << scalar << std::endl;
		m_matWorld =

			//glm::rotate<float>(M_PI * time * 2 / 5, glm::vec3(1, 0, 0))* //Als� forgat�s, az �sszes lap ellen�rz�s�hez
			glm::rotate<float>(time * 2 * M_PI / 5, glm::vec3(0, 1, 0)) *						//Forgatok
			glm::translate<float>(glm::vec3(9,0,0)) *											//Eltolom 
			glm::rotate<float>(time * 2 * M_PI / 5, glm::vec3(0, -1, 0))*						//Ellenforgatok

			glm::translate<float>(getSphere(fabs(sin(randArray[i])), fabs(sin(randArray[i])))) * //gomb felsz�nen elhelyz�s
			glm::scale<float>(glm::vec3(scalar, 1, 1)) *										//SPACE hat�s�ra sk�l�z�s
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

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,			// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f);		// tavoli vagosik
}