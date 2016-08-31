#ifndef FRAMEWORK //macros to ensure the code is included once
#define FRAMEWORK

#ifdef _WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

#include <vector>
#include <cmath>
#include <cassert>

#define DEG2RAD 0.0174532925


//My Point class
class Vector3
{
public:
	float x,y,z;

	Vector3();
	Vector3(float x, float y, float z);

	double length();
	double length() const;

	void normalize();
	void random(float range);
	float distance(const Vector3& v);

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

		bool inverse();

		//rotate only
		Vector3 rotateVector( const Vector3& v);

		//traslate using world coordinates
		void traslate(float x, float y, float z);
		void rotate( float angle_in_rad, const Vector3& axis  );
		void traslateLocal(float x, float y, float z);
		void rotateLocal( float angle_in_rad, const Vector3& axis  );

		//create a transformation matrix from scratch
		void setTranslation(float x, float y, float z);
		void setRotation( float angle_in_rad, const Vector3& axis );

		Matrix44 operator * (const Matrix44& matrix) const;
};

class VectorPoints4x1
{
public:
	Vector3 P1;
	Vector3 P2;
	Vector3 P3;
	Vector3 P4;

	VectorPoints4x1();
	VectorPoints4x1(Vector3 &point1, Vector3 &point2, Vector3 &point3, Vector3 &point4);
};

class Vector1x4
{
public:
	float B1;
	float B2;
	float B3;
	float B4;

	Vector1x4();

	void calculateBernstein(float t);
};

//Operators, they are our friends
//Matrix44 operator * ( const Matrix44& a, const Matrix44& b );
Vector3 operator * (const Matrix44& matrix, const Vector3& v);
Vector3 operator + (const Vector3& a, const Vector3& b);
Vector3 operator - (const Vector3& a, const Vector3& b);
Vector3 operator * (const Vector3& a, float v);
Vector3 operator * (float v ,const Vector3& a);
//My own operators
Vector3 operator * (const Vector1x4& a, const VectorPoints4x1& b);

//General functions **************
long getTime();

#endif