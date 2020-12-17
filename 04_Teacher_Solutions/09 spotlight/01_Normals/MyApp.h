#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "gCamera.h"
#include "ObjParser_OGL3.h"

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
	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// seg�df�ggv�nyek
	glm::vec3 GetPos(float u, float v);
	glm::vec3 GetNorm(float u, float v);
	glm::vec2 GetTex(float u, float v);

	// shaderekhez sz�ks�ges v�ltoz�k
	GLuint m_programID; // shaderek programja

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object er�forr�s azonos�t�
	GLuint m_vboID; // vertex buffer object er�forr�s azonos�t�
	GLuint m_ibID;  // index buffer object er�forr�s azonos�t�
	GLuint m_loadedTextureID; // bet�lt�tt text�ra er�forr�s azonos�t�

	GLuint m_planeVAO;
	GLuint m_planeVBO;
	GLuint m_planeIB;

	gCamera	m_camera;


	// uniform v�ltoz�k helye a shaderekben
	GLuint m_loc_mvp;
	GLuint m_loc_world;
	GLuint m_loc_worldIT;
	//GLuint m_loc_tex;

	GLuint m_loc_spot_pos;
	GLuint m_loc_spot_dir;

	// NxM darab n�gysz�ggel k�zel�tj�k a parametrikus fel�let�nket => (N+1)x(M+1) pontban kell ki�rt�kelni
	static const int N = 80;
	static const int M = 40;

	//A jobb olvashat�s�g kedv��rt
	void InitSphere();
	void InitShaders();
	void InitTextures();


	Mesh* m_mesh;

	glm::vec3 cam_pos;
	glm::vec3 cam_dir;
	bool move_light = true;
};

