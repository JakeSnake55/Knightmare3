#pragma once
#include <allegro5/allegro5.h>
#include <vector>
#include "World.h"
#include "Files.h"
#include "Settings.h"
#include "Defs.h"
#include "IMGUI/imgui.h"
#include "TimeHandler.h"


class Window 
{
private:
	void createWindow();
	void setupImgui();

	void addStyles();

	
	ImVec4 StyleColors[ImGuiCol_COUNT];


public:
	TimeHandler time;
	ALLEGRO_DISPLAY* display;
	Settings settings;
	Camera camera;
	std::vector<World> world;
	Files file;
	Window();
	void buildDebugWindow();
	void buildMainMenu();
	void buildOptionMenu();
	void buildWorldCreationMenu(int id = -1);
	void cleanExit();
	void render();
};

