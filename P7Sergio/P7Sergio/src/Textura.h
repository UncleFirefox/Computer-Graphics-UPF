#ifndef TEXTURA_H_INCLUDED
#define TEXTURA_H_INCLUDED

//includes
#ifdef _WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)


#include <iostream> //to output
#include "framework.h"
#include "camera.h"

typedef struct sTGAInfo //a general struct to store all the information about a TGA file
{
	GLuint width;
	GLuint height;
	GLuint bpp; //bits per pixel
	GLubyte* data; //bytes with the pixel information
} TGAInfo;

class Texture 
{
	public:
	GLuint texture_id;

	bool loadTexture(const char * filename);
	void enable();
	void disable();

	void DrawQuad(float offset, float size) ;
};

class Escenario : public Texture
{
	public :
	void Draw();
};

class Suelo : public Texture
{
	public :
	void Draw();
};

class Fuente : public Texture
{
	public :
	void Draw(float);
};

class Estrellas : public Texture
{
public :
	std::vector<Vector3> estrellas;
	void Draw(Camera *current_camera);
	void DrawCenter(Camera *current_camera);
	void ini(int numestrellas);
};

#endif