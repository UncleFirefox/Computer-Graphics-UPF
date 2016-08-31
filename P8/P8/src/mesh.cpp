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
	Vector3 normal = (v2 - v1).cross(v3 - v1);
	normal.normalize();

	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	colors.push_back( color );
	colors.push_back( color );
	colors.push_back( color );

	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);
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

void Mesh::createSphere(float size, int arcs, int rads)
{

}

void Mesh::render()
{
	glBegin( GL_TRIANGLES );
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		glColor3fv( (GLfloat*)&colors[i] );
		if (normals.size()) glNormal3fv( (GLfloat*)&normals[i] );
		glVertex3fv( (GLfloat*)&vertices[i] );
	}
	glEnd();
}
