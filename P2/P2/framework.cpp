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

float sgn(int a)
{
	if (a > 0)
	{
		return 1.0f;
	}
	else if (a == 0)
	{
		return 0.0f;
	}

	return -1.0f;
}

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

//Fill this function
std::vector<Point> Line::CalculePointsUsingBresenham()
{
	std::vector<Point> result;

	//add the first point
	result.push_back( start );

	//Bresenham algorythm
	//...

	int dx = end.x - start.x;
	int dy = end.y - start.y;

	int incx, incy, decision;
	Point current_point = start;

	//Determinamos la funcion de decision incial;
	//decision = 2*dy-dx;

	//En funcion del signo de dx / dy sabremos si tendremos que ir incrmentando o decrementando los ejes en cada iteracion
	if (dx < 0)
	{
		incx = -1;
		//Cambiamos de signo para trasladar al caso base
		dx = -dx;
	}
	else
	{
		incx = 1;
	}

	if (dy < 0)
	{
		incy = -1;
		//Cambiamos de signo para trasladar al caso base
		dy = -dy;
	}
	else
	{
		incy = 1;
	}

	//Determinamos el eje con mas recorrido
	if (dx >= dy)
	{
		decision = 2*dy-dx;
		//for incrementando eje x
		for (int i = 0; i < dx; i++)
		{
			if (decision <= 0)
			{
				current_point.x += incx;
				decision+=2*dy;

			}
			else
			{
				current_point.x += incx;
				current_point.y += incy;
				decision += 2*dy - 2*dx;
			}

			result.push_back(current_point);
		}
	}
	else
	{
		decision = 2*dx-dy;
		//for con las y
		for (int i = 0; i < dy; i++)
		{
			if (decision <= 0)
			{
				current_point.y += incy;
				decision += 2*dx;
			}
			else
			{
				current_point.y += incy;
				current_point.x += incx;
				decision += 2*dx - 2*dy;
			}

			result.push_back(current_point);
		}
	}


	//add the last point
	result.push_back( end );

	return result;
}

std::vector<Point> Line::CalculePointsUsingDDA()
{
	std::vector<Point> result;

	//DDA algorythm
	//...

	float xstart = start.x;
	float xend = end.x;

	float ystart = start.y;
	float yend = end.y;

	float dist;
    float dx, dy;
    
	Point currentpoint;

	if ( fabs(xend-xstart) >= fabs(yend-ystart) )
	{
        dist = fabs (xend-xstart);
	}
    else
	{
        dist = fabs (yend-ystart);
	}

    dx = (xend-xstart) / dist;
    dy = (yend-ystart) / dist;

    currentpoint.x = xstart+sgn(xstart)*0.5;
    currentpoint.y = ystart+sgn(ystart)*0.5;

    for (int i=0; i<=dist; i++)
    {
		result.push_back(currentpoint);

        currentpoint.x += dx;
        currentpoint.y += dy;
    }

	return result;
}

//Cercle class
//...
Circle::Circle(Point a, Point b)
{
	radius = sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
	center = a;
}

//Calculate Bresenham Circle Points
std::vector<Point> Circle::CalculePointsUsingBresenham()
{
	std::vector<Point> result;

	float d = 5.0/4.0 - this->radius;

    Point currentpoint;

	currentpoint.x=0;
	currentpoint.y=this->radius;

	//Hay que generar todos los octantes!!
	//Octantes principales
	result.push_back(Point(currentpoint.x+center.x, currentpoint.y + center.y, 0));
	result.push_back(Point(currentpoint.x+center.x, -(currentpoint.y + center.y), 0));
	result.push_back(Point(currentpoint.y+center.x, currentpoint.x + center.y, 0));
	result.push_back(Point(currentpoint.y+center.x, -(currentpoint.x + center.y), 0));
	//paint_point(x, y, c);

    while (currentpoint.y > currentpoint.x)
	{
        if ( d < 0) 
		{
            d+=2*currentpoint.x+3;
            currentpoint.x++;
        }
        else 
		{
            d+=2*(currentpoint.x-currentpoint.y)+5;
            currentpoint.x++;
            currentpoint.y--;
        }
		
		//Hay que generar todos los octantes
		result.push_back(Point(currentpoint.x+center.x, currentpoint.y + center.y, 0));
		result.push_back(Point (-currentpoint.x+center.x, currentpoint.y + center.y, 0));
		result.push_back(Point(-currentpoint.x+center.x, -currentpoint.y + center.y, 0));
		result.push_back(Point(currentpoint.x+center.x, -currentpoint.y + center.y, 0));
		result.push_back(Point(currentpoint.y+center.x, currentpoint.x + center.y, 0));
		result.push_back(Point(-currentpoint.y+center.x, currentpoint.x + center.y, 0));
		result.push_back(Point(-currentpoint.y+center.x, -currentpoint.x + center.y, 0));
		result.push_back(Point(currentpoint.y+center.x, -currentpoint.x + center.y, 0));
        //paint_point(x, y, c);
	}

	return result;
}



