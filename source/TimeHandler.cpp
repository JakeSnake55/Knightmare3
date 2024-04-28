#include "TimeHandler.h"
#include <allegro5/allegro5.h>
#include <ctime>

int timePerSecond = 1;


namespace timehandler {
	bool fastmode = false;
	int tickRate = 60;
	double elapsedTime = 0;
	clock_t deltaTime = clock();
	double dayLength = 1800;
	unsigned int day = 0;
	int unlinkedTime = 0;
	int clockTicks;

	/*Changes the values for worldtime and day. Should be what determines all time based events*/
	void changeTime() {
		
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
	}
}