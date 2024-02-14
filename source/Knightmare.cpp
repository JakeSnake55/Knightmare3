/* Derived from an example demonstrating how to use ALLEGRO_TRANSFORM to represent a 3D
 * camera.
 */
#include "Defs.h"
#include "Vector.h"
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
Project temp;


time_t timed;
time_t timing;

bool drawTerrain = false;



bool mouseSleepUpdate = true;
bool centGrav = false;
bool closeGame = false;

int startSky = 0;
int endSky = 0;

float storedX, storedY, storedZ;
int chunkX, chunkY, chunkZ;


VoxelSet loadedChunks = {};

VoxelSet generatedChunkArray[8][8][8] = {};

Terrain generated[16][16][16] = {};



/*Generate z height*/
int height_generator(int x, int y, int size, int chunkZ) {
    if (chunkZ < 0)
        return size;
    if (chunkZ > 0)
        return 0;
    return 3 + perlin((chunkY * size + y) * 0.1, (chunkX * size + x) * 0.1) * 3;
}

/* Generate Checkered cubes*/
static void chunk_generator(int chunkX = 0, int chunkY = 0, int chunkZ = 0)
{
    int z = 0;
    Terrain tmp = {};
    int size = 16;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            for (int j = 0; j < 16; j++) {
                generated[x][y][j].material = NOBLOCK;
            }
            z = height_generator(x, y, size, chunkZ);

            Colour stoneColour = { 0.5f,0.5f,0.5f,1 };

            for (int j = 0; (j < z && j < 16); j++) {
                generated[x][y][j].material = STONE;
                generated[x][y][j].colour = stoneColour;
            }
        }
    }
}

/* Create a 16x16x16 chunk made from vertex faces. Would like to implement Greedy Meshing here in the future. */
static void draw_chunk(Window &window, int x0 = 0, int y0 = 0, int z0 = 0)
{
    drawTerrain = true;
    bool fullCube = true;
    bool nullCube = false;
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                if (generated[x][y][z].material == NOBLOCK) {
                    fullCube = false;
                }
                if (generated[x][y][z].material != NOBLOCK) {
                    nullCube = false;
                }
            }
        }
    }
    if (fullCube) {
        ALLEGRO_COLOR COLOR = al_map_rgba_f(generated[0][0][0].colour.r * 0.98f, generated[0][0][0].colour.g * 0.98f, generated[0][0][0].colour.b * 0.98f, generated[0][0][0].colour.a);
        add_quad((double)x0 * 16, (double)z0 * 16, (double)y0 * 16,
            0, 0, 16,
            0, 16, 0,
            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);


        add_quad(16 + (double)x0 * 16, (double)z0 * 16, (double)y0 * 16,
            0, 0, 16,
            0, 16, 0,
            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);

        COLOR = al_map_rgba_f(generated[0][0][0].colour.r * 0.94f, generated[0][0][0].colour.g * 0.94f, generated[0][0][0].colour.b * 0.94f, generated[0][0][0].colour.a);

        add_quad((double)x0 * 16, (double)z0 * 16, (double)y0 * 16,
            16, 0, 0,
            0, 16, 0,
            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);


        add_quad((double)x0 * 16, (double)z0 * 16, 16 + (double)y0 * 16,
            16, 0, 0,
            0, 16, 0,
            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);

        COLOR = al_map_rgba_f(generated[0][0][0].colour.r * 0.90f, generated[0][0][0].colour.g * 0.90f, generated[0][0][0].colour.b * 0.90f, generated[0][0][0].colour.a);


        add_quad((double)x0 * 16, (double)z0 * 16, (double)y0 * 16,
            16, 0, 0,
            0, 0, 16,
            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);


        add_quad((double)x0 * 16, 16 + (double)z0 * 16, (double)y0 * 16,
            16, 0, 0,
            0, 0, 16,
            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);
        return;
    }
    if (nullCube) {
        return;
    }
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                if (generated[x][y][z].material != NOBLOCK) {
                    ALLEGRO_COLOR COLOR = al_map_rgba_f(generated[x][y][z].colour.r * 0.98f, generated[x][y][z].colour.g * 0.98f, generated[x][y][z].colour.b * 0.98f, generated[x][y][z].colour.a);
                    if ((x == 0) || (generated[x - 1][y][z].material == NOBLOCK)) {
                        add_quad(x + (double)x0 * 16, z + (double)z0 * 16, y + (double)y0 * 16,
                            0, 0, 1,
                            0, 1, 0,
                            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);
                    }
                    if ((x == 15) || (generated[x + 1][y][z].material == NOBLOCK)) {
                        add_quad(x + 1 + (double)x0 * 16, z + (double)z0 * 16, y + (double)y0 * 16,
                            0, 0, 1,
                            0, 1, 0,
                            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);
                    }
                    COLOR = al_map_rgba_f(generated[x][y][z].colour.r * 0.94f, generated[x][y][z].colour.g * 0.94f, generated[x][y][z].colour.b * 0.94f, generated[x][y][z].colour.a);
                    if ((y == 0) || (generated[x][y - 1][z].material == NOBLOCK)) {
                        add_quad(x + (double)x0 * 16, z + (double)z0 * 16, y + (double)y0 * 16,
                            1, 0, 0,
                            0, 1, 0,
                            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);
                    }
                    if ((y == 15) || (generated[x][y + 1][z].material == NOBLOCK)) {
                        add_quad((double)x + x0 * 16, (double)z + z0 * 16, (double)y + 1 + y0 * 16,
                            1, 0, 0,
                            0, 1, 0,
                            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);
                    }
                    COLOR = al_map_rgba_f(generated[x][y][z].colour.r * 0.90f, generated[x][y][z].colour.g * 0.90f, generated[x][y][z].colour.b * 0.90f, generated[x][y][z].colour.a);

                    if ((z == 0) || (generated[x][y][z - 1].material == NOBLOCK)) {
                        add_quad((double)x + x0 * 16, (double)z + z0 * 16, (double)y + y0 * 16,
                            1, 0, 0,
                            0, 0, 1,
                            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);
                    }
                    if ((z == 15) || (generated[x][y][z + 1].material == NOBLOCK)) {
                        add_quad((double)x + x0 * 16, (double)z + 1 + z0 * 16, (double)y + y0 * 16,
                            1, 0, 0,
                            0, 0, 1,
                            COLOR, COLOR, window, generatedChunkArray[x0][y0][z0]);
                    }
                }
            }
        }
    }
    drawTerrain = false;
}



