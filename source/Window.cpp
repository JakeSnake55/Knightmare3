
#include "Window.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include <sstream>
#include <iostream>

#include "IMGUI/imgui_internal.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_allegro5.h"
#include "knightmare3config.h"
#include "Settings.h"
#include "World.h"
#include "Files.h"
#include "Defs.h"
#include "TimeHandler.h"



Window::Window()
{
  world.resize(0);
  createWindow();
  setupImgui();
  file.loadWorldFolder(world);
  for (int i = 0; i < ImGuiCol_COUNT; i++) {
    StyleColors[i] = ImGui::GetStyleColorVec4(i);
  }
  time.syncTime();
}

void Window::addStyles() {

  for (int i = 0; i < ImGuiCol_COUNT; i++) {
    ImGui::PushStyleColor(i, StyleColors[i]);
  }
}

static void popStyles() {
  ImGui::PopStyleColor(ImGuiCol_COUNT);
}

void clockOut(float value, std::string prefix) {
  ImGui::Text((prefix + "Clocks, %f milliseconds").c_str(), value);
}



//Builds the Debug window for developer use only
void Window::buildDebugWindow()
{
  addStyles();
  ImGui::Begin("Debug");

  {
    float max[4] = { 0 };
    float min[4] = { 999999 };
    float total[4] = { 0 };

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    static float thickness = 3.0f;

    ImU32 colour[4] = {
      ImColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)),
      ImColor(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), ImColor(ImVec4(0.0f, 0.0f, 1.0f, 1.0f)) };

    const ImVec2 p = ImGui::GetCursorScreenPos();

    const float spacing = 10.0f;
    ImGuiContext& g = *GImGui;


    for (int i = 0; i < IM_ARRAYSIZE(g.FramerateSecPerFrame); i++) {


      if (g.FramerateSecPerFrame[i] > max[0])
        max[0] = g.FramerateSecPerFrame[i];

      if (g.FramerateSecPerFrame[i] < min[0])
        min[0] = g.FramerateSecPerFrame[i];
    }




    float x = p.x + 4.0f;
    float y = p.y + 80.0f;
    for (int i = 0; i < IM_ARRAYSIZE(g.FramerateSecPerFrame); i++) {
      float currTime = 1000.0 * (g.FramerateSecPerFrame[(g.FramerateSecPerFrameIdx + i) % IM_ARRAYSIZE(g.FramerateSecPerFrame)]);

      total[0] += currTime / 1000.0;


      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + thickness, y - currTime), colour[0]);
      x += thickness;
    }



    ImGui::Dummy(ImVec2((spacing) * 10.2f, (spacing) * 3.0f + 80.0f));

    const char* titles[4] = { "Total Time","Process Time","Render Time","VSync Time" };


    ImGui::Text("framerate: %f", g.IO.Framerate);
    for (int j = 0; j < 1; j++) {
      ImGui::Text(titles[j]);
      clockOut(max[j], "Max ");
      clockOut(min[j], "Min ");
      clockOut(total[j] / IM_ARRAYSIZE(g.FramerateSecPerFrame), "Avg ");
    }

  }
  bool camUpdate = settings.turnCamera;

  ImGui::Checkbox("Demo Window", &settings.showDemoWindow);//Shows what is possible with ImGui
  ImGui::Checkbox("VSync", &settings.waitForVSync);//Pauses frames to achieve VSync
  ImGui::Checkbox("Fastforward", &time.fastmode);
  ImGui::InputInt("Tick rate", &time.tickRate);
  ImGui::Checkbox("WireFrame mode", &settings.wireFrame);
  ImGui::Checkbox("Draw Terrain", &settings.drawTerrain);
  ImGui::Checkbox("Redraw Chunks", &settings.redrawChunks);
  ImGui::Checkbox("Draw Sky", &settings.drawSkybox);
  ImGui::Checkbox("Activate Camera", &settings.turnCamera);
  ImGui::Checkbox("Activate Keyboard", &settings.keyboardSleep);
  ImGui::SliderFloat("FOV", &settings.FOV, 0, 180, "%.3f");
  ImGui::SliderFloat("Zoom", &settings.zoom, 0, 10, "%.3f");

  if (camUpdate != settings.turnCamera) {
    settings.mouseSleepUpdate = true;
  }
  ImGui::End();//end a ImGui definition like this always
  popStyles();
}

std::string append_number(std::string const& x, unsigned int num, char sep = '_') {
  std::stringstream s;
  s << x << sep << num;
  return s.str();
}//util for label names


