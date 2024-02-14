#include "TimeHandler.h"
#include <allegro5/allegro5.h>
#include "IMGUI/imgui.h"
int offset = 0;
int timePerSecond = 1;


namespace timehandler {
	int tickRate = 60;
	double elapsedTime = 0;
	double deltaTime = 0;
	double dayLength = 1800;
	unsigned int day = 0;
	int unlinkedTime = 0;
	float framesPerSecond;

	/*Changes the values for worldtime and day. Should be what determines all time based events*/
	void changeTime() {
		
		double currentTime = ImGui::GetTime();
		elapsedTime = currentTime - offset;
		/*Unlimited Framerate (Usually like 1000000000/second)(so long the draw requirements are low)*/
		if (tickRate < 0) {
			unlinkedTime++;
			if ((elapsedTime + unlinkedTime) >= (double)dayLength) {
				unlinkedTime -= dayLength;
				day++;
			}
			//Measure FPS with time intervals between executions
			framesPerSecond = ((float)1 / (currentTime - deltaTime));
			deltaTime = currentTime;
		}

		else {
			/*Limited Framerate (Usually like 60/second or 20/second)*/
			if ((currentTime - deltaTime) >= (1.0 / (float)tickRate)) {

				if ((elapsedTime + unlinkedTime) >= (double)dayLength) {
					offset += dayLength;
					day++;
				}
				//Measure FPS with time intervals between executions
				framesPerSecond = ((float)1 / (currentTime - deltaTime));
				deltaTime = currentTime;
			}
		}
	}
}