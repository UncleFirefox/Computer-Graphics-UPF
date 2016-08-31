#ifndef FRAMEWORK //macros to ensure the code is included once
#define FRAMEWORK

#ifdef _WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

#include <vector>
#include <cmath>

#define DEG2RAD 0.0174532925


//My Point class
class Vector3
{
public:
	float x,y,z;

	Vector3();
	Vector3(float x, float y, float z);

	double length();
	void normalize();
	void random(float range);

	Vector3 cross( const Vector3& v );
	float dot( const Vector3& v );
};

//Matrix44 class
class Matrix44
{
	public:

		//This matrix works in 
		union { //allows to access the same var using different ways
			float M[4][4]; //[row][column]
			float m[16];
		};

		Matrix44();
		Matrix44(const float* v);

		void clear();
		void setIdentity();
		void transpose();

		//transform the coordinates system
		void traslate(float x, float y, float z);
		void rotate( float angle_in_rad, const Vector3& axis  );

		//create a transformation matrix from scratch
		void setTranslation(float x, float y, float z);
		void setRotation( float angle_in_rad, const Vector3& axis );

		Matrix44 operator * (const Matrix44& matrix) const;
};

//Operators, they are our friends
//Matrix44 operator * ( const Matrix44& a, const Matrix44& b );
Vector3 operator * (const Matrix44& matrix, const Vector3& v);
Vector3 operator + (const Vector3& a, const Vector3& b);
Vector3 operator - (const Vector3& a, const Vector3& b);
Vector3 operator * (const Vector3& a, const float &b);

//General functions **************
long getTime();

#endif