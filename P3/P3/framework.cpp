#include "framework.h"

#define WIN32

#include <cmath> //for sqrt (square root) function

//**************************************
#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

long getTime()
{
	#ifdef WIN32
		return GetTickCount();
	#else
		struct timeval tv;
		gettimeofday(&tv,NULL);
		return (int)(tv.tv_sec*1000 + (tv.tv_usec / 1000));
	#endif
}

// **************************************

Point::Point()
{
	x = y = z = 0.0f;
}

Point::Point(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point::Random(float range)
{
	//rand returns a value between 0 and RAND_MAX
	x = (rand() / (double)RAND_MAX) * 2 * range - range; //value between -range and range
	y = (rand() / (double)RAND_MAX) * 2 * range - range; //value between -range and range
	z = (rand() / (double)RAND_MAX) * 2 * range - range; //value between -range and range
}

//***********************************

Line::Line(Point a, Point b)
{
	start = a;
	end = b;
}

float Line::Lenght()
{
	//pitagoras
	return sqrt( (end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y) );
}

void Line::draw()
{
	glBegin(GL_LINES);
		glVertex2f(start.x, start.y);
		glVertex2f(end.x, end.y);
	glEnd();
}

//*********************************
ClippingRectangle::ClippingRectangle() { xmin = ymin = xmax = ymax = 0.0f; }

ClippingRectangle::ClippingRectangle(float xmin, float ymin, float xmax, float ymax)
{
	this->xmin = xmin;
	this->ymin = ymin;
	this->xmax = xmax;
	this->ymax = ymax;
}

void ClippingRectangle::set(const Point& a, const Point& b)
{
	if (a.y < b.y)
	{
		ymin = a.y;
		ymax = b.y;
	}
	else
	{
		ymin = b.y;
		ymax = a.y;
	}

	if (a.x < b.x)
	{
		xmin = a.x;
		xmax = b.x;
	}
	else
	{
		xmin = b.x;
		xmax = a.x;
	}
}

void ClippingRectangle::draw()
{
	glBegin(GL_LINE_LOOP);
		glVertex2f(xmin, ymin);
		glVertex2f(xmin, ymax);
		glVertex2f(xmax, ymax);
		glVertex2f(xmax, ymin);
	glEnd();	
}

//Calculo de la ecuacion de l recta dada en funcion de X o de Y
float fdeX (float x0, float x1, float y0, float y1, float x)
{
	return (((y1-y0)/(x1-x0))*(x-x0)) + y0;
}

float fdeY (float x0, float x1, float y0, float y1, float y)
{
	return (((y-y0)*(x1-x0))/(y1-y0)) + x0;
}

//this function changes the line passed as parameter to match the clipping plane
//it returns false if the line is totally outside
bool ClippingRectangle::clipLine( Line& line )
{
	//Clip the line to be inside the rectangle
	//TO DO...

	code codeStart = this->computeCode(line.start.x,line.start.y);
	code codeEnd = this->computeCode(line.end.x,line.end.y);

	//Primero descartamos los casos triviales
	if((codeStart & codeEnd) != 0)
	{
		return false;
	}
	else if ((codeStart | codeEnd) == 0)
	{
		return true;
	}
	else
	{
		//Precalculamos los puntos limite
		float yLeft = fdeX(line.start.x,line.end.x,line.start.y,line.end.y,this->xmin);
		float yRight = fdeX(line.start.x,line.end.x,line.start.y,line.end.y,this->xmax);
		float xDown = fdeY(line.start.x,line.end.x,line.start.y,line.end.y,this->ymin);
		float xUp = fdeY(line.start.x,line.end.x,line.start.y,line.end.y,this->ymax);
		
		//Procesamos primero las alturas y luego si el punto aun no esta dentro, procesamos los laterales
		if (codeStart & TOP)
		{
			line.start.y = this->ymax;
			line.start.x = xUp;
			codeStart = this->computeCode(line.start.x, line.start.y);
		}
		else if (codeStart & BOTTOM)
		{
			line.start.y = this->ymin;
			line.start.x = xDown;
			codeStart = this->computeCode(line.start.x, line.start.y); 
		}

		if (codeStart & LEFT)
		{
			line.start.x = this->xmin;
			line.start.y = yLeft;
		}
		else if (codeStart & RIGHT)
		{
			line.start.x = this->xmax;
			line.start.y = yRight;
		}
		
		//Lo mismo con el punto final
		if (codeEnd & TOP)
		{
			line.end.y = this->ymax;
			line.end.x = xUp;
			codeEnd = this->computeCode(line.end.x, line.end.y);
		}
		else if (codeEnd & BOTTOM)
		{
			line.end.y = this->ymin;
			line.end.x = xDown;
			codeEnd = this->computeCode(line.end.x, line.end.y);
		}

		if (codeEnd & LEFT)
		{
			line.end.x = this->xmin;
			line.end.y = yLeft;
		}
		else if (codeEnd & RIGHT)
		{
			line.end.x = this->xmax;
			line.end.y = yRight;
		}

	}

	return true;
}

//this function returns the bit code of the point according to the clipping rectangle
code ClippingRectangle::computeCode( float x, float y )
{
	//TO DO...

	code codigo = 0;

	//TOP = 0x8, BOTTOM = 0x4, RIGHT = 0x2, LEFT = 0x1
	if (x < this->xmin)
	{
		codigo |= LEFT;
	}
	else if (x > xmax)
	{
		codigo |= RIGHT;
	}
	
	if (y < this->ymin)
	{
		codigo |= BOTTOM;
	}
	else if (y > this->ymax)
	{
		codigo |= TOP;
	}

	return codigo;
}