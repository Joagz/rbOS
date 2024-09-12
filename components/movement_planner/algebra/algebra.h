#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "movement_planner.h"

#define ANTICLOCKWISE 0
#define CLOCKWISE 1

typedef struct
{
    double x;
    double y;
} vector_t;

typedef struct
{
    double angle_degrees;
    char direction;
} direction_t;

direction_t *get_angle_between(double current_degrees_from_north, node_t end);
void direction_free(direction_t *ptr);
#endif