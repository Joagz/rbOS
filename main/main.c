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
    struct plane_map *p = movement_planner_init(10, 10, 0);

    block_at(p, 7, 7);
    block_at(p, 6, 7);
    block_at(p, 5, 7);

    struct path_map * map = generate_new_path(p, 0, 0, 8, 9);

    free_plane_map(p);

    vTaskDelete(NULL);
}

void app_main(void)
{
    static uint8_t ucParameterToPass;

    struct process *p = new_process(process_fun, &ucParameterToPass, "ProcessFunction", PROCESS_MODE_SKIP);
    run_process((eval_fn)NULL, p);

}