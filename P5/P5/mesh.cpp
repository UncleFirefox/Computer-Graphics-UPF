#include "mesh.h"

#ifdef _WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)


void Mesh::clear()
{
	vertices.clear();
	colors.clear();
}

void Mesh::addTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 color)
{
	vertices.push_back(v1);
	colors.push_back( color );
	vertices.push_back(v2);
	colors.push_back( color );
	vertices.push_back(v3);
	colors.push_back( color );
}

void Mesh::addQuad(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4, Vector3 color)
{
	vertices.push_back(v1);
	colors.push_back( color );
	vertices.push_back(v2);
	colors.push_back( color );
	vertices.push_back(v3);
	colors.push_back( color );

	vertices.push_back(v2);
	colors.push_back( color );
	vertices.push_back(v3);
	colors.push_back( color );
	vertices.push_back(v4);
	colors.push_back( color );
}

void Mesh::createCube(float size)
{
	float hs = size * 0.5;
	Vector3 vs[8];

	Vector3 red(1,0,0);
	Vector3 green(0,1,0);
	Vector3 blue(0,0,1);

	//vertices of a cube
	vs[0] = Vector3(hs,hs,hs);
	vs[1] = Vector3(hs,hs,-hs);
	vs[2] = Vector3(hs,-hs,hs);
	vs[3] = Vector3(hs,-hs,-hs);
	vs[4] = Vector3(-hs,hs,hs);
	vs[5] = Vector3(-hs,hs,-hs);
	vs[6] = Vector3(-hs,-hs,hs);
	vs[7] = Vector3(-hs,-hs,-hs);

	//coplanares: 0123-4567 0145-2367 0246-1357

	//create the triangles
	addQuad( vs[0], vs[1], vs[2], vs[3], red );
	addQuad( vs[4], vs[5], vs[6], vs[7], red );

	addQuad( vs[0], vs[1], vs[4], vs[5], green );
	addQuad( vs[2], vs[3], vs[6], vs[7], green );

	addQuad( vs[0], vs[2], vs[4], vs[6], blue );
	addQuad( vs[1], vs[3], vs[5], vs[7], blue );
}

void Mesh::createPiramide(float size)
{
	float hs = size * 0.5;
	Vector3 vs[5];

	Vector3 red(1,0,0);
	Vector3 green(0,1,0);
	Vector3 blue(0,0,1);
	Vector3 otro(0,1,1);
	Vector3 gris(0.2f,0.2f,0.2f);

	//vertices of a piramide
	vs[0] = Vector3(hs,0,hs);
	vs[1] = Vector3(-hs,0,hs);
	vs[2] = Vector3(-hs,0,-hs);
	vs[3] = Vector3(hs,0,-hs);
	vs[4] = Vector3(0,hs,0);

	//create the triangles
	addTriangle( vs[0], vs[1], vs[4], red );
	addTriangle( vs[1], vs[2], vs[4], blue );
	addTriangle( vs[2], vs[3], vs[4], green );
	addTriangle( vs[3], vs[4], vs[0], otro );

	//The base is obviously squared
	addQuad( vs[0], vs[1], vs[3], vs[2], gris );
}

void Mesh::createOctaedro(float size)
{
	float hs = size * 0.5;
	Vector3 vs[6];

	Vector3 red(1,0,0);
	Vector3 green(0,1,0);
	Vector3 blue(0,0,1);
	Vector3 otro(0,1,1);
	Vector3 gris(0.2f,0.2f,0.2f);

	//vertices of a piramide
	vs[0] = Vector3(hs,0,hs);
	vs[1] = Vector3(-hs,0,hs);
	vs[2] = Vector3(-hs,0,-hs);
	vs[3] = Vector3(hs,0,-hs);
	vs[4] = Vector3(0,2*hs,0);
	vs[5] = Vector3(0,-2*hs,0);

	//create the triangles
	addTriangle( vs[0], vs[1], vs[4], red );
	addTriangle( vs[1], vs[2], vs[4], blue );
	addTriangle( vs[2], vs[3], vs[4], otro );
	addTriangle( vs[3], vs[4], vs[0], green );

	//Cara de abajo
	addTriangle( vs[0], vs[1], vs[5], blue );
	addTriangle( vs[1], vs[2], vs[5], green );
	addTriangle( vs[2], vs[3], vs[5], red );
	addTriangle( vs[3], vs[5], vs[0], otro );
}

