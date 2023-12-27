#pragma once
#include <allegro5/allegro5.h>
#include "Settings.h"
#include "Camera.h"

class Window 
{
private:
	void installs();
	void createWindow();
	void createEventQueue();
	void setupImgui();

public:
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_EVENT event;
	Settings settings;
	Camera camera;
	Window();
	void buildDebugWindow();
	void buildMainMenu();
	void cleanExit();
	bool getEvent();
	void render();
};