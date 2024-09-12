#ifndef MOEVEMENTPLANNER_H
#define MOEVEMENTPLANNER_H

#include "pqueue.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define BLOCK_LENGTH_CM 10
#define BLOCK_SIZE_CM2 100
#define DEFAULT_INITIAL_PATH_CAPACITY 500
#define CURRENT_END_NODE_NOT_DEFINED 0xFA0A
#define CURRENT_NODE_NOT_DEFINED 0xFA1A
#define CURRENT_NODE_NO_POS 0xFF

#define NEIGHBOUR_UPPER 0x00
#define NEIGHBOUR_LOWER 0x01
#define NEIGHBOUR_LEFT 0x02
#define NEIGHBOUR_RIGHT 0x03
#define NEIGHBOUR_UPPER_LEFT 0x04
#define NEIGHBOUR_UPPER_RIGHT 0x05
#define NEIGHBOUR_LOWER_LEFT 0x06
#define NEIGHBOUR_LOWER_RIGHT 0x07

typedef struct
{
    bool enabled;
    int x;
    int y;
    pqueue_pri_t pri;
    size_t pos;
} node_t;

struct path
{
    node_t *nodes;
    int32_t length;
};

struct path_map
{
    struct path *path;
    int32_t length;
};

struct plane_map
{
    int32_t size_x;
    int32_t size_y;
    node_t *nodes;
};

node_t *get_at(struct plane_map *p, int32_t x, int32_t y);
void block_at(struct plane_map *p, int32_t x, int32_t y);
void free_at(struct plane_map *p, int32_t x, int32_t y);
// free a plane map. Must call it after using a struct plane_map 
void free_plane_map(struct plane_map *p);
// alloc size for a new plane map and return its pointer
struct plane_map *new_plane_map(int32_t size_x, int32_t size_y);
// generate a path to a node in the map
struct path_map *generate_new_path(struct plane_map *p, int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y);

#endif