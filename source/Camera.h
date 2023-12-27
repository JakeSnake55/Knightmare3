#pragma once
#include <allegro5/allegro5.h>
#include <cstdint>
#include "Vector.h"


class Camera
{
private:
	uint32_t yaw;
	uint32_t pitch;
	Vector right;
	Vector up;

public:
	Camera();
	//returns current yaw value from 0 to 2pi
	double cameraYaw();
	//returns current pitch value from 0 to 2pi
	double cameraPitch();
	//adds yaw value to current yaw
	void addYaw(double value);
	//adds pitch value to current pitch
	void addPitch(double value);
};