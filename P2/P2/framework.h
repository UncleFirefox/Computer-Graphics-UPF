#ifndef FRAMEWORK //macros to ensure the code is included once
#define FRAMEWORK

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

	std::vector<Point> CalculePointsUsingBresenham();
	std::vector<Point> CalculePointsUsingDDA();
};

//Circle class
//...

class Circle
{
public:
	Point center;
	float radius;
	Circle(Point a, Point b);
	std::vector<Point> CalculePointsUsingBresenham();
};

//General functions **************
long getTime();

#endif