#pragma once
#include <allegro5/allegro5.h>

const int starCount = 100;
const int spotsCount = 50;

class Settings 
{

public:
	bool showDemoWindow = false;
	bool showMainMenu = true;
	bool showOptionsMenu = false;
	bool waitForVSync = true;
	bool wireFrame = false;
	bool drawTerrain = false;
	bool drawSkybox = false;
	bool turnCamera = false;
	bool keyboardSleep = true;

	bool redrawChunks = true;
	float FOV = 70;
	float zoom = 1;
	bool makeNewWorld = false;
	int currentId = -1;

	bool drawNewSkybox = false;

};