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
	clock_t processTime;
	clock_t renderTime;
	clock_t vsyncTime;
	TimeHandler();
	void changeTime();
	void changeWorldTime();
	void fillArray();
	void syncTime();

	void processLog();
	void renderLog();
	void vsyncLog();

	int frameTimes[4][DEBUGFRAMES] = {};
	size_t point = 0;
	bool timer = false;
	float refresh = 60;
	
};

enum logs {
	TOTAL,
	PROCESS,
	RENDER,
	VSYNC
};