#include "process_manager.h"

#include "esp_log.h"

#define PROCESS_MANAGER_TAG "PROCESS_MANAGER"
#define MAX_STACK_SIZE_PER_TASK_B 4096

struct process *new_process(TaskFunction_t function, void *params, const char *name, process_type_t process_type);

int checkProcessType(process_type_t type)
{
    if (type > PROCESS_TYPE_LENGTH)
    {
        return -1;
    }

    return 0;
}

/*
Create a new process structure, if the function has no params, the value passed should be a pointer
to an int of value 1.

example:

... function to register as process

void myfun(int v);

... in your caller function

void* ptr = NULL;
int v = 1;
ptr = &v;

struct process* p = new_process(myfun, ptr, "myfunName", PROCESS_MODE_ANY);

Otherwise, the function will return NULL as params are NULL
*/
struct process *new_process(TaskFunction_t function, void *params, const char *name, process_type_t type)
{
    if (checkProcessType(type) < 0)
    {
        return NULL;
    }

    if (params == NULL)
    {
        return NULL;
    }

    struct process *p = (struct process *)malloc(sizeof(struct process));

    p->process_code = function;
    p->process_name = name;
    p->process_params = params;
    p->process_mode = type;
    return p;
}

// Register new process
TaskHandle_t register_process(TaskFunction_t function, const char *name, void *const params, process_type_t process_type, UBaseType_t priority)
{
    TaskHandle_t handle = NULL;
    xTaskCreate(function, name, MAX_STACK_SIZE_PER_TASK_B, params, priority, &handle);
    configASSERT(handle);

    if (handle == NULL)
    {
        ESP_LOGE(PROCESS_MANAGER_TAG, "Handler for task %s couldn't be created.", name);
        return NULL;
    }

    // decide what to do with the task handler
    return handle;
}

int run_process(int (*eval)(void), struct process *process)
{
    int status = -PROCESS_NO_EXECUTED;
    int priority = tskIDLE_PRIORITY + 1; // low priority

    TaskHandle_t handle = NULL;

    if (process->process_mode == PROCESS_MODE_HIGH_PRIORITY)
        priority = configMAX_PRIORITIES - 1;

    if ((handle = register_process(process->process_code, process->process_name, process->process_params, process->process_mode, priority)) == NULL)
    {
        goto out;
    }

    ESP_LOGD(PROCESS_MANAGER_TAG, "Selecting for process mode");

    if (process->process_mode == PROCESS_MODE_SKIP)
    {
        vTaskDelete(handle);
    }
    else if (process->process_mode == PROCESS_MODE_WAIT_FOR_CONDITIONS)
    {
        // Here we should check for physical conditions or other conditions depending on process_type
        ESP_LOGD(PROCESS_MANAGER_TAG, "Waiting for condition");

        while (status < 0)
        {
            status = eval();
            printf("eval %d\n", status);
        }

        vTaskDelete(handle);
    }

out:
    return status;
}
