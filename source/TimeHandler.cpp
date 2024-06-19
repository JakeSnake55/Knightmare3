#include "TimeHandler.h"
#include <allegro5/allegro5.h>
#include <ctime>
#include <iostream>
#include "knightmare3config.h"

int timePerSecond = 1;


TimeHandler::TimeHandler(): 
	fastmode(false),tickRate(60),elapsedTime(0),
	dayLength(1800),day(0),unlinkedTime(0),clockTicks(0),
	deltaTime(0), refresh(60), processTime(0), renderTime(0), vsyncTime(0)
{ }

void TimeHandler::syncTime() {
	al_wait_for_vsync();
	deltaTime = clock();
}

void TimeHandler::processLog() {
	if(processTime==frameTimes[PROCESS][(point-1)%DEBUGFRAMES])
		processTime = clock() - deltaTime;
}

void TimeHandler::renderLog() {
	if (renderTime == frameTimes[RENDER][(point - 1) % DEBUGFRAMES])
	  renderTime = clock() - deltaTime;
}

void TimeHandler::vsyncLog() {
	if (vsyncTime == frameTimes[VSYNC][(point - 1) % DEBUGFRAMES])
	  vsyncTime = clock() - deltaTime;
}

	/*Changes the values for worldtime and day. Should be what determines all time based events*/
void TimeHandler::changeTime() {
		
		clock_t currentTime = clock();
		if ((currentTime - deltaTime) >= (clock_t)(((float)CLOCKS_PER_SEC) /(float)tickRate)) {
			timer = true;
			//Measure Clocks between cycles with clock intervals between executions
			clockTicks = (currentTime - deltaTime);
			int timesOver = ((currentTime - deltaTime) / (clock_t)(((float)CLOCKS_PER_SEC) / (float)tickRate)-0.5);
			deltaTime += (clock_t)(((float)CLOCKS_PER_SEC)/(float)tickRate)*timesOver;
			fillArray();
		}
		elapsedTime = (float)(currentTime)/CLOCKS_PER_SEC - (double)day*dayLength;

}

void TimeHandler::changeWorldTime() {
	if (fastmode) {
		unlinkedTime++;
	}
	if ((elapsedTime + unlinkedTime) >= (double)dayLength) {
		if (fastmode) {
			unlinkedTime -= dayLength;
		}
		day++;
	}
}

void TimeHandler::fillArray() {
	frameTimes[TOTAL][point] = clockTicks;
	frameTimes[PROCESS][point] = processTime;
	frameTimes[RENDER][point] = renderTime;
	frameTimes[VSYNC][point] = vsyncTime;
	point = (point + 1) % DEBUGFRAMES;

}