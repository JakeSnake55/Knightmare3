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
	int clockTicks;
	TimeHandler();
	void changeTime();
	void fillArray();

	int frameTimes[DEBUGFRAMES] = {};
	size_t point = 0;
	
};

