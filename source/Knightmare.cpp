/* Derived from an example demonstrating how to use ALLEGRO_TRANSFORM to represent a 3D
 * camera.
 */
#include "Defs.h"
#include "Vector.h"
#include "Knightmare.h"
#include <random>
#include <time.h>
#include <cmath>
#include <iostream>

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_allegro5.h"

#include "TimeHandler.h"
#include "PerlinNoiseGenerator.h"
#include "Window.h"

#include "Camera.h"
#include "DrawCalls.h"
#include "Skybox.h"

Skybox sky;
double pitchie = 0;

Project km;


bool mouseSleepUpdate = true;
bool closeGame = false;

int startSky = 0;
int endSky = 0;

knightmare::knightmare() {
  redraw = true;
  
  initializeScenes();
  display = window.display;
  createEventQueue();

  ALLEGRO_BITMAP* icon[3] = { nullptr };
  icon[0] = al_load_bitmap("Resources/ml48x.png");
  icon[1] = al_load_bitmap("Resources/ml32x.png");
  icon[2] = al_load_bitmap("Resources/ml16x.png");
  if (icon[0])
    al_set_display_icons(display, 1, icon);

  ALLEGRO_MONITOR_INFO info;
  al_get_monitor_info(al_get_num_video_adapters() - 1, &info);

  display_height = info.y2 - info.y1;
  display_width = info.x2 - info.x1;
  setup_scene();
  while (true) {
    gameLoop();
  }
}

void knightmare::createEventQueue()
{
  queue = al_create_event_queue();
  al_register_event_source(queue, al_get_display_event_source(display));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_mouse_event_source());
}

void knightmare::draw_windows() {
  if (window.settings.showOptionsMenu) {
    window.buildOptionMenu();
  }
  if (window.settings.showMainMenu) {
    window.buildMainMenu();
  }
  if (window.settings.makeNewWorld) {
    window.buildWorldCreationMenu(window.settings.currentId);
  }
#ifdef DEBUG  //Only for Developers
  window.buildDebugWindow();

  if (window.settings.showDemoWindow) {
    ImGui::ShowDemoWindow(&window.settings.showDemoWindow);
  }
#endif

}


void knightmare::draw_scene()
{
  ImGui_ImplAllegro5_NewFrame();

  if (window.settings.drawNewSkybox) {
    sky.loadSeed(window.world[window.settings.currentId].seed);
    window.settings.drawNewSkybox = false;
  }


  Camera* c = &km.camera;
  Vector p = km.camera.position;
  /* We save Allegro's projection so we can restore it for drawing text. */
  ALLEGRO_TRANSFORM projection = *al_get_current_projection_transform();
  ALLEGRO_DISPLAY* display = window.display;
  ALLEGRO_TRANSFORM t;

  ALLEGRO_COLOR front = al_color_name("white");
  ALLEGRO_COLOR back = al_map_rgba(0, 0, 0, 128);

  ImGui::NewFrame();//Must be before Imgui windows are drawn.
  draw_windows();
  ImGui::Render(); //Must end before primitives are drawn.
  setup_3d_projection(window.settings.FOV);

  if (window.settings.drawSkybox)
  {
    sky.add_skybox(window, km, startSky, endSky);
  }
  else {
    al_clear_to_color(back);
  }

  if (window.settings.drawSkybox || window.settings.drawTerrain) {
    al_build_camera_transform(&t,
      km.camera.position.x, km.camera.position.y, km.camera.position.z,
      km.camera.position.x - km.camera.zaxis.x,
      km.camera.position.y - km.camera.zaxis.y,
      km.camera.position.z - km.camera.zaxis.z,
      km.camera.yaxis.x, km.camera.yaxis.y, km.camera.yaxis.z);
    al_use_transform(&t);
  }

  if (window.settings.drawSkybox) {
    if (!window.settings.wireFrame)
    {
      al_draw_prim(km.general.v, NULL, NULL, startSky, endSky, ALLEGRO_PRIM_TRIANGLE_LIST);
    }
    else {
      al_draw_prim(km.general.v, NULL, NULL, startSky, endSky, ALLEGRO_PRIM_LINE_LIST);
    }
  }
  km.general.n = 0;
  ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
  window.render();

}


