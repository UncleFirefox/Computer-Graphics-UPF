#ifndef CAMERA_H
#define CAMERA_H

#include "framework.h"

class Camera
{
public:
	//vectors to define the orientation of the camera
	Vector3 eye;
	Vector3 center;
	Vector3 up;

	//properties of the projection of the camera
	float fov;			//view angle
	float aspect;		//aspect ratio
	float near_plane;	//near plane
	float far_plane;	//far plane

	//matrices
	Matrix44 view_matrix;
	Matrix44 projection_matrix;

	void setProperties(float fov, float aspect, float near_plane, float far_plane);
	void lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	void updateViewMatrix();
	void updateProjectionMatrix();
	
	//Custom camera functions
	void rotateCameraLookingCenterY(float angle_deg);
	void rotateCameraLookingCenterX(float angle_deg);
	void zoomCameraLookingCenter(float units);
	void changeCameraLook(Vector3 newpoint);
};


#endif