#include<allegro5/allegro5.h>
#include<allegro5/allegro_primitives.h>

#include <math.h>
#include <iostream>

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_allegro5.h"

#include "Window.h"
#include "Camera.h"
#include "Settings.h"
#include "knightmare3config.h"
#ifndef DEBUG true
#define DEBUG false
#endif




void processEvents(Window &window, bool &running) 
{
    ImGui_ImplAllegro5_ProcessEvent(&window.event);
    if (window.event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        running = false;

    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        running = false;

   
    
    if (window.event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
    {
        ImGui_ImplAllegro5_InvalidateDeviceObjects();
        al_acknowledge_resize(window.display);
        ImGui_ImplAllegro5_CreateDeviceObjects();
    }
    if (window.event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        if (window.settings.turnCamera) {
            window.camera.addPitch(window.event.mouse.dy);
            window.camera.addYaw(window.event.mouse.dx);
            al_get_display_height(window.display);
            al_set_mouse_xy(window.display, al_get_display_width(window.display)/2 , al_get_display_height(window.display)/2);
        }
    }
}

ALLEGRO_VERTEX v[] = {
    {.x = -1, .y = 0, .z = 0, .u = -1, .v = 0, .color = al_map_rgb(255,255,0)},
    {.x = 0, .y = -2, .z = 0, .u = 0, .v = -2, .color = al_map_rgb(255,255,0)},
    {.x = -2, .y = -2, .z = 0, .u = -2, .v = -2, .color = al_map_rgb(255,255,0)} };
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

        float w = al_get_display_width(window.display), h = al_get_display_height(window.display); // assume our display is 800 x 450
        float fov = tan(window.settings.FOV * ALLEGRO_PI / 360); // 90 degree field of view
        float aspect = h / w;

        // This projection is very different from the one in the first example.
        // Here we map the left and right edge of the screen to -1 and +1. And
        // the y axis goes from -1 at the bottom to +1 at the top, scaled by
        // the aspect ratio. We also add a zoom parameter so we can control
        // the visible portion of the scene independent of the field of view.

        ALLEGRO_TRANSFORM projection;
        al_identity_transform(&projection);
        al_perspective_transform(&projection,
            -1 / window.settings.zoom, aspect / window.settings.zoom,
            1 / fov,
            1 / window.settings.zoom, -aspect / window.settings.zoom,
            2000);
        al_use_projection_transform(&projection);
        // Moves everything by -4 in the z direction.


        ALLEGRO_TRANSFORM camera;
        
        
        float x = window.camera.cameraYaw();
        float y = window.camera.cameraPitch();
        float posX = 0;
        float posY = 0;
        float posZ = 4;
        
        al_build_camera_transform(&camera, 
            posX,        posY, posZ, 
            posX-sin(-x)*cos(y), posY-sin(y), posZ-cos(-x)*cos(y), 
            0,        1, 0);
        al_use_transform(&camera);

        // At a z distance of 4 with a 90° hfov everything would be scaled
        // down to 25%. However we also zoom 2-fold, so the final scaling is
        // 50%. This rectangle therefore will appear at a size of 400 x 225
        // pixel (assuming the display is 800 x 450).

        al_draw_filled_rectangle(-1, -1, 1, 1, al_map_rgb(255,0,0));
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