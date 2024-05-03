#include "TimeHandler.h"
#include <allegro5/allegro5.h>
#include <ctime>
#include "knightmare3config.h"

int timePerSecond = 1;


TimeHandler::TimeHandler(): 
	fastmode(false),tickRate(60),elapsedTime(0),
	dayLength(1800),day(0),unlinkedTime(0),clockTicks(0),
	deltaTime(0) 
{ }

	/*Changes the values for worldtime and day. Should be what determines all time based events*/
void TimeHandler::changeTime() {
		
		clock_t currentTime = clock();
		elapsedTime = (float)(currentTime)/CLOCKS_PER_SEC - (double)day*dayLength;
		/*Unlimited Speed*/
		if (fastmode) {
			unlinkedTime++;
			if ((elapsedTime + unlinkedTime) >= (double)dayLength) {
				unlinkedTime -= dayLength;
				day++;
			}
		}

		else {
			/*Limited Speed (Usually like 60/second or 20/second)*/
			if ((currentTime - deltaTime) >= (((float)CLOCKS_PER_SEC)/tickRate)) {
				if ((elapsedTime + unlinkedTime) >= (double)dayLength) {
					day++;
				}
			}
		}
		//Measure Clocks between cycles with clock intervals between executions
		clockTicks = (currentTime - deltaTime);
		deltaTime = currentTime;
		fillArray();
}

void TimeHandler::fillArray() {
	frameTimes[point] = clockTicks;
	point = (point + 1) % DEBUGFRAMES;
}