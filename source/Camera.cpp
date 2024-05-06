#include "Camera.h"
/*

#include "knightmare3config.h"
#include <climits>
#include <cmath>

//Constructor
Camera::Camera()
{
	
	forwards = { 0,0,1 };
	right = { 1,0,0 };
	upwards = { 0,1,0 };
}
*/
#include "Defs.h"
#include "Vector.h"

/* Rotate the camera around the given axis. */
void camera_rotate_around_axis(Camera* c, Vector axis, double radians)
{
    ALLEGRO_TRANSFORM t;
    al_identity_transform(&t);
    al_rotate_transform_3d(&t, axis.x, axis.y, axis.z, radians);
    al_transform_coordinates_3d(&t, &c->yaxis.x, &c->yaxis.y, &c->yaxis.z);
    al_transform_coordinates_3d(&t, &c->zaxis.x, &c->zaxis.y, &c->zaxis.z);

    /* Make sure the axes remain orthogonal to each other. */
    c->zaxis = vector_normalize(c->zaxis);
    c->xaxis = vector_cross_product(c->yaxis, c->zaxis);
    c->xaxis = vector_normalize(c->xaxis);
    c->yaxis = vector_cross_product(c->zaxis, c->xaxis);
}

/* Move the camera along its x axis and z axis (which corresponds to
 * right and backwards directions).
 */
void camera_move_along_direction(Camera* camera, double right,
    double forward)
{
    vector_iadd(&camera->position, vector_mul(camera->xaxis, right));
    vector_iadd(&camera->position, vector_mul(camera->zaxis, -forward));
}

/* Get a vector with y = 0 looking in the opposite direction as the camera z
 * axis. If looking straight up or down returns a 0 vector instead.
 */
Vector get_ground_forward_vector(Camera* camera)
{
    Vector move = vector_mul(camera->zaxis, -1);
    move.y = 0;
    return vector_normalize(move);
}

/* Get a vector with y = 0 looking in the same direction as the camera x axis.
 * If looking straight up or down returns a 0 vector instead.
 */
Vector get_ground_right_vector(Camera* camera)
{
    Vector move = camera->xaxis;
    move.y = 0;
    return vector_normalize(move);
}

/* Like camera_move_along_direction but moves the camera along the ground plane
 * only.
 */
void camera_move_along_ground(Camera* camera, double right,
    double forward)
{
    Vector f = get_ground_forward_vector(camera);
    Vector r = get_ground_right_vector(camera);
    camera->position.x += f.x * forward + r.x * right;
    camera->position.z += f.z * forward + r.z * right;
}

/* Calculate the pitch of the camera. This is the angle between the z axis
 * vector and our direction vector on the y = 0 plane.
 */
double get_pitch(Camera* c)
{
    Vector f = get_ground_forward_vector(c);
    return asin(vector_dot_product(f, c->yaxis));
}

/* Calculate the yaw of the camera. This is basically the compass direction.
 */
double get_yaw(Camera* c)
{
    return atan2(c->zaxis.x, c->zaxis.z);
}

/* Calculate the roll of the camera. This is the angle between the x axis
 * vector and its project on the y = 0 plane.
 */
double get_roll(Camera* c)
{
    Vector r = get_ground_right_vector(c);
    return asin(vector_dot_product(r, c->yaxis));
}

/* Set up a perspective transform. We make the screen span
 * 2 vertical units (-1 to +1) with square pixel aspect and the camera's
 * vertical field of view. Clip distance is always set to 1.
 */
void setup_3d_projection(float vertical_field_of_view)
{
    ALLEGRO_TRANSFORM projection;
    ALLEGRO_DISPLAY* display = al_get_current_display();
    double dw = al_get_display_width(display);
    double dh = al_get_display_height(display);
    double f = (vertical_field_of_view/180.0)*pi;
    al_identity_transform(&projection);
    al_translate_transform_3d(&projection, 0, 0, -1);
    f = tan(f / 2);
    al_perspective_transform(&projection, -1 * dw / dh * f, f,
        1,
        f * dw / dh, -f, 10000000000000);//Check here later, draw distance? but also affects colours?
    al_use_projection_transform(&projection);
}