void knightmare::setup_scene()
{
  km.camera.xaxis.x = 1;
  km.camera.yaxis.y = 1;
  km.camera.zaxis.z = 1;
  km.camera.position.y = 16;
  km.camera.vertical_field_of_view = 60 * pi / 180;

  km.mouse_look_speed = 0.03;
  km.movement_speed = 0.10 * 4;

  km.font = al_create_builtin_font();

  km.general = {};

  window.time.refresh = al_get_display_refresh_rate(display);

  sky.add_skybox(window, km, startSky, endSky);
}

void knightmare::playerMotion(double x, double y, double z) {

  /* Move the camera, along the ground axis. */
  double distance = sqrt(x * x + y * y + z * z);//distance of motion
  if (distance > 0) {
    x /= distance;
    y /= distance;
    z /= distance;

    camera_move_along_ground(&km.camera, km.movement_speed * x,
      km.movement_speed * y);
    km.camera.position.y += z * km.movement_speed;
    //camera_move_along_direction
    //^^ could be used to make a flying thing
  }

  /* Rotate the camera, either freely or around world up only. */
  if (window.settings.turnCamera) {
    Vector up = { 0,1,0 };
    pitchie -= km.mouse_look_speed * km.mouse_dy;
    camera_rotate_around_axis(&km.camera, km.camera.xaxis, -km.mouse_look_speed * km.mouse_dy);
    float limit = 1.5;
    if (pitchie < -limit) {
      camera_rotate_around_axis(&km.camera, km.camera.xaxis,
        -(pitchie + limit));
      pitchie += -(pitchie + limit);
    }
    if (pitchie > limit) {
      camera_rotate_around_axis(&km.camera, km.camera.xaxis,
        -(pitchie - limit));
      pitchie += -(pitchie - limit);
    }
    camera_rotate_around_axis(&km.camera, up, -km.mouse_look_speed * km.mouse_dx);
  }
}

void knightmare::handle_input()
{
  ImGuiIO io = ImGui::GetIO();
  double x = 0, y = 0, z = 0;

  if (io.KeysDown[ALLEGRO_KEY_A] || io.KeysDown[ALLEGRO_KEY_LEFT]) x--;
  if (io.KeysDown[ALLEGRO_KEY_S] || io.KeysDown[ALLEGRO_KEY_DOWN]) y--;
  if (io.KeysDown[ALLEGRO_KEY_D] || io.KeysDown[ALLEGRO_KEY_RIGHT]) x++;
  if (io.KeysDown[ALLEGRO_KEY_W] || io.KeysDown[ALLEGRO_KEY_UP]) y++;
  if (io.KeysDown[ALLEGRO_KEY_SPACE]) z++;
  if (io.KeysDown[ALLEGRO_KEY_LSHIFT] || io.KeysDown[ALLEGRO_KEY_RSHIFT]) z--;

  /* Change field of view with Z/X. */
  if (io.KeysDown[ALLEGRO_KEY_ESCAPE]) {
    window.cleanExit();
    exit(0);
  }
  if (io.KeysDown[ALLEGRO_KEY_Z]) {
    double m = 20 * pi / 180;
    km.camera.vertical_field_of_view -= 0.01;
    if (km.camera.vertical_field_of_view < m)
      km.camera.vertical_field_of_view = m;
  }
  if (io.KeysDown[ALLEGRO_KEY_X]) {
    double m = 120 * pi / 180;
    km.camera.vertical_field_of_view += 0.01;
    if (km.camera.vertical_field_of_view > m)
      km.camera.vertical_field_of_view = m;
  }
  if (io.KeysDown[ALLEGRO_KEY_T]) {
    window.settings.keyboardSleep = true;
  }
  if (io.KeysDown[ALLEGRO_KEY_P]) {
    window.settings.turnCamera = true;
  }
  if (io.KeysDown[ALLEGRO_KEY_U]) {
    window.settings.turnCamera = false;
    mouseSleepUpdate = true;
  }
  if (io.KeysDown[ALLEGRO_KEY_F]) {
    window.time.fastmode = true;
  }
  if (io.KeysDown[ALLEGRO_KEY_O]) {
    window.time.tickRate = 60;
    window.time.fastmode = false;
  }
  if (io.KeysDown[ALLEGRO_KEY_I]) {
    window.time.tickRate = 20;
    window.time.fastmode = false;
  }

  playerMotion(x, y, z);
}