void Mesh::createTriforce(float size)
{
	float hs = size * 0.5;
	Vector3 vs[6];

	Vector3 gold(252.0f/255.0f, 194.0f/255.0f, 0.0f/255.0f);

	//Triangulo superior
	//vertices of triforce

	//cara delantera
	vs[0] = Vector3(hs,hs,-5);
	vs[1] = Vector3(-hs,hs,-5);
	vs[2] = Vector3(0,2*hs,-5);

	//Cara Trasera
	vs[3] = Vector3(hs,hs,5);
	vs[4] = Vector3(-hs,hs,5);
	vs[5] = Vector3(0,2*hs,5);

	//create the triangles
	addTriangle( vs[0], vs[1], vs[2], gold );
	addTriangle( vs[3], vs[4], vs[5], gold );
	
	//Link izquierdo
	addQuad( vs[2], vs[5], vs[1], vs[4], gold );

	//Link derecho
	addQuad( vs[5], vs[2], vs[3], vs[0], gold );

	//Link inferior
	addQuad( vs[0], vs[1], vs[3], vs[4], gold );

	//Triangulo izquierdo
	//vertices of triforce

	//cara delantera
	vs[0] = Vector3(0,0,-5);
	vs[1] = Vector3(2.0f * -hs,0,-5);
	vs[2] = Vector3(-hs,hs,-5);

	//Cara Trasera
	vs[3] = Vector3(0,0,5);
	vs[4] = Vector3(2.0f*-hs,0,5);
	vs[5] = Vector3(-hs,hs,5);

	//create the triangles
	addTriangle( vs[0], vs[1], vs[2], gold );
	addTriangle( vs[3], vs[4], vs[5], gold );
	
	//Link izquierdo
	addQuad( vs[2], vs[5], vs[1], vs[4], gold );

	//Link derecho
	addQuad( vs[5], vs[2], vs[3], vs[0], gold );

	//Link inferior
	addQuad( vs[0], vs[1], vs[3], vs[4], gold );

	//Triangulo derecho
	//vertices of triforce

	//cara delantera
	vs[0] = Vector3(0,0,-5);
	vs[1] = Vector3(2.0f * hs,0,-5);
	vs[2] = Vector3(hs,hs,-5);

	//Cara Trasera
	vs[3] = Vector3(0,0,5);
	vs[4] = Vector3(2.0f*hs,0,5);
	vs[5] = Vector3(hs,hs,5);

	//create the triangles
	addTriangle( vs[0], vs[1], vs[2], gold );
	addTriangle( vs[3], vs[4], vs[5], gold );
	
	//Link izquierdo
	addQuad( vs[2], vs[5], vs[1], vs[4], gold );

	//Link derecho
	addQuad( vs[5], vs[2], vs[3], vs[0], gold );

	//Link inferior
	addQuad( vs[0], vs[1], vs[3], vs[4], gold );
}

void Mesh::createPlatform(float size)
{
	float hs = size * 0.5;
	Vector3 vs[8];

	Vector3 red(1,0,0);
	Vector3 green(0,1,0);
	Vector3 blue(0,0,1);

	//vertices of a cube
	vs[0] = Vector3(hs,hs/20,hs);
	vs[1] = Vector3(hs,hs/20,-hs);
	vs[2] = Vector3(hs,-hs/20,hs);
	vs[3] = Vector3(hs,-hs/20,-hs);
	vs[4] = Vector3(-hs,hs/20,hs);
	vs[5] = Vector3(-hs,hs/20,-hs);
	vs[6] = Vector3(-hs,-hs/20,hs);
	vs[7] = Vector3(-hs,-hs/20,-hs);

	//coplanares: 0123-4567 0145-2367 0246-1357

	//create the triangles
	addQuad( vs[0], vs[1], vs[2], vs[3], red );
	addQuad( vs[4], vs[5], vs[6], vs[7], red );

	addQuad( vs[0], vs[1], vs[4], vs[5], green );
	addQuad( vs[2], vs[3], vs[6], vs[7], green );

	addQuad( vs[0], vs[2], vs[4], vs[6], blue );
	addQuad( vs[1], vs[3], vs[5], vs[7], blue );
}


void Mesh::render()
{
	glBegin( GL_TRIANGLES );
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		glColor3fv( (GLfloat*)&colors[i] );
		glVertex3fv( (GLfloat*)&vertices[i] );
	}
	glEnd();
}

