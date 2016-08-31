#include "framework.h"

#include <cmath> //for sqrt (square root) function

//**************************************

#define WIN32

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

const float PI = 3.1415926f;

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

//*********************************
Matrix33::Matrix33()
{
	memset(data,0, sizeof(float) * 9);
}

void Matrix33::setIdentity()
{
	data[0]=1; data[1]=0; data[2]=0;
	data[3]=0; data[4]=1; data[5]=0;
	data[6]=0; data[7]=0; data[8]=1;
}

//multiply one matrix by the other and store the result in this one
void Matrix33::multiply( const Matrix33& m )
{
	//this matrix has to store the concatenation of both matrices
	//...We evade the fors in order to get a much faster approach multiplying a matrix
	
	float buffer[9];

	//Primera fila con columnas 1 2 y 3
	buffer[0] = (this->data[0] * m.data[0]) + (this->data[1] * m.data[3]) + (this->data[2] * m.data[6]);
	buffer[1] = (this->data[0] * m.data[1]) + (this->data[1] * m.data[4]) + (this->data[2] * m.data[7]);
	buffer[2] = (this->data[0] * m.data[2]) + (this->data[1] * m.data[5]) + (this->data[2] * m.data[8]);

	//Segunda fila con columnas 1 2 y 3

	buffer[3] = (this->data[3] * m.data[0]) + (this->data[4] * m.data[3]) + (this->data[5] * m.data[6]);
	buffer[4] = (this->data[3] * m.data[1]) + (this->data[4] * m.data[4]) + (this->data[5] * m.data[7]);
	buffer[5] = (this->data[3] * m.data[2]) + (this->data[4] * m.data[5]) + (this->data[5] * m.data[8]);

	//Tercera fila con columnas 1 2 y 3
	buffer[6] = (this->data[6] * m.data[0]) + (this->data[7] * m.data[3]) + (this->data[8] * m.data[6]);
	buffer[7] = (this->data[6] * m.data[1]) + (this->data[7] * m.data[4]) + (this->data[8] * m.data[7]);
	buffer[8] = (this->data[6] * m.data[2]) + (this->data[7] * m.data[5]) + (this->data[8] * m.data[8]);

	this->data[0] = buffer[0];
	this->data[1] = buffer[1];
	this->data[2] = buffer[2];
	this->data[3] = buffer[3];
	this->data[4] = buffer[4];
	this->data[5] = buffer[5];
	this->data[6] = buffer[6];
	this->data[7] = buffer[7];
	this->data[8] = buffer[8];
}

//multiply the vector by the matrix
void Matrix33::transform( Point& p )
{
	//p should be changed to have the transform
	//be aware of reusing the same container as input on the calculations
	//...

	Point bufferpoint;

	bufferpoint.x = (this->data[0] * p.x) + (this->data[1] * p.y) + (this->data[2]); // La ultima suma es la simulacion del tercer componente en 2D que es 1 pero no es z!
	bufferpoint.y = (this->data[3] * p.x) + (this->data[4] * p.y) + (this->data[5]);
	bufferpoint.z = (this->data[6] * p.x) + (this->data[7] * p.y) + (this->data[8]);

	p = bufferpoint;
}

//assign a matrix to perform a transform
void Matrix33::setTranslation(float x, float y)
{
	//set the matrix data to be a translation matrix
	data[0]=1.0f; data[1]=0.0f; data[2]=x;
	data[3]=0.0f; data[4]=1.0f; data[5]=y;
	data[6]=0.0f; data[7]=0.0f; data[8]=1.0f;
}

void Matrix33::setRotation(float angle)
{
	//set the matrix data to be a rotation matrix
	//remember that the angle will be in DEGREES so convert it to radians
	//...

	float radangle = (angle*PI)/180;

	data[0]=cos(angle); data[1]=-sin(angle); data[2]=0.0f;
	data[3]=sin(angle); data[4]=cos(angle); data[5]=0.0f;
	data[6]=0.0f; data[7]=0.0f; data[8]=1.0f;
}

void Matrix33::setScale(float x, float y)
{
	//set the matrix data to be a scale matrix
	//...
	data[0]=x; data[1]=0.0f; data[2]=0.0f;
	data[3]=0.0f; data[4]=y; data[5]=0.0f;
	data[6]=0.0f; data[7]=0.0f; data[8]=1.0f;
}
