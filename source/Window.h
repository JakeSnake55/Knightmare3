#pragma once
#include <allegro5/allegro5.h>
#include <vector>
#include "World.h"
#include "Files.h"
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
	std::vector<World> world;
	Files file;
	Window();
	void buildDebugWindow();
	void buildMainMenu();
	void buildWorldCreationMenu(int id = -1);
	void cleanExit();
	bool getEvent();
	void render();
};