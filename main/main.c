#include <stdio.h>

#include "process_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int flag = 0;

void process_fun(void *param)
{
    for (;;)
    {
        printf("flag : %d\n", flag);
    }
}

int eval()
{
    if (flag > 5)
    {
        return 0;
    }

    flag++;
    return -1;
}

void app_main(void)
{
    static uint8_t ucParameterToPass;

    struct process *p = new_process(process_fun, &ucParameterToPass, "ProcessFunction", PROCESS_MODE_WAIT_FOR_CONDITIONS);
    run_process(eval, p); // blocks
}