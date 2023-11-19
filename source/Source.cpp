#include<allegro5/allegro5.h>
#include<allegro5/allegro_primitives.h>

#include "imgui.h"
#include "imgui_impl_allegro5.h"

#include "Window.h"
#include "Settings.h"
#include "knightmare3config.h"
#ifndef DEBUG
#define DEBUG false
#endif

void processEvents(Window &window, bool &running) 
{
    ImGui_ImplAllegro5_ProcessEvent(&window.event);
    if (window.event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        running = false;
    if (window.event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
    {
        ImGui_ImplAllegro5_InvalidateDeviceObjects();
        al_acknowledge_resize(window.display);
        ImGui_ImplAllegro5_CreateDeviceObjects();
    }
}

ALLEGRO_VERTEX v[] = {
    {.x = 128, .y = 0, .z = 0, .u = 128, .v = 0, .color = al_map_rgb(255,255,0)},
    {.x = 0, .y = 256, .z = -1, .u = 0, .v = 256, .color = al_map_rgb(255,255,0)},
    {.x = 256, .y = 256, .z = 0, .u = 256, .v = 256, .color = al_map_rgb(255,255,0)} };
float x = 0;
float y = -1;
float z = 3;


// Draw the screen contents
void drawFrame(Window &window)
{
    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();

    al_clear_to_color(al_map_rgba_f(0, 0.3, 0.1, 0.2));
    if (window.settings.showDemoWindow) {
        ImGui::ShowDemoWindow();
    }
    if (window.settings.drawPrimitives) {
        //Temporary placement, change when you understand how to do it
        ALLEGRO_TRANSFORM trans;
        al_identity_transform(&trans);
        al_translate_transform_3d(&trans, 0, 0, -1);
        al_perspective_transform(&trans, -1 * 0.7 * 0.4, 0.4,
            1,
            0.4 * 0.7, -0.4, 1000);

        al_use_transform(&trans);
        al_draw_prim(v,NULL,0,0,3,ALLEGRO_PRIM_TRIANGLE_LIST);

    }
    if (window.settings.showMainMenu) {
        window.buildMainMenu();
    }
    if (DEBUG) { //Only for Developers
        window.buildDebugWindow();
    }
    window.render();
    
}

void processFrame(Window &window, bool &running) 
{
    while (window.getEvent())
    //Following occurs for every allegro event
    {
        processEvents(window, running);
    }

    //Following occurs each time that the computer can
    if (window.settings.waitForVSync) {//Should wait till screen refresh
        al_wait_for_vsync();
    }
    drawFrame(window);
}

void mainLoop() 
{
    Window window;
    bool running = true;
    
    while (running) {
        processFrame(window, running);
    }
    window.cleanExit();
}

int main() 
{
    mainLoop();
	return 0;
}