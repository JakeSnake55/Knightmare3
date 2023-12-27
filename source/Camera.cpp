#include "Camera.h"
#include "knightmare3config.h"
#include <climits>
#include <cmath>

//Constructor
Camera::Camera()
{
	yaw = 0;
	pitch = 0;
	forwards = { 0,0,-1 };
	left = { 0,0,0 };
}

void Camera::findVectors() {
	double x = cameraYaw();
	double y = cameraPitch();
	forwards.x = - sin(-x) * cos(y);
	forwards.y = - sin(y);
	forwards.z = - cos(-x) * cos(y);


}

double Camera::cameraYaw() {
	double temp = yaw * ALLEGRO_PI *2;
	return temp / UINT16_MAX;
}

double Camera::cameraPitch() {
	double temp = pitch * ALLEGRO_PI *2;
	return temp / UINT16_MAX;
}

void Camera::addYaw(double value) {
	if (UINT16_MAX - (value * SCALE) < yaw) {
		yaw =0;
	}
	yaw += (value*SCALE) ;
}

void Camera::addPitch(double value) {
	if (UINT16_MAX - (value * SCALE) < pitch) {
		pitch =0;
	}
	pitch += (value*SCALE);
}