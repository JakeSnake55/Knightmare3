#pragma once
#include "Window.h"
class knightmare {
private:
  bool redraw;
  int display_width;
  int display_height;
  ALLEGRO_DISPLAY* display;
  ALLEGRO_EVENT_QUEUE* queue;
  ALLEGRO_TIMER* timer;

  void draw_scene();
  void handle_input();
  void playerMotion(double x, double y, double z);
  void setup_scene();
  void draw_windows();
  void initializeScenes();

public:
  knightmare();
  Window window;
  void gameLoop();
};