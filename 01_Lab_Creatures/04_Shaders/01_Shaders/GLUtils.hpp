#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

/* 

Az http://www.opengl-tutorial.org/ oldal alapján.

*/
inline GLuint loadShader(GLenum _shaderType, const char* _fileName)
{
	// shader azonosito letrehozasa
	GLuint loadedShader = glCreateShader( _shaderType );

	// ha nem sikerult hibauzenet es -1 visszaadasa
	if ( loadedShader == 0 )
	{
		std::cerr << "Hiba a " << _fileName << " shader inicializálásakor (glCreateShader)!\n";
		return 0;
	}
	
	// shaderkod betoltese _fileName fajlbol
	std::string shaderCode = "";

	// _fileName megnyitasa
	std::ifstream shaderStream(_fileName);

	if ( !shaderStream.is_open() )
	{
		std::cerr << "Hiba a " << _fileName << " shader betöltésekor (glCreateShader)!\n";
		return 0;
	}

	// file tartalmanak betoltese a shaderCode string-be
	std::string line = "";
	while ( std::getline(shaderStream, line) )
	{
		shaderCode += line + "\n";
	}

	shaderStream.close();

	// fajlbol betoltott kod hozzarendelese a shader-hez
	const char* sourcePointer = shaderCode.c_str();
	glShaderSource( loadedShader, 1, &sourcePointer, NULL );

	// shader leforditasa
	glCompileShader( loadedShader );

	// ellenorizzuk, h minden rendben van-e
	GLint result = GL_FALSE;
    int infoLogLength;

	// forditas statuszanak lekerdezese
	glGetShaderiv(loadedShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(loadedShader, GL_INFO_LOG_LENGTH, &infoLogLength);

	if ( GL_FALSE == result )
	{
		// hibauzenet elkerese es kiirasa
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(loadedShader, infoLogLength, NULL, &VertexShaderErrorMessage[0]);

		std::cerr << &VertexShaderErrorMessage[0];
	}

	return loadedShader;
}
