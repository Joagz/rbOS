/*
 ** rbOS **
 * Author:   Joaquín Gómez
 * Date:     09/09/2024
 */
#include <stdio.h>

#include "process_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int flag = 0;

void process_fun(void *param)
{
    static int i = 1;
    for (;;)
    {
        printf("COUNTER: %d\n", i);
        vTaskDelay(pdMS_TO_TICKS(1000));
        i++;
    }
}

int eval()
{
    vTaskDelay(pdMS_TO_TICKS(10000));
    return 0;
}

void app_main(void)
{
    static uint8_t ucParameterToPass;

    struct process *p = new_process(process_fun, &ucParameterToPass, "ProcessFunction", PROCESS_MODE_EVAL);
    run_process(eval, p);
}