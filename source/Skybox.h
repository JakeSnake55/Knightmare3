#pragma once
#include <random>
#include <time.h>
#include "Vector.h"
#include "Camera.h"
#include "TimeHandler.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>





Project km;



const int starCount = 100;
const int spotsCount = 50;
static Stars starList[starCount];
static Orbital moonSpotsList[spotsCount];
static Orbital sunSpotsList[spotsCount];

std::mt19937 gen(165902765245633);

int startSky = 0;
int endSky = 0;

/* Adds a new vertex to our scene. */
static void add_vertex(double x, double y, double z,
    ALLEGRO_COLOR color, VoxelSet& vertexList = km.general)
{

    int i = vertexList.n++;
    if (i >= vertexList.v_size) {
        vertexList.v_size += 1;
        vertexList.v_size *= 2;
        ALLEGRO_VERTEX* tmp = (ALLEGRO_VERTEX*)realloc(vertexList.v, vertexList.v_size * sizeof * vertexList.v);
        if (tmp != NULL) {

            vertexList.v = tmp;
        }
    }
    vertexList.v[i].x = x;
    vertexList.v[i].y = y;
    vertexList.v[i].z = z;
    vertexList.v[i].color = color;

}

/* Adds two triangles (6 vertices) to the scene. With gradient colours */
static void add_quad(double x, double y, double z,
    double ux, double uy, double uz,
    double vx, double vy, double vz,
    ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, VoxelSet& vertexList = km.general)
{
    if (!false)
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

/* Preposition stars to avoid doing huge amounts of random generation each frame
*/
static void StarPositioner() {
    std::uniform_real_distribution<> distribute(0, 1);
    std::uniform_real_distribution<> randSize(0.2, 3);
    std::uniform_int_distribution<> randStarLocation(0, 100);
    for (int i = 0; i < starCount; i++) {
        int x = randStarLocation(gen);
        int y = randStarLocation(gen);
        starList[i].x = (sin(x) * 50);
        starList[i].y = (cos(x) * cos(y) * 50);
        starList[i].z = (sin(y) * 50);
        starList[i].s = (randStarLocation(gen) / 200.0) + 0.3;
    }

    for (int i = 0; i < spotsCount; i++) {
        moonSpotsList[i].s = randSize(gen);
        moonSpotsList[i].x = (10 - (moonSpotsList[i].s / 2)) * distribute(gen);
        moonSpotsList[i].y = (10 - (moonSpotsList[i].s / 2)) * distribute(gen);
        sunSpotsList[i].s = (randSize(gen) * 2.0);
        sunSpotsList[i].x = (11 - (sunSpotsList[i].s / 2)) * distribute(gen);
        sunSpotsList[i].y = (11 - (sunSpotsList[i].s / 2)) * distribute(gen);
    }
}
/* Create a skybox. Sun and Moon, stars and colour of sky depends on angle from sun.
 */
static void add_skybox(void) {

    startSky = km.general.n;

    Vector p = km.camera.position;
    ALLEGRO_COLOR whiteColour = al_map_rgba(255, 255, 255, 255);
    ALLEGRO_COLOR grayShading = al_map_rgba(0, 0, 0, 60);
    ALLEGRO_COLOR orangishYellow = al_map_rgba(228, 208, 10, 60);

    /* Rotations lookup
    */
    float angle = (((float)2 * pi) / (float)1800) * (timehandler::elapsedTime + timehandler::unlinkedTime);
    Vector light = { sin(angle),cos(angle),0 };

    Vector right = km.camera.xaxis;
    Vector up = km.camera.yaxis;
    Vector back = km.camera.zaxis;
    //sky colouring
    double skyLighting = sqrt(pow(back.x - light.x, 2) + pow(back.y - light.y, 2) + pow(back.z, 2)) / 3.0;
    ALLEGRO_COLOR sky = al_map_rgb_f(pow(skyLighting, 2) / 5.0, pow(skyLighting, 2) * 5.0 / 9.0, skyLighting);
    al_clear_to_color(sky);

    /* Stars randomly distributed, no motion, should fade out near sun or moon*/

    for (int i = 0; i < starCount; i++) {

        float scale = starList[i].s;
        float distance = sqrt((pow(starList[i].x / 50.0 + light.x, 2) + pow(starList[i].y / 50.0 - light.y, 2)) * 2.5f) - 1.5f;
        add_quad((double)p.x - starList[i].x - right.x * scale / 2 - up.x * scale / 2, (double)p.y + starList[i].y - right.y * scale / 2 - up.x * scale / 2, (double)p.z - starList[i].z - right.z * scale / 2 - up.x * scale / 2,
            (double)scale * right.x, (double)scale * right.y, (double)scale * right.z,
            (double)scale * up.x, (double)scale * up.y, (double)scale * up.z,
            al_map_rgba_f(1, 1, 1, distance), al_map_rgba_f(1, 1, 1, distance));
    }

    // Moon 
    add_quad((double)p.x - light.x * 50 - 5.0 * right.x - 5.0 * up.x, (double)p.y - light.y * 50 - 5.0 * right.y - 5.0 * up.y, (double)p.z - 5.0 * right.z - 5.0 * up.z,
        (double)right.x * 10, (double)right.y * 10, (double)right.z * 10,
        (double)up.x * 10, (double)up.y * 10, (double)up.z * 10,
        whiteColour, whiteColour);

    // Sun 
    add_quad(p.x + light.x * 50 - 7.0 * right.x - 7.0 * up.x, p.y + light.y * 50 - 7.0 * right.y - 7.0 * up.y, p.z - 7.0 * right.z - 7.0 * up.z,
        right.x * 14.0, right.y * 14.0, right.z * 14.0,
        up.x * 14.0, up.y * 14.0, up.z * 14.0,
        orangishYellow, orangishYellow);

    add_quad(p.x + light.x * 50 - 6.0 * right.x - 6.0 * up.x, p.y + light.y * 50 - 6.0 * right.y - 6.0 * up.y, p.z - 6.0 * right.z - 6.0 * up.z,
        right.x * 12.0, right.y * 12.0, right.z * 12.0,
        up.x * 12.0, up.y * 12.0, up.z * 12.0,
        whiteColour, whiteColour);


    //Moon and Sunspots based on imaginary pixels, distibuted across the celestial bodies from a pregenerated list of positions and sizes
    for (int i = 0; i < 50; i++) {
        int size = moonSpotsList[i].s;
        int x = moonSpotsList[i].x;
        int y = moonSpotsList[i].y;
        int sunSize = sunSpotsList[i].s;
        int sunX = sunSpotsList[i].x;
        int sunY = sunSpotsList[i].y;

        add_quad(p.x - light.x * 50 + up.x * (y - 5.0) + right.x * (x - 5.0), p.y - light.y * 50 + up.y * (y - 5.0) + right.y * (x - 5.0), p.z + up.z * (y - 5.0) + right.z * (x - 5.0),
            (double)right.x * size, (double)right.y * size, (double)right.z * size,
            (double)up.x * size, (double)up.y * size, (double)up.z * size,
            grayShading, grayShading);

        add_quad(p.x + light.x * 50 + up.x * (sunY - 6.0) + right.x * (sunX - 6.0), p.y + light.y * 50 + up.y * (sunY - 6.0) + right.y * (sunX - 6.0), p.z + up.z * (sunY - 6.0) + right.z * (sunX - 6.0),
            (double)right.x * sunSize, (double)right.y * sunSize, (double)right.z * sunSize,
            (double)up.x * sunSize, (double)up.y * sunSize, (double)up.z * sunSize,
            orangishYellow, orangishYellow);
    }
    endSky = km.general.n;
}