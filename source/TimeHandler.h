#pragma once
#pragma once 
#include <ctime>

namespace timehandler
{
	extern bool fastmode;
	extern int tickRate;
	extern double elapsedTime;
	extern clock_t deltaTime;
	extern int unlinkedTime;
	extern unsigned int day;
	extern double dayLength;
	extern int clockTicks;
	void changeTime();
}