void knightmare::initializeScenes()
{

  //al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  //al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  //al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST);
  al_set_new_display_flags(ALLEGRO_RESIZABLE);
  al_set_new_display_flags(ALLEGRO_FULLSCREEN);
  al_set_new_display_flags(ALLEGRO_NOFRAME);
  al_set_new_display_flags(ALLEGRO_GENERATE_EXPOSE_EVENTS);
  al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

}

void knightmare::gameLoop() {
  window.time.changeTime();
  window.time.changeWorldTime();
  
  if (!al_is_event_queue_empty(queue)) {
    al_get_next_event(queue, &ev);
    ImGui_ImplAllegro5_ProcessEvent(&ev);
    switch (ev.type) {
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      al_destroy_event_queue(queue);
      window.cleanExit();
      exit(0);
    case ALLEGRO_EVENT_DISPLAY_RESIZE:
      ImGui_ImplAllegro5_InvalidateDeviceObjects();
      al_acknowledge_resize(display);
      ImGui_ImplAllegro5_CreateDeviceObjects();
      break;
    case ALLEGRO_EVENT_KEY_DOWN:
      km.key[ev.keyboard.keycode] = true;
      km.keystate[ev.keyboard.keycode] = true;
      break;

    case ALLEGRO_EVENT_KEY_UP:
      /* In case a key gets pressed and immediately released, we will still
       * have set ex.key so it is not lost.
       */
      km.keystate[ev.keyboard.keycode] = false;
      break;
   
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      km.button[ev.mouse.button] = true;
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
      km.button[ev.mouse.button] = false;
      break;
    case ALLEGRO_EVENT_MOUSE_AXES:
      km.mouse_dx += ev.mouse.dx;
      km.mouse_dy += ev.mouse.dy;
      break;
    }
    
  }
  if (window.time.timer) {
    if (window.settings.keyboardSleep)
      handle_input();

    

    if (mouseSleepUpdate) {
      if (window.settings.turnCamera) {
        al_hide_mouse_cursor(display);
        mouseSleepUpdate = false;
      }
      else {
        al_show_mouse_cursor(display);
      }
    }
    km.mouse_dx = 0;
    km.mouse_dy = 0;
    if (window.settings.turnCamera) {
      al_set_mouse_xy(display, display_width / 2, display_height / 2);
    }
   
    redraw = true;
  }

  if (redraw ) {
    draw_scene();
    window.time.renderLog();
    redraw = false;
  }
  if (al_is_event_queue_empty(queue)) {
    window.time.processLog();
    if (window.settings.waitForVSync) {
      al_wait_for_vsync();
    }
    window.time.vsyncLog();
  }
  
}

void installs()
{
  if (!al_init()) {
    std::cerr << __FILE__ << " : " << __LINE__ << " Allegro was not found, check linker settings or whether it is installed. Clean exit." << std::endl;
    std::exit(1);
  }

#ifndef IMGUI_VERSION
  std::cerr << __FILE__ << " : " << __LINE__ << " ImGui was not found, check linker settings or whether it is installed. Clean exit." << std::endl;
  std::exit(2);
#endif // !IMGUI_VERSION

  al_install_keyboard();
  al_install_mouse();
  al_init_primitives_addon();
  al_init_font_addon();
  al_init_image_addon();
}

int main(int argc, char** argv)
{
  installs();
  knightmare kn;
}