void Mesh::renderTriforce()
{
	glBegin( GL_TRIANGLES );
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		glColor3fv( (GLfloat*)&colors[i] );
		glVertex3fv( (GLfloat*)&vertices[i] );
	}
	glEnd();

	glLineWidth(2.0f);
	glEnable(GL_LINE_SMOOTH);

	//Lineas del triangulo superior
	glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3fv( (GLfloat*)&vertices[2] );
		glVertex3fv( (GLfloat*)&vertices[0] );
		glVertex3fv( (GLfloat*)&vertices[1] );
		glVertex3fv( (GLfloat*)&vertices[2] );
		glVertex3fv( (GLfloat*)&vertices[5] );
		glVertex3fv( (GLfloat*)&vertices[4] );
		glVertex3fv( (GLfloat*)&vertices[3] );
		glVertex3fv( (GLfloat*)&vertices[5] );
		glVertex3fv( (GLfloat*)&vertices[3] );
		glVertex3fv( (GLfloat*)&vertices[0] );
		glVertex3fv( (GLfloat*)&vertices[2] );
		glVertex3fv( (GLfloat*)&vertices[5] );
		glVertex3fv( (GLfloat*)&vertices[3] );
		glVertex3fv( (GLfloat*)&vertices[4] );
		glVertex3fv( (GLfloat*)&vertices[1] );
	glEnd();

	//Lineas del triangulo izquierdo
	glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3fv( (GLfloat*)&vertices[2+24] );
		glVertex3fv( (GLfloat*)&vertices[0+24] );
		glVertex3fv( (GLfloat*)&vertices[1+24] );
		glVertex3fv( (GLfloat*)&vertices[2+24] );
		glVertex3fv( (GLfloat*)&vertices[5+24] );
		glVertex3fv( (GLfloat*)&vertices[4+24] );
		glVertex3fv( (GLfloat*)&vertices[3+24] );
		glVertex3fv( (GLfloat*)&vertices[5+24] );
		glVertex3fv( (GLfloat*)&vertices[3+24] );
		glVertex3fv( (GLfloat*)&vertices[0+24] );
		glVertex3fv( (GLfloat*)&vertices[2+24] );
		glVertex3fv( (GLfloat*)&vertices[5+24] );
		glVertex3fv( (GLfloat*)&vertices[3+24] );
		glVertex3fv( (GLfloat*)&vertices[4+24] );
		glVertex3fv( (GLfloat*)&vertices[1+24] );
	glEnd();

	Vector3 v5 = Vector3(vertices[26].x+0.2f,vertices[26].y,vertices[26].z);
	Vector3 v6 = Vector3(vertices[29].x+0.2f,vertices[29].y,vertices[29].z);

	glBegin(GL_LINES);
		glVertex3fv( (GLfloat*)&v5 );
		glVertex3fv( (GLfloat*)&v6 );
	glEnd();

	//Lineas del triangulo derecho
	glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3fv( (GLfloat*)&vertices[2+48] );
		glVertex3fv( (GLfloat*)&vertices[0+48] );
		glVertex3fv( (GLfloat*)&vertices[1+48] );
		glVertex3fv( (GLfloat*)&vertices[2+48] );
		glVertex3fv( (GLfloat*)&vertices[5+48] );
		glVertex3fv( (GLfloat*)&vertices[4+48] );
		glVertex3fv( (GLfloat*)&vertices[3+48] );
		glVertex3fv( (GLfloat*)&vertices[5+48] );
		glVertex3fv( (GLfloat*)&vertices[3+48] );
		glVertex3fv( (GLfloat*)&vertices[0+48] );
		glVertex3fv( (GLfloat*)&vertices[2+48] );
		glVertex3fv( (GLfloat*)&vertices[5+48] );
		glVertex3fv( (GLfloat*)&vertices[3+48] );
		glVertex3fv( (GLfloat*)&vertices[4+48] );
		glVertex3fv( (GLfloat*)&vertices[1+48] );
	glEnd();

	//Sospechosamente, creo que debido al zbuffer la junta entre los 2 triangulos inferiores no se ve
	//Haremos una trampilla para que se vea xDDD
	Vector3 v1 = Vector3(vertices[48].x,vertices[48].y+0.1f,vertices[48].z);
	Vector3 v2 = Vector3(vertices[51].x,vertices[51].y+0.1f,vertices[51].z);

	Vector3 v3 = Vector3(vertices[50].x-0.2f,vertices[50].y,vertices[50].z);
	Vector3 v4 = Vector3(vertices[53].x-0.2f,vertices[53].y,vertices[53].z);

	glBegin(GL_LINES);
		glVertex3fv( (GLfloat*)&v1 );
		glVertex3fv( (GLfloat*)&v2 );
		glVertex3fv( (GLfloat*)&v3 );
		glVertex3fv( (GLfloat*)&v4 );
	glEnd();

}
