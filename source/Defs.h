#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <math.h>

#ifdef linux
#define strcpy_s(x,y) strcpy(x,y)
#endif


#define pi ALLEGRO_PI

typedef struct {
    float r, g, b, a;
} Colour;

typedef struct {
    float x, y, z;
} Vector;

typedef struct {
    float x, y, z, s;
} Stars;

typedef struct {
    float x, y, s;
} Orbital;


typedef struct {
    Vector position;
    Vector xaxis; /* This represent the direction looking to the right. */
    Vector yaxis; /* This is the up direction. */
    Vector zaxis; /* This is the direction towards the viewer ('backwards'). */
    double vertical_field_of_view; /* In radians. */
} Camera;

struct VoxelSet{
    size_t n, v_size;
    ALLEGRO_VERTEX* v;
    VoxelSet():n(0),v_size(0),v(0){};
};

typedef struct {
    Camera camera;

    /* controls sensitivity */
    double mouse_look_speed;
    double movement_speed;

    /* keyboard and mouse state */
    int button[10];
    bool key[ALLEGRO_KEY_MAX];
    bool keystate[ALLEGRO_KEY_MAX];
    int mouse_dx, mouse_dy;

    /* control scheme selection */
    int controls;
    char const* controls_names[3];

    VoxelSet general;

    /* used to draw some info text */
    ALLEGRO_FONT* font;

    /* if not NULL the skybox picture to use */
    ALLEGRO_BITMAP* skybox;
} Project;

enum Material
{
    NOBLOCK,
    STONE
};

typedef struct {
    Colour colour;
    Material material;
} Terrain;

