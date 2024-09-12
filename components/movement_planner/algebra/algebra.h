#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "commons.h"

#define ANTICLOCKWISE 0
#define CLOCKWISE 1

/*
    Get angle respect to north and end node, and generate the angle
    in between. Returns a POSITIVE ANGLE and DIRECTION:

    ANTICLOCKWISE = 0;
    CLOCKWISE = 1;
*/
direction_t *direction_from_angle(double north_deg, node_t *v_o);
void direction_free(direction_t *ptr);

double radian_to_degree(double angle);
double degree_to_radian(double angle);
bool is_angle_valid_rad(double angle);

#endif