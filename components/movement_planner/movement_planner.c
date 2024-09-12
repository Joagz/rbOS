#include "movement_planner.h"
#include "darr.h"

static node_t *CURRENT_END_NODE = NULL;
static struct plane_map *CURRENT_MAP = NULL;

void print_debug(const char *fn_name, const char *msg)
{
    printf("(D) %s: %s\n", fn_name, msg);
}

void print_err(const char *fn_name, const char *msg)
{
    printf("(E) %s: %s\n", fn_name, msg);
}

bool is_valid_coord(struct plane_map *p, int32_t x, int32_t y)
{
    return !((y >= p->size_y || y < 0) || (x >= p->size_x || x < 0));
}

void print_closed_list(struct darr *CLOSED)
{
    for (int i = 0; i < CLOSED->length; i++)
    {
        node_t *node = (node_t *)CLOSED->items[i];
        printf("ITEM %d: (%d, %d)\n", i, node->x, node->y);
    }
}

node_t *get_at(struct plane_map *p, int32_t x, int32_t y)
{
    if (!is_valid_coord(p, x, y))
    {
        return 0;
    }

    return &p->nodes[y * p->size_x + x];
}

node_t *get_neighbor(struct plane_map *p, int position, int32_t x, int32_t y)
{

    if (!is_valid_coord(p, x, y))
    {
        return NULL;
    }

    switch (position)
    {
    case NEIGHBOUR_UPPER:

        if (y - 1 < 0)
        {
            return NULL;
        }
        return get_at(p, x, y - 1);
    case NEIGHBOUR_LOWER:

        if (y + 1 >= p->size_y)
        {
            return NULL;
        }

        return get_at(p, x, y + 1);
    case NEIGHBOUR_LEFT:

        if (x <= 0)
        {
            return NULL;
        }

        return get_at(p, x - 1, y);
    case NEIGHBOUR_RIGHT:
        if (x >= p->size_x)
        {
            return NULL;
        }

        return get_at(p, x + 1, y);
    case NEIGHBOUR_UPPER_LEFT:
        if (x < 0 || y - 1 < 0)
        {
            return NULL;
        }

        return get_at(p, x - 1, y - 1);
    case NEIGHBOUR_UPPER_RIGHT:
        if (x >= p->size_x || y - 1 < 0)
        {
            return NULL;
        }

        return get_at(p, x + 1, y - 1);
    case NEIGHBOUR_LOWER_LEFT:
        if (x < 0 || y + 1 >= p->size_y)
        {
            return NULL;
        }

        return get_at(p, x - 1, y + 1);
    case NEIGHBOUR_LOWER_RIGHT:
        if (x >= p->size_x || y + 1 >= p->size_y)
        {
            return NULL;
        }

        return get_at(p, x + 1, y + 1);
    default:
        return NULL;
        break;
    }
}

void block_at(struct plane_map *p, int32_t x, int32_t y)
{
    p->nodes[y * p->size_x + x].enabled = false;
}

void free_at(struct plane_map *p, int32_t x, int32_t y)
{
    p->nodes[y * p->size_x + x].enabled = true;
}

void free_plane_map(struct plane_map *p)
{
    if (p == NULL)
        return;

    free(p->nodes);
    free(p);
    print_debug("free_plane_map", "plane_map freed");
}

void clear_globals()
{
    CURRENT_MAP = NULL;
    CURRENT_END_NODE = NULL;
}

struct plane_map *new_plane_map(int32_t size_x, int32_t size_y)
{

    if (CURRENT_MAP != NULL)
    {
        print_debug("new_plane_map", "loading new map into context");
        clear_globals();
    }

    struct plane_map *p = (struct plane_map *)(malloc(sizeof(struct plane_map)));
    p->size_x = size_x;
    p->size_y = size_y;
    p->nodes = (node_t *)(calloc(size_x * size_y, sizeof(node_t)));

    for (int x = 0; x < size_x; x++)
    {
        for (int y = 0; y < size_y; y++)
        {
            p->nodes[y * size_x + x].enabled = true;
            p->nodes[y * size_x + x].x = x;
            p->nodes[y * size_x + x].y = y;
            p->nodes[y * size_x + x].pos = -CURRENT_NODE_NO_POS;
            // printf("NODE {%d, %d} ENABLED\n", x, y);
        }
    }

    printf("Final size of plane_map in B is %ld\n", size_y * size_x * sizeof(node_t));
    printf("Final size of plane_map in KB is %ld\n", (size_y * size_x * sizeof(node_t)) / 1000);
    CURRENT_MAP = p;
    return p;
}

int get_node_h_euclidean(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y)
{
    return (int)ceil(sqrt(pow(end_x - start_x, 2) + pow(end_y - start_y, 2)));
}

