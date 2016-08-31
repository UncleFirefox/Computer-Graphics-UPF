#ifndef MESH_H
#define MESH_H

#include <vector>
#include "framework.h"

class Mesh
{
public:
	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > colors;	 //here we store the colors of every vertex

	void clear();
	void addTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 color);
	void addQuad(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4, Vector3 color);

	void render();

	void createCube(float size);
};

#endif