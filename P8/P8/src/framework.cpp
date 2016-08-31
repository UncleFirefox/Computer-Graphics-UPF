#include "framework.h"
#include <cassert>
#include <cmath> //for sqrt (square root) function

//**************************************
#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

long getTime()
{
	#ifdef _WIN32
		return GetTickCount();
	#else
		struct timeval tv;
		gettimeofday(&tv,NULL);
		return (int)(tv.tv_sec*1000 + (tv.tv_usec / 1000));
	#endif
}

// **************************************

Vector3::Vector3()
{
	x = y = z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

double Vector3::length() 
{
	return sqrt(x*x + y*y + z*z);
}

double Vector3::length() const
{
	return sqrt(x*x + y*y + z*z);
}

void Vector3::normalize()
{
	double len = length();
	x /= len;
	y /= len;
	z /= len;
}

float Vector3::distance(const Vector3& v)
{
	return (v - *this).length();
}

Vector3 Vector3::cross( const Vector3& b )
{
	return Vector3(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
}

float Vector3::dot( const Vector3& v )
{
	return x*v.x + y*v.y + z*v.z;
}

void Vector3::random(float range)
{
	//rand returns a value between 0 and RAND_MAX
	x = (rand() / (double)RAND_MAX) * 2 * range - range; //value between -range and range
	y = (rand() / (double)RAND_MAX) * 2 * range - range; //value between -range and range
	z = (rand() / (double)RAND_MAX) * 2 * range - range; //value between -range and range
}

//*********************************
Matrix44::Matrix44()
{
	clear();
}

void Matrix44::clear()
{
	memset(m, 0, 16*sizeof(float));
}

void Matrix44::setIdentity()
{
	m[0]=1; m[4]=0; m[8]=0; m[12]=0;
	m[1]=0; m[5]=1; m[9]=0; m[13]=0;
	m[2]=0; m[6]=0; m[10]=1; m[14]=0;
	m[3]=0; m[7]=0; m[11]=0; m[15]=1;
}

void Matrix44::transpose()
{
   std::swap(m[1],m[4]); std::swap(m[2],m[8]); std::swap(m[3],m[12]);
   std::swap(m[6],m[9]); std::swap(m[7],m[13]); std::swap(m[11],m[14]);
}

void Matrix44::traslate(float x, float y, float z)
{
	Matrix44 T;
	T.setTranslation(x, y, z);
	*this = *this * T;
}

void Matrix44::rotate( float angle_in_rad, const Vector3& axis )
{
	Matrix44 R;
	R.setRotation(angle_in_rad, axis);
	*this = *this * R;
}

Vector3 Matrix44::rotateVector(const Vector3& v)
{
	Matrix44 temp = *this;
	temp.m[12] = 0.0;
	temp.m[13] = 0.0;
	temp.m[14] = 0.0;
	return temp * v;
}

void Matrix44::traslateLocal(float x, float y, float z)
{
	Matrix44 T;
	T.setTranslation(x, y, z);
	*this = T * *this;
}

void Matrix44::rotateLocal( float angle_in_rad, const Vector3& axis )
{
	Matrix44 R;
	R.setRotation(angle_in_rad, axis);
	*this = R * *this;
}

//To create a traslation matrix
void Matrix44::setTranslation(float x, float y, float z)
{
	setIdentity();
	m[12] = x;
	m[13] = y;
	m[14] = z;
}

//To create a rotation matrix
void Matrix44::setRotation( float angle_in_rad, const Vector3& axis  )
{
	clear();
	Vector3 axis_n = axis;
	axis_n.normalize();

	double c = cos( angle_in_rad );
	double s = sin( angle_in_rad );
	double t = 1 - c;

	M[0][0] = t * axis.x * axis.x + c;
	M[0][1] = t * axis.x * axis.y - s * axis.z;
	M[0][2] = t * axis.x * axis.z + s * axis.y;

	M[1][0] = t * axis.x * axis.y + s * axis.z;
	M[1][1] = t * axis.y * axis.y + c;
	M[1][2] = t * axis.y * axis.z - s * axis.x;

	M[2][0] = t * axis.x * axis.z - s * axis.y;
	M[2][1] = t * axis.y * axis.z + s * axis.x;
	M[2][2] = t * axis.z * axis.z + c;

	M[3][3] = 1.0f;
}

void Matrix44::orthonormalize()
{
	Vector3 top = topVector();
	Vector3 right = rightVector();
	Vector3 front = frontVector();

	top.normalize();
	front.normalize();
	right.normalize();

	m[0] = right.x;
	m[1] = right.y;
	m[2] = right.z;

	m[4] = top.x;
	m[5] = top.y;
	m[6] = top.z;

	m[8] = front.x;
	m[9] = front.y;
	m[10] = front.z;
}

//Multiply a matrix by another and returns the result
Matrix44 Matrix44::operator*(const Matrix44& matrix) const
{
	Matrix44 ret;

	unsigned int i,j,k;
	for (i=0;i<4;i++) 	
	{
		for (j=0;j<4;j++) 
		{
			ret.M[i][j]=0.0;
			for (k=0;k<4;k++) 
				ret.M[i][j] += M[i][k] * matrix.M[k][j];
		}
	}

	return ret;
}

//it allows to add two vectors
Vector3 operator + (const Vector3& a, const Vector3& b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z );
}

//it allows to add two vectors
Vector3 operator - (const Vector3& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z );
}