static void buildChunks(Window &window) {
    int width = 8;
    chunkX = floor(km.camera.position.x / 16.0);
    chunkY = floor(km.camera.position.z / 16.0);
    chunkZ = floor(km.camera.position.y / 16.0);
    loadedChunks = {};
    bool exit = false;
    for (int i = -4; i < 0; i++) {
        for (int j = -4; j < 0; j++) {
            for (int d = -4; d < 0; d++) {
                if (true) {
                    chunk_generator(i + chunkX, j + chunkY, d + chunkZ);
                    draw_chunk(window, i + chunkX, j + chunkY, d + chunkZ);
                    exit = true;
                    
                }
            }
        }
    }
}



static void draw_scene(Window &window)
{
    
    if (window.settings.redrawChunks) {
        buildChunks(window);
        window.settings.redrawChunks = false;
    }
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
    int th;
    double yaw, roll;


    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();
    
    setup_3d_projection(window.settings.FOV);
    
    al_set_render_state(ALLEGRO_DEPTH_TEST, 0);

    al_clear_depth_buffer(1);

    /* We use a depth buffer. */
    if (window.settings.drawSkybox) 
    {
        km.general.n = startSky;
        sky.add_skybox(window, km, startSky, endSky);
    }
    


    /* Draw pregenerated Terrain
     *
     */
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

    /* Construct a transform corresponding to our camera. This is an inverse
     * translation by the camera position, followed by an inverse rotation
     * from the camera orientation.
     */
    al_clear_depth_buffer(1);
    al_set_render_state(ALLEGRO_DEPTH_TEST, 1);

   
    
    if (window.settings.drawTerrain) {
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                for (int d = 0; d < 8; d++) {
                    if (!window.settings.wireFrame)
                    {
                        al_draw_prim(generatedChunkArray[i][j][d].v, NULL, NULL, 0, generatedChunkArray[i][j][d].n, ALLEGRO_PRIM_TRIANGLE_LIST);
                    }
                    else {
                        al_draw_prim(generatedChunkArray[i][j][d].v, NULL, NULL, 0, generatedChunkArray[i][j][d].n, ALLEGRO_PRIM_LINE_LIST);
                    }
                }
            }
        }
    }

    if (window.settings.showOptionsMenu) {
        window.buildOptionMenu();
    }

    if (window.settings.showMainMenu) {
        window.buildMainMenu();
    }
    if (window.settings.makeNewWorld) {
        window.buildWorldCreationMenu(window.settings.currentId);
    }
#ifdef DEBUG { //Only for Developers
        window.buildDebugWindow();

        if (window.settings.showDemoWindow) {
            ImGui::ShowDemoWindow();
        }
#endif
    window.render();
}


