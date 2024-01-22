#pragma once
#include <allegro5/allegro5.h>
#include "Defs.h"
#include "Window.h"

void add_vertex(double x, double y, double z,
    ALLEGRO_COLOR color, VoxelSet& vertexList);

void add_quad(double x, double y, double z,
    double ux, double uy, double uz,
    double vx, double vy, double vz,
    ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, Window& window, VoxelSet& vertexList);