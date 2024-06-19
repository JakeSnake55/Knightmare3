#pragma once
#include <allegro5/allegro5.h>
#include "Defs.h"
#include "Window.h"

/*
 function: add_vertex
 description: Adds a new vertex to the vertexList
 pre: vertexList is in a valid state,
 3d projection is in use.
 a position in x y and z with a color for the vertex are passed in
 post: vertexList is dynamically reallocated if the number of vertices is larger than the currently allocated space.
 if realloc fails to allocate the memory, it will exit with code 3.
*/
void add_vertex(double x, double y, double z,
  ALLEGRO_COLOR color, VoxelSet& vertexList);

void add_quad(double x, double y, double z,
  double ux, double uy, double uz,
  double vx, double vy, double vz,
  ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, Window& window, VoxelSet& vertexList);