/*
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
*/
#pragma once
#include "Defs.h"
#include "Vector.h"

void camera_rotate_around_axis(Camera* c, Vector axis, double radians);

/* Move the camera along its x axis and z axis (which corresponds to
 * right and backwards directions).
 */
void camera_move_along_direction(Camera* camera, double right,
  double forward);

/* Get a vector with y = 0 looking in the opposite direction as the camera z
 * axis. If looking straight up or down returns a 0 vector instead.
 */
Vector get_ground_forward_vector(Camera* camera);

/* Get a vector with y = 0 looking in the same direction as the camera x axis.
 * If looking straight up or down returns a 0 vector instead.
 */
Vector get_ground_right_vector(Camera* camera);

/* Like camera_move_along_direction but moves the camera along the ground plane
 * only.
 */
void camera_move_along_ground(Camera* camera, double right,
  double forward);

/* Calculate the pitch of the camera. This is the angle between the z axis
 * vector and our direction vector on the y = 0 plane.
 */
double get_pitch(Camera* c);

/* Calculate the yaw of the camera. This is basically the compass direction.
 */
double get_yaw(Camera* c);

/* Calculate the roll of the camera. This is the angle between the x axis
 * vector and its project on the y = 0 plane.
 */
double get_roll(Camera* c);

/* Set up a perspective transform. We make the screen span
 * 2 vertical units (-1 to +1) with square pixel aspect and the camera's
 * vertical field of view. Clip distance is always set to 1.
 */
void setup_3d_projection(float vertical_field_of_view = ALLEGRO_PI / 2);