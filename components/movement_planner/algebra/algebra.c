#include "algebra.h"
#include <math.h>

double radian_to_degree(double angle)
{
    return (180/M_PI) * angle;
}

double get_modulus(vector_t vector)
{
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

double dot_product2d(vector_t u, vector_t v)
{
    return u.x * v.x + u.y * v.y;
}

/*
    Get angle respect to north and end node, and generate the angle
    in between. Returns a POSITIVE ANGLE and DIRECTION:

    ANTICLOCKWISE = 0;
    CLOCKWISE = 1;
*/
direction_t *get_angle_between(double current_degrees_from_north, node_t end)
{
    if (current_degrees_from_north < 0 || current_degrees_from_north >= 2 * M_PI)
    {
        return NULL;
    }

    direction_t *direction = (direction_t *)malloc(sizeof(direction_t));

    if(direction == NULL)
    {
        return NULL;
    }

    // Unit vector of the current direction
    vector_t start_direction_u = {.x = cos(current_degrees_from_north),
                                  .y = sin(current_degrees_from_north)};

    vector_t end_direction = {.x = (double)end.x,
                              .y = (double)end.y};

    double ev_mod = get_modulus(end_direction);

    // Unit vector of the end direction
    vector_t end_direction_u = {.x = end_direction.x / ev_mod,
                                .y = end_direction.y / ev_mod};

    // get angle in between:

    double dot_product = dot_product2d(start_direction_u, end_direction_u);
    double modulus = get_modulus(start_direction_u) * get_modulus(end_direction_u);
    
    if(modulus == 0)
    {
        return NULL;
    }

    double phi = acos(dot_product / modulus);

    double vector_product = modulus * sin(phi);

    char direction_clock = CLOCKWISE;

    if (vector_product < 0)
    {
        direction_clock = ANTICLOCKWISE;
    }

    direction->angle_degrees = radian_to_degree(phi);
    direction->direction = direction_clock;


    return direction;
}

void direction_free(direction_t *ptr)
{
    free(ptr);
}
