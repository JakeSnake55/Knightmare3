#pragma once
#include <allegro5/allegro5.h>
#include <cstdint>
#include "Vector.h"


class Camera
{
private:
	uint16_t yaw;
	uint16_t pitch;
	

public:
	Vector forwards;
	Vector left;
	Vector position;
	Vector velocity;
	Camera();

	void findVectors();

	//returns current yaw value from 0 to 2pi
	double cameraYaw();
	//returns current pitch value from 0 to 2pi
	double cameraPitch();
	//adds yaw value to current yaw
	void addYaw(double value);
	//adds pitch value to current pitch
	void addPitch(double value);
};