int get_node_h_manhattan(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y)
{
    return abs(start_x - end_x) + abs(start_y - end_y);
}

/* COMPARATION FUNCTIONS FOR PQUEUE */

/*
    The callback function to run to compare two elements This callback should return 0 for 'lower'
    and non-zero for 'higher', or vice versa if reverse priority is desired
*/
int cmppri(pqueue_pri_t next, pqueue_pri_t cur)
{
    if (next < cur)
        return 0;
    return 1;
}

/*
    ptr should point to a node_t object
*/
pqueue_pri_t getpri(void *ptr)
{
    node_t *node = (node_t *)ptr;
    return node->pri;
}

void setpri(void *ptr, pqueue_pri_t pri)
{
    node_t *node = (node_t *)ptr;
    node->pri = pri;
}

size_t getpos(void *ptr)
{
    node_t *node = (node_t *)ptr;
    return node->pos;
}

static void setpos(void *ptr, size_t pos)
{
    node_t *node = (node_t *)ptr;
    node->pos = pos;
}

/* END OF COMPARATION FUNCTIONS FOR PQUEUE */

int get_node_priority(node_t *ptr)
{
    if (CURRENT_END_NODE == NULL)
    {
        exit(1);
    }

    int pri_m = get_node_h_manhattan(ptr->x, ptr->y, CURRENT_END_NODE->x, CURRENT_END_NODE->y);
    int pri_e = get_node_h_euclidean(ptr->x, ptr->y, CURRENT_END_NODE->x, CURRENT_END_NODE->y);

    if (pri_e > pri_m)
    {
        return pri_m;
    }
    else
        return pri_e;
}

struct path_map *generate_new_path(struct plane_map *p, int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y)
{
    if (CURRENT_MAP != p)
    {
        CURRENT_MAP = p;
    }

    CURRENT_END_NODE = get_at(p, end_x, end_y);
    if (CURRENT_END_NODE == NULL)
    {
        print_err("generate_new_path", "Exiting...");
        return NULL;
    }

    if (!is_valid_coord(p, end_x, end_y) || !is_valid_coord(p, start_x, start_y))
    {
        print_err("generate_new_path", "Exiting...");
        return NULL;
    }

    pqueue_t *OPEN = pqueue_init(DEFAULT_INITIAL_PATH_CAPACITY, cmppri, getpri, setpri, getpos, setpos);
    struct darr *CLOSED = darr_new(DEFAULT_INITIAL_PATH_CAPACITY);

    node_t *start_node = get_at(p, start_x, start_y);
    if (start_node == NULL)
    {
        pqueue_free(OPEN);
        darr_free(CLOSED);
        return NULL;
    }

    start_node->pri = get_node_h_euclidean(start_x, start_y, end_x, end_y);
    pqueue_insert(OPEN, (void *)start_node);

    while (OPEN->size > 0)
    {
        node_t *current = pqueue_pop(OPEN);
        if (current == NULL)
        {
            break;
        }

        darr_add(CLOSED, (void *)current);

        if (current == CURRENT_END_NODE)
        {
            break; // Found the path
        }

        for (int NEIGHBOR_POS = 0; NEIGHBOR_POS < 8; NEIGHBOR_POS++)
        {
            node_t *neighbor = get_neighbor(p, NEIGHBOR_POS, current->x, current->y);

            if (neighbor == NULL || neighbor->enabled == false)
            {
                continue;
            }

            int cost_from_current = current->pri + get_node_h_euclidean(current->x, current->y, neighbor->x, neighbor->y);
            int e_heuristic = get_node_h_euclidean(neighbor->x, neighbor->y, CURRENT_END_NODE->x, CURRENT_END_NODE->y);
            // int m_heuristic = get_node_h_manhattan(neighbor->x, neighbor->y, CURRENT_END_NODE->x, CURRENT_END_NODE->y);

            int total_cost = cost_from_current + e_heuristic;
            neighbor->pri = e_heuristic;

            int open_pos = OPEN->getpos(neighbor);
            if (open_pos >= 0 && open_pos < OPEN->avail)
            {
                if (total_cost <= neighbor->pri)
                {
                    pqueue_remove(OPEN, (void *)neighbor);
                }
            }

            int closed_index = darr_find(CLOSED, (void *)neighbor);
            if (closed_index >= 0)
            {
                node_t *closed_neighbor = (node_t *)CLOSED->items[closed_index];
                if (total_cost <= closed_neighbor->pri)
                {
                    darr_delete(CLOSED, closed_index);
                }
            }

            if (OPEN->getpos(neighbor) > OPEN->avail && closed_index < 0)
            {
                pqueue_insert(OPEN, (void *)neighbor);
            }
        }
    }

    print_closed_list(CLOSED);

    pqueue_free(OPEN);
    darr_free(CLOSED);


    // TODO
    return (struct path_map *)1;
}