#include "algebra.h"
#include <math.h>

bool is_angle_valid_rad(double a)
{
    if (a < 0 || a >= 2 * M_PI)
        return false;
    return true;
}

double radian_to_degree(double angle)
{
    return (180 / M_PI) * angle;
}

double degree_to_radian(double angle)
{
    return (M_PI / 180) * angle;
}

double get_modulus(vector_t vector)
{
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

double dot_product2d(vector_t u, vector_t v)
{
    return u.x * v.x + u.y * v.y;
}

double cross_product2d(vector_t u, vector_t v, double phi)
{
    return sin(phi) * get_modulus(u) * get_modulus(v);
}

char evaluate_crossp2d(double a)
{
    if (a > 0)
        return CLOCKWISE;
    return ANTICLOCKWISE;
}

void direction_free(direction_t *ptr)
{
    free(ptr);
}

double degrees_from_north(vector_t v)
{
    vector_t vn = {.x = 0, .y = 1};
    return acos(dot_product2d(v, vn) / (get_modulus(vn) * get_modulus(v)));
}

direction_t *direction_t_init(double angle, char direction)
{
    direction_t *d = (direction_t *)malloc(sizeof(direction_t));
    d->angle_rad = angle;
    d->is_clockwise = false;
    return d;
}

direction_t *direction_from_angle(double north_deg, node_t *v_o)
{
    if (!is_angle_valid_rad(degree_to_radian(north_deg)))
    {
        return NULL;
    }

    vector_t v = {.x = (double)v_o->x, .y = (double)v_o->y};
    vector_t dir_v = {.x = cos(north_deg), .y = sin(north_deg)};

    double deg = degrees_from_north(v); // degrees of objective respect to north

    char direction = evaluate_crossp2d(cross_product2d(v, dir_v, deg)); // evaluate direction

    double final_angle = 2 * M_PI - (deg + north_deg);
    
    if (final_angle == 2 * M_PI)
        final_angle = 0;

    return direction_t_init(final_angle, direction);
}
