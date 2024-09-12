#ifndef COMMONS_H
#define COMMONS_H

#include <stdint.h>
#include <stdbool.h>
#include "pqueue.h"

typedef struct
{
    double x;
    double y;
} vector_t;

typedef struct
{
    double angle_rad;
    bool is_clockwise;
} direction_t;


typedef struct
{
    bool enabled;
    int x;
    int y;
    pqueue_pri_t pri;
    size_t pos;
} node_t;

struct path_map
{
    direction_t *directions;
    size_t length;
};

struct plane_map
{
    int32_t size_x;
    int32_t size_y;
    node_t *nodes;
};

#endif