#pragma once
#include <allegro5/allegro5.h>
#include <cstdint>
#include "Vector.h"


class Camera
{
private:



public:
    Vector forwards;
    Vector right;
    Vector upwards;
    Vector position;
    Vector velocity;
    Camera();

};

static void camera_rotate_around_axis(Camera* c, Vector axis, double radians)
{
    /*Vector move = vector_mul(c->forwards, -1);
    move.y = 0;
    Vector f = vector_normalize(move);
    double pitch = asin(vector_dot_product(f, c->upwards))*180/pi;
    */
    ALLEGRO_TRANSFORM t;
    al_identity_transform(&t);
    al_rotate_transform_3d(&t, axis.x, axis.y, axis.z, radians);
    al_transform_coordinates_3d(&t, &c->forwards.x, &c->forwards.y, &c->forwards.z);
    al_transform_coordinates_3d(&t, &c->right.x, &c->right.y, &c->right.z);


    /* Make sure the axes remain orthogonal to each other. */
    c->forwards = vector_normalize(c->forwards);
    c->right = vector_cross_product(c->upwards, c->forwards);
    c->right = vector_normalize(c->right);
    c->upwards = vector_cross_product(c->forwards, c->right);
}

/* Move the camera along its x axis and z axis (which corresponds to
 * right and backwards directions).
 */
static void camera_move_along_direction(Camera* camera, double right,
    double forward)
{
    vector_iadd(&camera->position, vector_mul(camera->right, right));
    vector_iadd(&camera->position, vector_mul(camera->forwards, -forward));
}

/* Get a vector with y = 0 looking in the opposite direction as the camera z
 * axis. If looking straight up or down returns a 0 vector instead.
 */
static Vector get_ground_forward_vector(Camera* camera)
{
    Vector move = vector_mul(camera->forwards, -1);
    move.y = 0;
    return vector_normalize(move);
}

/* Get a vector with y = 0 looking in the same direction as the camera x axis.
 * If looking straight up or down returns a 0 vector instead.
 */
static Vector get_ground_right_vector(Camera* camera)
{
    Vector move = camera->right;
    move.y = 0;
    return vector_normalize(move);
}

/* Like camera_move_along_direction but moves the camera along the ground plane
 * only.
 */
static void camera_move_along_ground(Camera* camera, double right,
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
static double get_pitch(Camera* c)
{
    Vector f = get_ground_forward_vector(c);
    return asin(vector_dot_product(f, c->upwards));
}

/* Calculate the yaw of the camera. This is basically the compass direction.
 */
static double get_yaw(Camera* c)
{
    return atan2(c->forwards.x, c->forwards.z);
}

/* Calculate the roll of the camera. This is the angle between the x axis
 * vector and its project on the y = 0 plane.
 */
static double get_roll(Camera* c)
{
    Vector r = get_ground_right_vector(c);
    return asin(vector_dot_product(r, c->upwards));
}
