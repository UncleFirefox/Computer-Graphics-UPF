#ifndef CAMERA_H
#define CAMERA_H

#include "framework.h"

class Camera
{
public:
	enum { PERSPECTIVE, ORTHOGRAPHIC };

	char type; //camera type

	//vectors to define the orientation of the camera
	Vector3 eye;
	Vector3 center;
	Vector3 up;

	//properties of the projection of the camera
	float fov;			//view angle
	float aspect;		//aspect ratio
	float near_plane;	//near plane
	float far_plane;	//far plane

	//for orthogonal projection
	float left,right,top,bottom;

	//matrices
	Matrix44 view_matrix;
	Matrix44 projection_matrix;

	Camera();
	void set();

	void move(Vector3 delta);
	void rotate(float angle, const Vector3& axis);

	Vector3 getLocalVector(const Vector3& v);

	void setPerspective(float fov, float aspect, float near_plane, float far_plane);
	void setOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane);
	void lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
	void moveToSee(Vector3 Position);

	void updateViewMatrix();
	void updateProjectionMatrix();
};


#endif