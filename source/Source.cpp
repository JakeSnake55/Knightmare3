/*

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
#include "Skybox.h"
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

    if (!window.settings.turnCamera) 
    {
        return;
    }

    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
        camera_move_along_direction(&window.camera, -1, 0);

    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_D)))
        camera_move_along_direction(&window.camera,1, 0);

    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_S))) 
    {
        camera_move_along_direction(&window.camera, 0, -1);
    }

    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_W))) 
    {
        camera_move_along_ground(&window.camera, 0, 1);
    }
    
    if (window.event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        camera_rotate_around_axis(&window.camera, window.camera.right, -(double)window.event.mouse.dy*SCALE);
        camera_rotate_around_axis(&window.camera, window.camera.upwards, -(double)window.event.mouse.dx*SCALE);
        
        al_set_mouse_xy(window.display, al_get_display_width(window.display)/2 , al_get_display_height(window.display)/2);
    }
}

ALLEGRO_VERTEX v[] = {
    {.x = -1, .y = 0, .z = -1, .u = -1, .v = 0, .color = al_map_rgb(255,255,0)},
    {.x = 0, .y = -2, .z = -1, .u = 0, .v = -2, .color = al_map_rgb(255,255,0)},
    {.x = -2, .y = -2, .z = -1, .u = -2, .v = -2, .color = al_map_rgb(255,255,0)} };
float x = 0;
float y = -1;
float z = 3;


// Draw the screen contents
void drawFrame(Window &window)
{
    timehandler::changeTime();
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
        

        
        al_build_camera_transform(&camera, 
            window.camera.position.x, window.camera.position.y, window.camera.position.z,
            window.camera.position.x - window.camera.forwards.x, window.camera.position.y - window.camera.forwards.y, window.camera.position.z - window.camera.forwards.z,
            0,1,0);
        al_use_transform(&camera);

        // At a z distance of 4 with a 90° hfov everything would be scaled
        // down to 25%. However we also zoom 2-fold, so the final scaling is
        // 50%. This rectangle therefore will appear at a size of 400 x 225
        // pixel (assuming the display is 800 x 450).

        al_draw_filled_rectangle(-1, -1, 1, 1, al_map_rgb(255,0,0));
        al_draw_prim(v,NULL,0,0,3,ALLEGRO_PRIM_TRIANGLE_LIST);

        al_set_render_state(ALLEGRO_DEPTH_TEST, 0);

        al_clear_depth_buffer(1);
        ex.n = startSky;
        
        add_skybox(window.camera);

        al_draw_prim(ex.v, NULL, NULL, startSky, endSky, ALLEGRO_PRIM_TRIANGLE_LIST);

        al_clear_depth_buffer(1);
        al_set_render_state(ALLEGRO_DEPTH_TEST, 1);

    }
    if (window.settings.showMainMenu) {
        window.buildMainMenu();
    }
    if (window.settings.makeNewWorld) {
      window.buildWorldCreationMenu(window.settings.currentId);
    }
    if (DEBUG) { //Only for Developers
        window.buildDebugWindow();
    }
    window.render();
    
}

void processFrame(Window &window, bool &running) 
{
    window.camera.velocity.x = 0;
    window.camera.velocity.y = 0;
    window.camera.velocity.z = 0;

    while (window.getEvent())
    //Following occurs for every allegro event
    {
        processEvents(window, running);
    }
    vector_normalize(window.camera.velocity);

    //Following occurs each time that the computer can
    if (window.settings.waitForVSync) {//Should wait till screen refresh
        al_wait_for_vsync();
    }
    vector_mul(window.camera.velocity, ImGui::GetIO().DeltaTime);
    
    vector_iadd(&window.camera.position, window.camera.velocity);
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

int _main() 
{
    mainLoop();
	return 0;
}
*/