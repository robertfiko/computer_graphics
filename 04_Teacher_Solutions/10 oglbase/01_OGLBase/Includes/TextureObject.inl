#include <GL\glew.h>
#include <GL\GL.h>
#include "TextureObject.h"

#include <SDL.h>
#include <SDL_image.h>

template<TextureType type>
inline TextureObject<type>::TextureObject()
{
	glGenTextures(1, &m_id);
}

template<TextureType type>
inline TextureObject<type>::TextureObject(const std::string &s)
{
	AttachFromFile(s);
}

template<TextureType type>
inline TextureObject<type>::~TextureObject()
{
	Clean();
}

template<TextureType type>
inline TextureObject<type>::TextureObject(TextureObject && rhs)
{
	if (&rhs == this)
		return;

	m_id = rhs.m_id;
	rhs.m_id = 0;
}

template<TextureType type>
inline TextureObject<type> & TextureObject<type>::operator=(TextureObject && rhs)
{
	if (&rhs == this)
		return *this;

	m_id = rhs.m_id;
	rhs.m_id = 0;

	return *this;
}

template<TextureType type>
inline TextureObject<type>& TextureObject<type>::operator=(const std::string & s)
{
	AttachFromFile(s);
	return *this;
}

inline int transform_image(int pitch, int height, void* image_pixels)
{
	int index;
	void* temp_row;
	int height_div_2;

	temp_row = (void *)malloc(pitch);
	if (NULL == temp_row)
	{
		SDL_SetError("Not enough memory for image inversion");
		return -1;
	}
	//if height is odd, don't need to swap middle row
	height_div_2 = (int)(height * .5);
	for (index = 0; index < height_div_2; index++) {
		//uses string.h
		memcpy((Uint8 *)temp_row,
			(Uint8 *)(image_pixels)+
			pitch * index,
			pitch);

		memcpy(
			(Uint8 *)(image_pixels)+
			pitch * index,
			(Uint8 *)(image_pixels)+
			pitch * (height - index - 1),
			pitch);
		memcpy(
			(Uint8 *)(image_pixels)+
			pitch * (height - index - 1),
			temp_row,
			pitch);
	}
	free(temp_row);
	return 0;
}

template<TextureType type>
inline void TextureObject<type>::AttachFromFile(const std::string& filename, bool generateMipMap, GLuint role)
{
	SDL_Surface* loaded_img = IMG_Load(filename.c_str());

	if (loaded_img == 0)
	{
		std::cerr << "[AttachFromFile] Error loading image file " << filename << std::endl;
		return;
	}

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	Uint32 format = SDL_PIXELFORMAT_ABGR8888;
#else
	Uint32 format = SDL_PIXELFORMAT_RGBA8888;
#endif

	// �talak�t�s 32bit RGBA form�tumra, ha nem abban volt
	SDL_Surface* formattedSurf = SDL_ConvertSurfaceFormat(loaded_img, format, 0);
	if (formattedSurf == nullptr)
	{
		std::cout << "[AttachFromFile] Error converting image format: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(loaded_img);
		return;
	}

	// �tt�r�s SDL koordin�tarendszerr�l ( (0,0) balfent ) OpenGL text�ra-koordin�tarendszerre ( (0,0) ballent )
	if (transform_image(formattedSurf->pitch, formattedSurf->h, formattedSurf->pixels) == -1) {
		std::cout << "[AttachFromFile] Error transforming image: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(formattedSurf);
		SDL_FreeSurface(loaded_img);
		return;
	}


	glBindTexture(static_cast<GLenum>(type), m_id);
	glTexImage2D(
		role,							// melyik binding point-on van a text�ra er�forr�s, amihez t�rol�st rendel�nk
		0,								// melyik r�szletess�gi szint adatait hat�rozzuk meg
		GL_RGBA,						// text�ra bels� t�rol�si form�tuma (GPU-n)
		formattedSurf->w,				// sz�less�g
		formattedSurf->h,				// magass�g
		0,								// nulla kell, hogy legyen ( https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml )
		GL_RGBA,						// forr�s (=CPU-n) form�tuma
		GL_UNSIGNED_BYTE,				// forr�s egy pixel�nek egy csatorn�j�t hogyan t�roljuk
		formattedSurf->pixels);			// forr�shoz pointer

	if (generateMipMap)
		glGenerateMipmap(static_cast<GLenum>(type));

	glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(loaded_img);
}

template<TextureType type>
inline void TextureObject<type>::FromFile(const std::string& s)
{
	AttachFromFile(s);
}

template<TextureType type>
inline void TextureObject<type>::Clean()
{
	if (m_id != 0)
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}
}
