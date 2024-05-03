#include <allegro5/allegro5.h>
#include <iostream>
#include "Defs.h"
#include "Window.h"
#include "DrawCalls.h"



void add_vertex(double x, double y, double z,
    ALLEGRO_COLOR color, VoxelSet& vertexList)
{

    size_t i = vertexList.n++;
    if (i >= vertexList.v_size) {
        if (vertexList.v_size > ULONG_MAX / 2) {
          std::cerr <<__FILE__<<" : "<<__LINE__ << " very large amount of vertices, dirty exit."<<std::endl;
          exit(3);
        }
        vertexList.v_size += 1;
        vertexList.v_size *= 2;
        
        ALLEGRO_VERTEX* tmp = (ALLEGRO_VERTEX*)realloc(vertexList.v, vertexList.v_size * sizeof * vertexList.v);
        if (tmp == NULL) {
          std::cerr << __FILE__ << " : " << __LINE__ << " realloc failed, dirty exit."<<std::endl;
          exit(3);
        }
       
        vertexList.v = tmp;
    }
    vertexList.v[i].x = x;
    vertexList.v[i].y = y;
    vertexList.v[i].z = z;
    vertexList.v[i].color = color;

}

/* Adds two triangles (6 vertices) to the scene. With gradient colour


 */
void add_quad(double x, double y, double z,
    double ux, double uy, double uz,
    double vx, double vy, double vz,
    ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, Window& window, VoxelSet& vertexList)
{
    if (!window.settings.wireFrame)
    {
        add_vertex(x, y, z, c1, vertexList);
        add_vertex(x + ux, y + uy, z + uz, c1, vertexList);
        add_vertex(x + vx, y + vy, z + vz, c2, vertexList);
        add_vertex(x + vx, y + vy, z + vz, c2, vertexList);
        add_vertex(x + ux, y + uy, z + uz, c1, vertexList);
        add_vertex(x + ux + vx, y + uy + vy, z + uz + vz, c2, vertexList);
    }
    else {
        ALLEGRO_COLOR color = al_map_rgb_f(1, 1, 1);
        add_vertex(x, y, z, color, vertexList);
        add_vertex(x + ux, y + uy, z + uz, color, vertexList);
        add_vertex(x, y, z, color, vertexList);
        add_vertex(x + vx, y + vy, z + vz, color, vertexList);
        add_vertex(x + vx, y + vy, z + vz, color, vertexList);
        add_vertex(x + ux + vx, y + uy + vy, z + uz + vz, color, vertexList);
        add_vertex(x + ux, y + uy, z + uz, color, vertexList);
        add_vertex(x + ux + vx, y + uy + vy, z + uz + vz, color, vertexList);
        add_vertex(x, y, z, color, vertexList);
        add_vertex(x + ux + vx, y + uy + vy, z + uz + vz, color, vertexList);
    }

}
