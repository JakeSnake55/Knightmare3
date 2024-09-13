#include "TimeHandler.h"
#include <allegro5/allegro5.h>
#include <ctime>
#include <iostream>
#include "knightmare3config.h"

int timePerSecond = 1;


TimeHandler::TimeHandler() :
  fastmode(false), tickRate(60), elapsedTime(0),
  dayLength(1800), day(0), unlinkedTime(0), clockTicks(0),
  deltaTime(0)
{ }

void TimeHandler::syncTime() {
  al_wait_for_vsync();
  deltaTime = clock();
}

/*Determines how long it has been since the last frame, potentially allowing the next frame to be drawn*/
void TimeHandler::changeTime() {
  clock_t clocksPerFrame = (clock_t)(((float)CLOCKS_PER_SEC) / (float)tickRate);
  clock_t currentTime = clock();
  if ((currentTime - deltaTime) >= clocksPerFrame) {
    timer = true;
    //Measure Clocks between cycles with clock intervals between executions
    clockTicks = (currentTime - deltaTime);
    int timesOver = ((currentTime - deltaTime) / clocksPerFrame - 0.5);
    deltaTime += clocksPerFrame * timesOver;
  }
  elapsedTime = (float)(currentTime) / CLOCKS_PER_SEC - (double)day * dayLength;

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

