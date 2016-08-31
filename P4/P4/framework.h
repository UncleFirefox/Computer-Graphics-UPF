#ifndef FRAMEWORK //macros to ensure the code is included once
#define FRAMEWORK

#ifdef WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

#include <vector>
#include <cmath>

//My Point class
class Point
{
public:
	float x,y,z;

	Point();
	Point(float x, float y, float z = 1.0f);

	Point& operator = (const Point& c) { x = c.x; y = c.y; z = c.z; return *this; }

	void Random(float range);
};

//Matrix class
class Matrix33
{
	public:
		float data[9];

		Matrix33();
		void setIdentity();

		//multiply one matrix by the other and store the result in this one
		void multiply( const Matrix33& m );

		//multiply one vector by the matrix
		void transform( Point& p );

		//assign a matrix to perform a transform
		void setTranslation(float x, float y);
		void setRotation(float angle);
		void setScale(float x, float y);
};

//General functions **************
long getTime();

#endif