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

#include "ProgramObject.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "TextureObject.h"

#include "Mesh_OGL3.h"
#include "gCamera.h"

#include <vector>

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
	ProgramObject		m_passthroughProgram;
	ProgramObject		m_axesProgram;
	ProgramObject		m_particleProgram;

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// poz�ci�k t�mbje

	gCamera				m_camera;

	int		m_particleCount = 5;

	std::vector<glm::vec3>		m_particlePos{};
	std::vector<glm::vec3>		m_particleVel{};
	std::vector<glm::vec3>		m_lines{};

	VertexArrayObject	m_gpuParticleVAO;
	ArrayBuffer			m_gpuParticleBuffer;


	VertexArrayObject	m_linesVAO;
	ArrayBuffer			m_linesVBO;
	

	float energyRemaining = 1;
	glm::vec3 gravity = glm::vec3(0, -1, 0);
};

