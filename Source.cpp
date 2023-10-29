#include<allegro5/allegro5.h>
#include<allegro5/allegro_primitives.h>

#include "imgui.h"
#include "imgui_impl_allegro5.h"

#include "Window.h"

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

// Draw the screen contents
void drawFrame(Window window) 
{
    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    window.render();
    al_clear_to_color(al_map_rgba_f(0, 0.3, 0.1, 0.2));
    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
}

void processFrame(Window &window, bool &running) 
{
    while (window.getEvent())
    {
        processEvents(window, running);
    }
    drawFrame(window);
}

void mainLoop(Window window) 
{
    bool running = true;
    while (running) {
        processFrame(window, running);
    }
    window.cleanExit();
}

int main() 
{
    Window window;
    mainLoop(window);
	return 0;
}