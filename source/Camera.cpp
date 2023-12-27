#include "Camera.h"
#include "knightmare3config.h"
#include <climits>

//Constructor
Camera::Camera()
{
	yaw = 0;
	pitch = 0;
	right = { 1,0,0 };
	up = { 0,0,1 };
}

double Camera::cameraYaw() {
	double temp = yaw * ALLEGRO_PI *2;
	return temp / UINT32_MAX;
}

double Camera::cameraPitch() {
	double temp = pitch * ALLEGRO_PI *2;
	return temp / UINT32_MAX;
}

void Camera::addYaw(double value) {
	if (UINT32_MAX - (value * SCALE) < yaw) {
		yaw =0;
	}
	yaw += (value*SCALE) ;
}

void Camera::addPitch(double value) {
	if (UINT32_MAX - (value * SCALE) < pitch) {
		pitch -= UINT32_MAX;
	}
	pitch += (value*SCALE);
}