void Window::buildMainMenu()
{
  addStyles();
  static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;

  // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
  // Based on your use case you may want one or the other.
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);

  if (ImGui::Begin("Main Menu", NULL, flags))
  {
    bool NewWorldButton = false;

    //set a bool to process event at end of function
    if (ImGui::Button("New World"))
    {
      NewWorldButton = true;
    }
    ImGui::SameLine();
    ImGui::BeginDisabled(world.size() == 0);
    ImGui::Button("Copy World");
    ImGui::EndDisabled();
    ImGui::SameLine(ImGui::GetWindowWidth() - 65);
    if (ImGui::Button("Options")) {
      settings.showOptionsMenu = true;
    }

    // Left
    static int selected = 0;
    {
      ImGui::BeginChild("left pane", ImVec2(157, al_get_display_height(display) - 40), true, ImGuiWindowFlags_NoScrollbar);
      for (size_t i = 0; i < world.size(); i++)
      {
        if (ImGui::Selectable(world[i].name.c_str(), selected == i))
          selected = i;
      }
      ImGui::EndChild();
    }
    ImGui::SameLine();

    // Right
    if (world.size() > 0) {
      ImGui::BeginGroup();
      ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
      ImGui::Text(world[selected].name.c_str());
      ImGui::Separator();
      if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
      {
        if (ImGui::BeginTabItem("Description"))
        {
          if (ImGui::Button("Play"))
          {
            settings.turnCamera = true;
            settings.mouseSleepUpdate = true;
            settings.currentId = selected;
            settings.showMainMenu = false;
            settings.drawTerrain = true;
            settings.drawSkybox = true;
            settings.keyboardSleep = true;
            settings.drawNewSkybox = true;
          }
          ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Details"))
        {
          ImGui::Text("SEED: %d", world[selected].seed);
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
      ImGui::EndChild();

      ImGui::EndGroup();
    }

    //Process button presses at end
    if (NewWorldButton) {
      settings.showMainMenu = false;
      settings.makeNewWorld = true;
      World w;
      world.insert(world.begin(), w);
      NewWorldButton = false;
    }

    ImGui::End();
  }
  popStyles();

}

void Window::buildOptionMenu()
{

  static ImGuiWindowFlags flags = NULL;

  // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
  // Based on your use case you may want one or the other.

  addStyles();

  ImGui::Begin("Option Menu", NULL, flags);


  if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
  {
    if (ImGui::BeginTabItem("Colours")) {
      for (int i = 0; i < ImGuiCol_COUNT; i++) {
        ImGui::ColorEdit4(ImGui::GetStyleColorName(i), (float*)&StyleColors[i]);
      }
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Misc")) {

      if (&settings.showOptionsMenu && ImGui::Button("Close Options")) {
        settings.showOptionsMenu = false;
      }

      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }

  ImGui::End();


  popStyles();
}

void Window::buildWorldCreationMenu(int id) {
  addStyles();
  if (id == -1) {
    id = 0;
  }
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);

  static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
  if (ImGui::Begin("World Menu", NULL, flags))
  {
    const int worldNameLength = 120;
    char temp[worldNameLength];
    strcpy_s(temp, world[id].name.c_str());
    ImGui::InputTextWithHint("##world_name", "World name", temp, worldNameLength);
    ImGui::Text("Seed");
    ImGui::SameLine();
    ImGui::InputInt("##world_seed", &world[id].seed);

    world[id].name = temp;

    if (&settings.makeNewWorld && ImGui::Button("Create world")) {
      if (!(world[id].name.size() == 0))
      {
        file.addWorldFolder(world[id]);
      }
      else
      {
        world.erase(world.begin());
        world.resize(world.size() - 1);
      }

      settings.makeNewWorld = false;
      settings.currentId = -1;
      settings.showMainMenu = true;
    }
  }
  ImGui::End();
  popStyles();
}


void Window::createWindow()
{
  al_set_new_display_flags(ALLEGRO_RESIZABLE);
  display = al_create_display(1280, 720);
  al_set_window_title(display, "Knightmare 3.0");
}

void Window::setupImgui()
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();


  // Setup Platform/Renderer backends
  ImGui_ImplAllegro5_Init(display);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != nullptr);

}

void Window::cleanExit()
{
  ImGui_ImplAllegro5_Shutdown();
  ImGui::DestroyContext();

  al_destroy_display(display);
}


void Window::render()
{
  al_flip_display();
}
