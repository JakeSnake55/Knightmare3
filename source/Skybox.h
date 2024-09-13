#pragma once
#include <random>
#include "Defs.h"
#include "Window.h"

class Skybox {
private:
	void StarPositioner();
	std::mt19937 gen;
	Stars starList[starCount];
	Orbital moonSpotsList[spotsCount];
	Orbital sunSpotsList[spotsCount];
public:
	Skybox();
	void loadSeed(int seed);
	void add_skybox(Window& window, Project& km, int& startSky, int& endSky);
};




