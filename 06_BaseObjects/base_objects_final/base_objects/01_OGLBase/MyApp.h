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

	//Felhaszn�l�i interakci�k
	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	ProgramObject		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader
	ProgramObject		m_programAxes;


	//Cube
	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO

	//Skybox
	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;		

	//Plane
	VertexArrayObject	m_PlaneVao;
	IndexBuffer			m_PlaneIndices;
	ArrayBuffer			m_PlaneVbo;

	//Sphere G�MB
	VertexArrayObject	m_SphereVao;
	IndexBuffer			m_SphereIndices;
	ArrayBuffer			m_SphereVbo;
	int N = 20, M = 20;

	//H�tt�r
	VertexArrayObject	m_BakcgroundVao;
	IndexBuffer			m_BackgroundIndices;
	ArrayBuffer			m_BackgroundVbo;


	//Kamera
	gCamera				m_camera;

	//Text�r�k
	Texture2D			m_woodTexture;
	Texture2D			m_suzanneTexture;
	Texture2D			m_grassTexture;

	// nyers OGL azonos�t�k
	GLuint				m_skyboxTexture;

	struct Vertex
	{
		glm::vec3 p; //pos
		glm::vec3 n; //norm
		glm::vec2 t; //texture
	};

	// mesh adatok
	Mesh *m_suzanneMesh;
	Mesh* m_cylinderMesh;

	// a jobb olvashat�s�g kedv��rt
	void InitShaders();
	void InitCube();
	void InitSkyBox();
	void InitPlane();
	void InitBackground();
	void InitSphere();

	void DrawSphere(glm::mat4& world, ProgramObject& program);
	void DrawMesh(Mesh* mesh, Texture2D& texture, ProgramObject& program, glm::mat4 meshWorld);

	float trees[20][20];


	glm::vec3 GetPosPlane(float u, float v);
	glm::vec3 GetNormPlane(float u, float v);

	glm::vec3 GetSpherePos(float u, float v);
	glm::vec3 GetSphereNorm(float u, float v);

	float time = 0;



	//Imgue v�ltoz�k
	float value = 5;
	bool b = false;
};