Vector3 operator * (const Vector3& a, float v)
{
	return Vector3(a.x * v, a.y * v, a.z * v);
}

//Multiplies a vector by a matrix and returns the new vector
Vector3 operator * (const Matrix44& matrix, const Vector3& v)
{   
   float x = matrix.m[0] * v.x + matrix.m[4] * v.y + matrix.m[8] * v.z + matrix.m[12]; 
   float y = matrix.m[1] * v.x + matrix.m[5] * v.y + matrix.m[9] * v.z + matrix.m[13]; 
   float z = matrix.m[2] * v.x + matrix.m[6] * v.y + matrix.m[10] * v.z + matrix.m[14];
   return Vector3(x,y,z);
}

bool Matrix44::inverse()
{
   unsigned int i, j, k, swap;
   float t;
   Matrix44 temp, final;
   final.setIdentity();

   temp = (*this);

   unsigned int m,n;
   m = n = 4;
	
   for (i = 0; i < m; i++)
   {
      // Look for largest element in column

      swap = i;
      for (j = i + 1; j < m; j++)// m or n
	  {
		 if ( fabs(temp.M[j][i]) > fabs( temp.M[swap][i]) )
            swap = j;
	  }
   
      if (swap != i)
      {
         // Swap rows.
         for (k = 0; k < n; k++)
         {
			 std::swap( temp.M[i][k],temp.M[swap][k]);
			 std::swap( final.M[i][k], final.M[swap][k]);
         }
      }

      // No non-zero pivot.  The CMatrix is singular, which shouldn't
      // happen.  This means the user gave us a bad CMatrix.


#define MATRIX_SINGULAR_THRESHOLD 0.000001

      if ( fabsf(temp.M[i][i]) <= MATRIX_SINGULAR_THRESHOLD)
	  {
		  final.setIdentity();
         return false;
	  }
#undef MATRIX_SINGULAR_THRESHOLD

      t = 1.0f/temp.M[i][i];

      for (k = 0; k < n; k++)//m or n
      {
         temp.M[i][k] *= t;
         final.M[i][k] *= t;
      }

      for (j = 0; j < m; j++) // m or n
      {
         if (j != i)
         {
            t = temp.M[j][i];
            for (k = 0; k < n; k++)//m or n
            {
               temp.M[j][k] -= (temp.M[i][k] * t);
               final.M[j][k] -= (final.M[i][k] * t);
            }
         }
      }
   }

   *this = final;

   return true;
}

Matrix44 Matrix44::getRotationMatrix()
{
	Matrix44 tmp = *this;
	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	return tmp;
}
