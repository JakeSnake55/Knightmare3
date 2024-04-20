#include "TimeHandler.h"
#include <allegro5/allegro5.h>
#include <ctime>

int offset = 0;
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
		elapsedTime = (float)(currentTime)/CLOCKS_PER_SEC - offset;
		/*Unlimited Framerate Strange behaviour now*/
		if (fastmode) {
			unlinkedTime++;
			if ((elapsedTime + unlinkedTime) >= (double)dayLength) {
				unlinkedTime -= dayLength;
				day++;
			}
			//Measure FPS with time intervals between executions
			clockTicks =  (currentTime - deltaTime);
			deltaTime = currentTime;
		}

		else {
			/*Limited Framerate (Usually like 60/second or 20/second)*/
			if ((currentTime - deltaTime) >= (((float)CLOCKS_PER_SEC)/tickRate)) {

				if ((elapsedTime + unlinkedTime) >= (double)dayLength) {
					offset += dayLength;
					day++;
				}
				//Measure FPS with time intervals between executions
				clockTicks = (currentTime-deltaTime);
				deltaTime = currentTime;
			}
		}
	}
}