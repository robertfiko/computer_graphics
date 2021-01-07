#pragma once

// C++ includes
#include <memory>
#include <random>

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
	ProgramObject		m_MeshProgram;		// mesh shader
	ProgramObject		m_SkyboxProgram;	// skybox shader
	ProgramObject		m_AxesProgram;		// axes shader
	ProgramObject		m_seaProgram;

	// kocka
	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO
	// skybox kocka
	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;


	VertexArrayObject	m_CylinderVao;
	IndexBuffer			m_CylinderIndices;
	ArrayBuffer			m_CylinderPos;


	VertexArrayObject	m_TriVao;
	IndexBuffer			m_TriIndices;

	ArrayBuffer			m_TriVbo;


	VertexArrayObject	m_seaVao;
	IndexBuffer			m_seaIndices;
	ArrayBuffer			m_seaPos;

	// gömb
	const int N = 20;
	const int M = 30;
	VertexArrayObject	m_SphereVao;
	IndexBuffer			m_SphereIndices;
	ArrayBuffer			m_SphereVertexBuffer;

	gCamera				m_camera;

	Texture2D			m_WoodTexture;
	Texture2D			m_MarronTexture;
	Texture2D			m_EarthTexture;
	Texture2D			seaText;
	Texture2D			toronyImage; //tower

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture = 0;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	struct Boat {
		int x, y;
		float r, g, b;
	};
	
	int boatNum = 5;
	Boat boats[5];

	int boatDirection = 1;
	float radius = 5;

	// mesh adatok
	Mesh *m_SuzanneMesh = nullptr;

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitCube();
	void InitSphere();
	void InitSkyBox();

	void InitCylinder();
	void InitTriangle();



	//sea
	void InitSea();
};

