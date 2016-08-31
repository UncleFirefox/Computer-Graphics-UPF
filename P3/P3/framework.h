#ifndef FRAMEWORK //macros to ensure the code is included once
#define FRAMEWORK

#ifdef WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

#include <vector>

//My Point class
class Point
{
public:
	float x,y,z;

	Point();
	Point(float x, float y, float z);

	void Random(float range);
};

//Line class
class Line
{
public:
	Point start;
	Point end;

	Line() {}
	Line(Point a, Point b);
	float Lenght();
	void draw();

};


//********************
//enumeration
enum {TOP = 0x8, BOTTOM = 0x4, RIGHT = 0x2, LEFT = 0x1};
//code typedef
typedef unsigned int code;

class ClippingRectangle 
{
public:
	float xmin, ymin, xmax, ymax;

	ClippingRectangle();
	ClippingRectangle(float xmin, float ymin, float xmax, float ymax);
	void set(const Point& a, const Point& b);
	void draw();

	bool clipLine( Line& line );
	code computeCode(float x, float y);
};


//General functions **************
long getTime();

#endif