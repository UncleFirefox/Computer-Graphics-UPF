#include "camera.h"

#ifdef _WIN32 //under windows we need this file to make opengl work
	#include <windows.h>
#endif
#include <GL/glut.h> //including GLUT we include everything (opengl, glu and glut)

const float PI = 3.1415926f;

void Camera::setProperties(float fov, float aspect, float near_plane, float far_plane)
{
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
	//Creacion de la model

	Matrix44 model;
	model.setTranslation(-eye.x,-eye.y,-eye.z);

	//Creacion de la view

	//Crear los vectores front side y top

	Vector3 front = this->center - this->eye;
	front.normalize();

	//Normalizamos el up antes de hacer el producto vectorial
	this->up.normalize();

	Vector3 side = front.cross(this->up);
	side.normalize();

	Vector3 top = side.cross(front);
	top.normalize();

	//Ya tenemos los valores principales de la view, a crear la matriz!!!

	this->view_matrix.m[0] = side.x ;	this->view_matrix.m[1] = side.y ;	this->view_matrix.m[2] = side.z ;	this->view_matrix.m[3] = 0.0f ;
	this->view_matrix.m[4] = top.x ;	this->view_matrix.m[5] = top.y ;	this->view_matrix.m[6] = top.z ;	this->view_matrix.m[7] = 0.0f ;
	this->view_matrix.m[8] = -front.x ;	this->view_matrix.m[9] = -front.y ;	this->view_matrix.m[10] = -front.z ;	this->view_matrix.m[11] = 0.0f ;	
	this->view_matrix.m[12] =0.0f ;	this->view_matrix.m[13] = 0.0f ;	this->view_matrix.m[14] = 0.0f ;	this->view_matrix.m[15] = 1.0f;


	//Presuntamente hay que trasponer la matriz view

	this->view_matrix.transpose();

	//Una vez tenemos ya todos los valores, multiplicamos la model por la matrix y listo

	this->view_matrix = model * this->view_matrix;

}

// ******************************************

//Create a projection matrix
void Camera::updateProjectionMatrix()
{
	float f = 1/(tan(this->fov/2));
	float farnear = (this->far_plane + this->near_plane)/(this->near_plane - this->far_plane);

	this->projection_matrix.m[0] = f/this->aspect ;	this->projection_matrix.m[1] = 0.0f ;	this->projection_matrix.m[2] = 0.0f ;	this->projection_matrix.m[3] = 0.0f ;
	this->projection_matrix.m[4] = 0.0f ;	this->projection_matrix.m[5] = f;	this->projection_matrix.m[6] = 0.0f ;	this->projection_matrix.m[7] = 0.0f ;
	this->projection_matrix.m[8] = 0.0f ;	this->projection_matrix.m[9] = 0.0f;	this->projection_matrix.m[10] = farnear ;	this->projection_matrix.m[11] = 2*farnear ;
	this->projection_matrix.m[12] = 0.0f ;	this->projection_matrix.m[13] = 0.0f;	this->projection_matrix.m[14] = -1.0f ;	this->projection_matrix.m[15] = 1.0f ;

	this->projection_matrix.transpose();
}

void Camera::rotateCameraLookingCenterY(float angle_deg)
{
	Matrix44 rotation;
	rotation.setRotation((angle_deg * PI)/180.0f, Vector3(0,1,0));
	this->eye = (this->center) + (rotation * (this->eye - this->center));
	this->updateViewMatrix();
}

void Camera::rotateCameraLookingCenterX(float angle_deg)
{
	//Crear los vectores front side y top

	Vector3 front = this->center - this->eye;
	front.normalize();

	//Normalizamos el up antes de hacer el producto vectorial
	this->up.normalize();

	Vector3 right = this->up.cross(front);
	right.normalize();

	Vector3 top = right.cross(front);

	Vector3 side = front.cross(top);
	side.normalize();

	Matrix44 rotation;
	rotation.setRotation((angle_deg * PI)/180.0f, side);

	this->eye = (this->center) + (rotation * (this->eye - this->center));

	this->updateViewMatrix();
}

void Camera::zoomCameraLookingCenter(float units)
{
	Vector3 direction = (this->center - this->eye);
	direction.normalize();
	this->eye = eye + (direction * units);
	this->updateViewMatrix();
}

void Camera::changeCameraLook(Vector3 newpoint)
{
	this->center = newpoint;
	this->updateViewMatrix();
}
