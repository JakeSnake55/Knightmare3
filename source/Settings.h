#pragma once

class Settings 
{

public:
	bool showDemoWindow = false;
	bool showMainMenu = true;
	bool waitForVSync = true;
	bool wireFrame = false;
	bool drawTerrain = false;
	bool drawSkybox = false;
	bool writeDebug = false;
	bool turnCamera = false;
	bool keyboardSleep = true;

	bool redrawChunks = true;
	float FOV = 90;
	float zoom = 1;
	float x = 0;
	float y = 0;
	bool makeNewWorld = false;
	int currentId = -1;
};