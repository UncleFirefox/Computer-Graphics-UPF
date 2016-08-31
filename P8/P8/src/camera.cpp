#include "camera.h"

#ifdef _WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

Camera::Camera()
{
	view_matrix.setIdentity();
	//setOrthographic(-100,100,100,-100,-100,100);
}

void Camera::set()
{
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( view_matrix.m );

	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( projection_matrix.m );
}

Vector3 Camera::getLocalVector(const Vector3& v)
{
	Matrix44 iV = view_matrix;
	iV.inverse();
	Vector3 result = iV.rotateVector(v);
	return result;
}

void Camera::move(Vector3 delta)
{
	Vector3 localDelta = getLocalVector(delta);
	eye = eye - localDelta;
	center = center - localDelta;
	updateViewMatrix();
}

void Camera::rotate(float angle, const Vector3& axis)
{
	Matrix44 R;
	R.setRotation(angle,axis);
	Vector3 new_front = R * (center - eye);
	center = eye + new_front;
	updateViewMatrix();
}

void Camera::setOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane)
{
	type = ORTHOGRAPHIC;

	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	updateProjectionMatrix();
}

void Camera::setPerspective(float fov, float aspect, float near_plane, float far_plane)
{
	type = PERSPECTIVE;

	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	//update projection
	updateProjectionMatrix();
}

void Camera::lookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	this->eye = eye;
	this->center = center;
	this->up = up;

	updateViewMatrix();
}

void Camera::updateViewMatrix()
{
	//We activate the matrix we want to work: modelview
	glMatrixMode(GL_MODELVIEW);

	//We set it as identity
	glLoadIdentity();

	//We find the look at matrix
	gluLookAt( eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);

	//We get the matrix and store it in our app
	glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix.m );
}

// ******************************************

//Create a projection matrix
void Camera::updateProjectionMatrix()
{
	//We activate the matrix we want to work: projection
	glMatrixMode(GL_PROJECTION);

	//We set it as identity
	glLoadIdentity();

	if (type == PERSPECTIVE)
		gluPerspective(fov, aspect, near_plane, far_plane);
	else
		glOrtho(left,right,bottom,top,near_plane,far_plane);

	//upload to hardware
	glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix.m );
}
