#pragma once

class Settings 
{

public:
	bool showDemoWindow = false;
	bool showMainMenu = true;
	bool waitForVSync = true;
	bool drawPrimitives = true;
	bool turnCamera = false;
	float FOV = 90;
	float zoom = 1;
	float x = 0;
	float y = 0;
	bool makeNewWorld = false;
	int currentId = -1;
};