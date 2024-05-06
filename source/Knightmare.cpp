/* Derived from an example demonstrating how to use ALLEGRO_TRANSFORM to represent a 3D
 * camera.
 */
#include "Defs.h"
#include "Vector.h"
#include "Knightmare.h"
#include <random>
#include <time.h>
#include <cmath>

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
  queue = window.queue;

  ALLEGRO_BITMAP* icon[3] = { al_load_bitmap("Resources/ml48x.png"), al_load_bitmap("Resources/ml32x.png"),
                              al_load_bitmap("Resources/ml16x.png") };
  if (icon)
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
    startSky = km.general.n;
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
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
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
  timer = al_create_timer(1.0 / 60.0);
  al_register_event_source(queue, al_get_timer_event_source(timer));
  km.camera.xaxis.x = 1;
  km.camera.yaxis.y = 1;
  km.camera.zaxis.z = 1;
  km.camera.position.y = 16;
  km.camera.vertical_field_of_view = 60 * pi / 180;

  km.mouse_look_speed = 0.03;
  km.movement_speed = 0.10 * 4;

  km.font = al_create_builtin_font();

  km.general = {};

  sky.add_skybox(window, km, startSky, endSky);
  al_start_timer(timer);
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
  if (al_peek_next_event(queue, &window.event)) {
    window.getEvent();
    ImGui_ImplAllegro5_ProcessEvent(&window.event);
    switch (window.event.type) {
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      window.cleanExit();
      exit(0);
    case ALLEGRO_EVENT_DISPLAY_RESIZE:
      ImGui_ImplAllegro5_InvalidateDeviceObjects();
      al_acknowledge_resize(display);
      ImGui_ImplAllegro5_CreateDeviceObjects();
      break;
    case ALLEGRO_EVENT_KEY_DOWN:
      km.key[window.event.keyboard.keycode] = true;
      km.keystate[window.event.keyboard.keycode] = true;
      break;

    case ALLEGRO_EVENT_KEY_UP:
      /* In case a key gets pressed and immediately released, we will still
       * have set ex.key so it is not lost.
       */
      km.keystate[window.event.keyboard.keycode] = false;
      break;
    case ALLEGRO_EVENT_TIMER:

      if (window.settings.keyboardSleep)
        handle_input();

      redraw = true;

      /* Reset keyboard state for keys not held down anymore. */
      for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        if (km.keystate[i] == 0)
          km.key[i] = 0;
      }

      km.mouse_dx = 0;
      km.mouse_dy = 0;
      if (window.settings.turnCamera) {
        if (mouseSleepUpdate) {
          al_hide_mouse_cursor(display);
          mouseSleepUpdate = false;
        }
        al_set_mouse_xy(display, display_width / 2, display_height / 2);
      }
      else {
        al_show_mouse_cursor(display);
      }
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      km.button[window.event.mouse.button] = true;
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
      km.button[window.event.mouse.button] = false;
      break;
    case ALLEGRO_EVENT_MOUSE_AXES:
      km.mouse_dx += window.event.mouse.dx;
      km.mouse_dy += window.event.mouse.dy;
      break;
    }

    if (redraw && al_is_event_queue_empty(queue)) {
      draw_scene();
      redraw = false;
    }

  }
}

int main(int argc, char** argv)
{
  knightmare kn;
}