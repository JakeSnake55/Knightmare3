#pragma once
#include <ctime>
#include "knightmare3config.h"

struct TimeHandler{
	bool fastmode;
	int tickRate;
	double elapsedTime;
	clock_t deltaTime;
	int unlinkedTime;
	unsigned int day;
	double dayLength;
	clock_t clockTicks;
	TimeHandler();
	void changeTime();
	void changeWorldTime();
	void syncTime();

	size_t point = 0;
	bool timer = false;
	
};
