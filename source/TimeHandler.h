#pragma once
#pragma once 
#include <allegro5/timer.h>

namespace timehandler
{
	extern int tickRate;
	extern double elapsedTime;
	extern double deltaTime;
	extern int unlinkedTime;
	extern unsigned int day;
	extern float framesPerSecond;
	void changeTime();
}

