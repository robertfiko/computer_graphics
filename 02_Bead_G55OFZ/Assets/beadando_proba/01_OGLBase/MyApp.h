#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	// shaderekhez sz�ks�ges v�ltoz�k
	ProgramObject		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader
	ProgramObject		m_programAxes;


	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;		



	VertexArrayObject	m_SphereVao;
	IndexBuffer			m_SphereIndices;
	ArrayBuffer			m_SphereVbo;

	int N = 20, M = 20;





	gCamera				m_camera;

	Texture2D			m_woodTexture;
	Texture2D			m_suzanneTexture;
	Texture2D			m_grassTexture;

	// nyers OGL azonos�t�k
	GLuint				m_skyboxTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// mesh adatok
	Mesh *m_mesh;

	// a jobb olvashat�s�g kedv��rt
	void InitShaders();
	void InitSkyBox();
	void InitBackground();
	void InitSphere();

	void DrawSphere(glm::mat4& treeWorld);


	glm::vec3 GetSpherePos(float u, float v);
	glm::vec3 GetSphereNorm(float u, float v);

	float time = 0;
};

