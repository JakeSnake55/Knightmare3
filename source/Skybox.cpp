#include "Skybox.h"
#include <random>
#include <time.h>
#include "Vector.h"
#include "Camera.h"
#include "TimeHandler.h"
#include "DrawCalls.h"
#include "Settings.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>




Skybox::Skybox() {
    loadSeed(165902765245633);
}

void Skybox::loadSeed(int seed) {
    gen.seed(seed);
    starList[starCount] = {};
    moonSpotsList[spotsCount] = {};
    sunSpotsList[spotsCount] = {};
    StarPositioner();
}


/* Preposition stars to avoid doing huge amounts of random generation each frame
*/
void Skybox::StarPositioner() {
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
    first = true;
}


/* Create a skybox. Sun and Moon, stars and colour of sky depends on angle from sun.
 */
void Skybox::add_skybox(Window& window, Project &km, int &startSky, int &endSky) {
    
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

        float distance = sqrt((pow(starList[i].x / 50.0 + light.x, 2) + pow(starList[i].y / 50.0 - light.y, 2)) * 2.5f) - 1.5f;
        if (first) {
            float scale = starList[i].s;
            add_quad((double)p.x - starList[i].x - right.x * scale / 2 - up.x * scale / 2, (double)p.y + starList[i].y - right.y * scale / 2 - up.x * scale / 2, (double)p.z - starList[i].z - right.z * scale / 2 - up.x * scale / 2,
                (double)scale * right.x, (double)scale * right.y, (double)scale * right.z,
                (double)scale * up.x, (double)scale * up.y, (double)scale * up.z,
                al_map_rgba_f(1, 1, 1, distance), al_map_rgba_f(1, 1, 1, distance), window, km.general);
        }
        else {
            km.general.v[i].color.a = distance;
        }
    }
    first = false;

    // Moon 
    add_quad((double)p.x - light.x * 50 - 5.0 * right.x - 5.0 * up.x, (double)p.y - light.y * 50 - 5.0 * right.y - 5.0 * up.y, (double)p.z - 5.0 * right.z - 5.0 * up.z,
        (double)right.x * 10, (double)right.y * 10, (double)right.z * 10,
        (double)up.x * 10, (double)up.y * 10, (double)up.z * 10,
        whiteColour, whiteColour, window, km.general);

    // Sun 
    add_quad(p.x + light.x * 50 - 7.0 * right.x - 7.0 * up.x, p.y + light.y * 50 - 7.0 * right.y - 7.0 * up.y, p.z - 7.0 * right.z - 7.0 * up.z,
        right.x * 14.0, right.y * 14.0, right.z * 14.0,
        up.x * 14.0, up.y * 14.0, up.z * 14.0,
        orangishYellow, orangishYellow, window, km.general);

    add_quad(p.x + light.x * 50 - 6.0 * right.x - 6.0 * up.x, p.y + light.y * 50 - 6.0 * right.y - 6.0 * up.y, p.z - 6.0 * right.z - 6.0 * up.z,
        right.x * 12.0, right.y * 12.0, right.z * 12.0,
        up.x * 12.0, up.y * 12.0, up.z * 12.0,
        whiteColour, whiteColour, window, km.general);


    //Moon and Sunspots based on imaginary pixels, distributed across the celestial bodies from a pregenerated list of positions and sizes
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
            grayShading, grayShading, window, km.general);

        add_quad(p.x + light.x * 50 + up.x * (sunY - 6.0) + right.x * (sunX - 6.0), p.y + light.y * 50 + up.y * (sunY - 6.0) + right.y * (sunX - 6.0), p.z + up.z * (sunY - 6.0) + right.z * (sunX - 6.0),
            (double)right.x * sunSize, (double)right.y * sunSize, (double)right.z * sunSize,
            (double)up.x * sunSize, (double)up.y * sunSize, (double)up.z * sunSize,
            orangishYellow, orangishYellow, window, km.general);
    }
    endSky = km.general.n;
}
