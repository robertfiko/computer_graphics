#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
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

	//
	// shaderek bet�lt�se
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// illessz�k �ssze a shadereket (kimen�-bemen� v�ltoz�k �sszerendel�se stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( GL_FALSE == result )
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);

		std::cerr << &VertexShaderErrorMessage[0];
	}

	// m�r nincs ezekre sz�ks�g
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	return true;
}

void CMyApp::Clean()
{
	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
}


void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
	glUseProgram( m_programID );

	glUniform2fv(glGetUniformLocation(m_programID, "move"), 1, move);
	glUniform1f(glGetUniformLocation(m_programID, "zoom"), zoom);

	// kirajzol�s: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
	glDrawArrays(	GL_TRIANGLE_STRIP,	// primit�v t�pusa; amikkel mi foglalkozunk: GL_POINTS, GL_LINE_STRIP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES
					0,					// ha van t�rol� amiben a kirajzoland� geometri�k cs�cspontjait t�roljuk, akkor annak h�nyadik cs�cspontj�t�l rajzoljunk - most nincs ilyen, 
										// csak arra haszn�ljuk, hogy a gl_VertexID sz�ml�l� a shader-ben melyik sz�mr�l induljon, azaz most null�r�l
					4);					// h�ny cs�cspontot haszn�ljunk a primit�vek kirajzol�s�ra - most: gl_VertexID sz�ml�l� 0-t�l indul �s 3-ig megy, azaz �sszesen 4x fut le a vertex shader

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_RIGHT:
		move[0] += 0.1f;
		break;
	case SDLK_LEFT:
		move[0] -= 0.1f;
		break;
	case SDLK_UP:
		move[1] += 0.1f;
		break;
	case SDLK_DOWN:
		move[1] -= 0.1f;
		break;
	case SDLK_HOME:
		zoom *= .95f;
		break;
	case SDLK_END:
		zoom /= .95f;
		break;
	}

	zoom = glm::clamp(zoom, .00005f, 10.0f);
	std::cout << move[0] << " " << move[1] << std::endl;
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

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
}
