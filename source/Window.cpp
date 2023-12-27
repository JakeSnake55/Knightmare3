#include "Window.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include <sstream>

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_allegro5.h"
#include "Settings.h"
#include "World.h"
#include "Files.h"


Window::Window() 
{
    world.resize(0);
    installs();
    createWindow();
    createEventQueue();
    setupImgui();
}

//Builds the Debug window for developer use only
void Window::buildDebugWindow()
{
    ImGui::Begin("Debug");

    ImGui::Checkbox("Demo Window", &settings.showDemoWindow);//Shows what is possible with ImGui
    ImGui::Checkbox("VSync", &settings.waitForVSync);//Pauses frames to achieve VSync
    ImGui::Checkbox("Primitives", &settings.drawPrimitives);//Draw triangles with allegro
    ImGui::Checkbox("Activate Camera", &settings.turnCamera);//Draw triangles with allegro
    ImGui::SliderFloat("FOV", &settings.FOV,1,180,"%.3f");
    ImGui::SliderFloat("Zoom", &settings.zoom, 0, 10, "%.3f");
    ImGui::SliderFloat("X", &settings.x, 0, 2*ALLEGRO_PI, "%.3f");
    ImGui::SliderFloat("Y", &settings.y, 0, 2 * ALLEGRO_PI, "%.3f");
    ImGui::Text("view = %f, %f", camera.cameraPitch(), camera.cameraYaw());
    ImGui::End();//end a ImGui definition like this always
}

std::string append_number(std::string const& x, unsigned int num, char sep = '_') {
  std::stringstream s;
  s << x << sep << num;
  return s.str();
}//util for label names

void Window::buildMainMenu() 
{
   
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

    // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
    // Based on your use case you may want one or the other.
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos( viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    if (ImGui::Begin("Main Menu",NULL, flags))
    {

        if (ImGui::Button("New World")) 
        {
          settings.showMainMenu = false;
          settings.makeNewWorld = true;
          world.resize(world.size() + 1);
        }
        ImGui::SameLine();

        if (ImGui::Button("Delete selected")) {
          ImGui::OpenPopup("Delete.");
        }
          
        
        if (ImGui::BeginPopupModal("Delete.", NULL, ImGuiWindowFlags_AlwaysAutoResize)) 
        {
          ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
          ImGui::Separator();

          //static int unused_i = 0;
          //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

          static bool dont_ask_me_next_time = false;
          ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
          ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
          ImGui::PopStyleVar();

          if (ImGui::Button("OK", ImVec2(120, 0)))
          { 
            for (int i = 0; i < world.size(); i++) {
              if (world[i].selected) {
                world.erase(world.begin() + i);
                i--;
              }
            }
            ImGui::CloseCurrentPopup(); 
          }
          ImGui::SetItemDefaultFocus();
          ImGui::SameLine();
          if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
          ImGui::EndPopup();
        }
        
        for (int i = 0; i < world.size(); i++) {
          
          ImGui::Text(world[i].name.c_str());
          ImGui::OpenPopupOnItemClick(append_number("##world", i).c_str(), ImGuiPopupFlags_MouseButtonLeft);
          ImGui::SameLine();
          ImGui::Checkbox(append_number("##select",i).c_str(), &world[i].selected);

          if (ImGui::BeginPopup(append_number("##world", i).c_str())) 
          {
            ImGui::Text(world[i].name.c_str());
            ImGui::SameLine();
            ImGui::Text(std::to_string(world[i].seed).c_str());
            if (ImGui::Button("play"))
            {

            }
            ImGui::SameLine();
            if (ImGui::Button("edit"))
            {
              settings.currentId = i;
              settings.makeNewWorld = true;
            }
            ImGui::EndPopup();
          }

          
        }
        
        ImGui::CheckboxFlags("ImGuiWindowFlags_NoBackground", &flags, ImGuiWindowFlags_NoBackground);
        ImGui::CheckboxFlags("ImGuiWindowFlags_NoDecoration", &flags, ImGuiWindowFlags_NoDecoration);
        ImGui::Indent();
        ImGui::CheckboxFlags("ImGuiWindowFlags_NoTitleBar", &flags, ImGuiWindowFlags_NoTitleBar);
        ImGui::CheckboxFlags("ImGuiWindowFlags_NoCollapse", &flags, ImGuiWindowFlags_NoCollapse);
        ImGui::CheckboxFlags("ImGuiWindowFlags_NoScrollbar", &flags, ImGuiWindowFlags_NoScrollbar);
        ImGui::Unindent();

        if (&settings.showMainMenu && ImGui::Button("Close this window")) {
            settings.showMainMenu = false;
            settings.turnCamera = true;
            al_set_mouse_xy(display, al_get_display_width(display) / 2, al_get_display_height(display) / 2);
        }
    }
    ImGui::End();
}

void Window::buildWorldCreationMenu(int id) {
  if (id == -1) {
    id = world.size() - 1;
  }
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);

  static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
  if (ImGui::Begin("World Menu", NULL, flags))
  {
    const int worldNameLength = 120;
    char temp[worldNameLength] ;
    strcpy_s(temp, world[id].name.c_str());
    ImGui::InputTextWithHint("##world_name", "World name", temp, worldNameLength);
    ImGui::Text("Seed");
    ImGui::SameLine();
    ImGui::InputInt("##world_seed", &world[id].seed);
    
    world[id].name = temp;

    if (&settings.makeNewWorld && ImGui::Button("Create world")) {
      file.addWorldFolder(world[id]);
      settings.makeNewWorld = false;
      settings.currentId = -1;
      settings.showMainMenu = true;//Just for now whilst there aren't really actual files or anything so I can see the menu and stuff.
    }
  }
  ImGui::End();
}


void Window::installs()
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
}


void Window::createWindow() 
{
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(1280, 720);
    al_set_window_title(display, "Knightmare 3.0");
}

void Window::createEventQueue() 
{
    //double FPS = 60.0;
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
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
    al_destroy_event_queue(queue);
    al_destroy_display(display);
}

bool Window::getEvent() 
{
    return al_get_next_event(queue, &event);;
}

void Window::render()
{
    ImGui::Render(); //needed to avoid crash, dunno what it does
    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
    al_flip_display();
    
}