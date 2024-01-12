#include "Camera.h"
#include "knightmare3config.h"
#include <climits>
#include <cmath>

//Constructor
Camera::Camera()
{
	
	forwards = { 0,0,1 };
	right = { 1,0,0 };
	upwards = { 0,1,0 };
}