static void setup_scene(Window &window)
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
    
    sky.add_skybox(window, km, startSky, endSky);

}

void playerMotion(double x, double y, double z, Window &window) {
    /* With CentGrav enabled, always move the camera to height 32(player height). */
    if (centGrav) {
        if (km.camera.position.y > 16)
            km.camera.position.y -= 0.16 * km.movement_speed;
        if (km.camera.position.y < 16)
            km.camera.position.y += 0.16 * km.movement_speed;
    }

    /* Move the camera, along the ground axis. */
    double xyz = sqrt(x * x + y * y + z * z);//distance of motion
    if (xyz > 0) {
        x /= xyz;
        y /= xyz;
        z /= xyz;

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

static void handle_input(Window &window)
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
        closeGame = true;
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
    if (ImGui::GetIO().KeysDown[ALLEGRO_KEY_P]) {
        window.settings.turnCamera = true;
    }
    if (io.KeysDown[ALLEGRO_KEY_U]) {
        window.settings.turnCamera = false;
        mouseSleepUpdate = true;
    }
    if (io.KeysDown[ALLEGRO_KEY_F]) {
        timehandler::tickRate = -1;
    }
    if (io.KeysDown[ALLEGRO_KEY_O]) {
        timehandler::tickRate = 60;
    }
    if (io.KeysDown[ALLEGRO_KEY_I]) {
        timehandler::tickRate = 20;
    }

    playerMotion(x, y, z, window);
}
void initializeScenes() {
    
    
    //init_platform_specific();
    

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST);
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    al_set_new_display_flags(ALLEGRO_NOFRAME);
    al_set_new_display_flags(ALLEGRO_GENERATE_EXPOSE_EVENTS);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

}

int main(int argc, char** argv)
{

    Window window;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue = window.queue;
    ALLEGRO_DISPLAY* display = window.display;
    ALLEGRO_MONITOR_INFO info;

    

    time(&timed);
    bool redraw = true;
    int fps = 60;
    //al_set_new_display_adapter(al_get_num_video_adapters() - 1);

    //initializeScenes();


    do {
        al_get_monitor_info(al_get_num_video_adapters() - 1, &info);
    } while (!(info.x1 == 0 && info.y1 == 0));
    int display_width = info.x2 - info.x1;
    int display_height = info.y2 - info.y1;
    
    ALLEGRO_BITMAP* icon[3] = { al_load_bitmap("Resources/ml16x.png"), al_load_bitmap("Resources/ml32x.png"),
                                al_load_bitmap("Resources/ml48x.png")};

    if (icon)
        al_set_display_icons(display, 1, icon);

    //printf("%d\n", al_get_num_video_adapters());

    timer = al_create_timer(1.0 / (double)fps);
   

    al_register_event_source(queue, al_get_timer_event_source(timer));
    setup_scene(window);
    al_start_timer(timer);


   
   

    while (true) {
        timehandler::changeTime();
        if (al_peek_next_event(queue, &window.event)) {
            window.getEvent();
            ImGui_ImplAllegro5_ProcessEvent(&window.event);
            if (window.event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                closeGame = true;
            else if (window.event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
                ImGui_ImplAllegro5_InvalidateDeviceObjects();
                al_acknowledge_resize(display);
                ImGui_ImplAllegro5_CreateDeviceObjects();
            }
            else if (window.event.type == ALLEGRO_EVENT_KEY_DOWN) {

                /*Set key and keystate of keycode.
                */
                km.key[window.event.keyboard.keycode] = true;
                km.keystate[window.event.keyboard.keycode] = true;
            }
            else if (window.event.type == ALLEGRO_EVENT_KEY_UP) {
                /* In case a key gets pressed and immediately released, we will still
                 * have set ex.key so it is not lost.
                 */
                km.keystate[window.event.keyboard.keycode] = false;
            }
            else if (window.event.type == ALLEGRO_EVENT_TIMER) {
                
                if (closeGame) {
                    break;
                }
                int i;
                if (window.settings.keyboardSleep)
                    handle_input(window);
                else {
                    
                }
                redraw = true;

                /* Reset keyboard state for keys not held down anymore. */
                for (i = 0; i < ALLEGRO_KEY_MAX; i++) {
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
            }

            else if (window.event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                km.button[window.event.mouse.button] = true;
            }
            else if (window.event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                km.button[window.event.mouse.button] = false;
            }
            else {
                if (window.event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                    km.mouse_dx += window.event.mouse.dx;
                    km.mouse_dy += window.event.mouse.dy;
                }
            }
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            draw_scene(window);
            window.render();
            redraw = false;
        }
    }
    window.cleanExit();
    return false;
}