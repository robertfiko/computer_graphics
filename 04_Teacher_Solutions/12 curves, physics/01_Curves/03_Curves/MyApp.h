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
	// g�rb�nk ki�rt�kel�se
	glm::vec3 Eval(float);
	float	m_currentParam{ 0 };

	// shaderekhez sz�ks�ges v�ltoz�k
	ProgramObject		m_program;		// shaderek programja
	ProgramObject		m_axesProgram;
	ProgramObject		m_pointProgram;

	Texture2D			m_textureMetal;

	VertexArrayObject	m_vao;			// VAO objektum
	IndexBuffer			m_gpuBufferIndices;		// indexek
	ArrayBuffer			m_gpuBufferPos;	// poz�ci�k t�mbje
	ArrayBuffer			m_gpuBufferNormal;	// norm�lisok t�mbje
	ArrayBuffer			m_gpuBufferTex;	// text�rakoordin�t�k t�mbje

	std::unique_ptr<Mesh>	m_mesh;

	gCamera				m_camera;

	const int kMaxPointCount = 10;

	std::vector<glm::vec3>		m_controlPoints{ {-10,0,-10}, {10,0,-10} };
	std::vector<std::string>	m_pointName{ "Point 1", "Point 2" };

	glm::vec2 DirectionToAngle(glm::vec3 dir);
	glm::vec3 Catmull(float localT, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
};

