# include "Vector.h"
# include <math.h>

/* Calculate the dot product between two vectors. This corresponds to the
 * angle between them times their lengths.
 */
static double vector_dot_product(Vector a, Vector b)
{
    return (double)a.x * b.x + (double)a.y * b.y + (double)a.z * b.z;
}

/* Calculate the cross product of two vectors. This produces a normal to the
 * plane containing the operands.
 */
static Vector vector_cross_product(Vector a, Vector b)
{
    Vector v = { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
    return v;
}

/* Return a vector multiplied by a scalar. */
static Vector vector_mul(Vector a, float s)
{
    Vector v = { a.x * s, a.y * s, a.z * s };
    return v;
}

/* Return the vector norm (length). */
static double vector_norm(Vector a)
{
    return sqrt(vector_dot_product(a, a));
}

/* Return a normalized version of the given vector. */
static Vector vector_normalize(Vector a)
{
    double s = vector_norm(a);
    if (s == 0)
        return a;
    return vector_mul(a, 1 / s);
}

/* In-place add another vector to a vector. */
static void vector_iadd(Vector* a, Vector b)
{
    a->x += b.x;
    a->y += b.y;
    a->z += b.z;
}