/*
 ** rbOS **
 * Author:   Joaquín Gómez
 * Date:     09/09/2024
 */
#include <stdio.h>

#include "process_manager.h"
#include "movement_planner.h"
#include "algebra.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void process_fun(void *param)
{
    struct plane_map *p = new_plane_map(10, 10);

    block_at(p, 7, 7);
    block_at(p, 6, 7);
    block_at(p, 5, 7);

    generate_new_path(p, 0, 0, 8, 9);

    free_plane_map(p);

    vTaskDelete(NULL);
}

void app_main(void)
{
    // static uint8_t ucParameterToPass;

    // struct process *p = new_process(process_fun, &ucParameterToPass, "ProcessFunction", PROCESS_MODE_SKIP);
    // run_process((eval_fn)NULL, p);

    node_t node_end = {.enabled = true, .x = 40, .y = -30, .pos = 1, .pri = 1};
    direction_t *direction = get_angle_between(1.5, node_end);

    printf("A %c\n", ANTICLOCKWISE+48);
    printf("C %c\n", CLOCKWISE+48);

    if (direction != NULL)
        printf("DIRECTION:\n{\nangle_degrees: %f,\ndirection: %c\n}\n", direction->angle_degrees, direction->direction + 48);
    else
        printf("DIRECTION IS NULL\n");

    // direction_free(direction);
}