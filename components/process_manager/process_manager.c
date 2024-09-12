#include "process_manager.h"

#include "esp_log.h"

#define PROCESS_MANAGER_TAG "PROCESS_MANAGER"
#define MAX_STACK_SIZE_PER_TASK_B 4096

int checkProcessType(process_type_t type)
{
    if (type > PROCESS_TYPE_LENGTH)
    {
        return -1;
    }

    return 0;
}

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
    p->process_priority = tskIDLE_PRIORITY + 1;
    return p;
}

void free_process(void *process)
{
    struct process *p = (struct process *)process;

    ESP_LOGD(PROCESS_MANAGER_TAG, "Freeing process '%s'", p->process_name);

    if (process == NULL)
    {
        return;
    }

    free(process);
}

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

    return handle;
}

TaskHandle_t exec_process(struct process *process)
{
    TaskHandle_t handle = NULL;
    ESP_LOGD(PROCESS_MANAGER_TAG, "[%s] Starting new process", process->process_name);

    if ((handle = register_process(process->process_code, process->process_name, process->process_params, process->process_mode, process->process_priority)) == NULL)
    {
        ESP_LOGD(PROCESS_MANAGER_TAG, "[%s] Failed to register process", process->process_name);
        return NULL;
    }

    return handle;
}

int run_process(int (*eval)(void), struct process *process)
{
    int status = -PROCESS_NO_EXECUTED;

    TaskHandle_t handle = NULL;

    if ((process->process_mode & PROCESS_MODE_HIGH_PRIORITY) == PROCESS_MODE_HIGH_PRIORITY)
        process->process_priority = configMAX_PRIORITIES - 1;

    if (eval == NULL)
    {
        ESP_LOGD(PROCESS_MANAGER_TAG, "WARNING: evaluation function is null for process name '%s'", process->process_name);

#ifdef PROCESS_MANAGER_SAFE_MODE
        free_process(process);
        return -PROCESS_NO_EXECUTED;
#endif
    }

    ESP_LOGD(PROCESS_MANAGER_TAG, "[%s] Checking process mode", process->process_name);

    if (eval == NULL || process->process_mode == PROCESS_MODE_SKIP)
    {
        handle = exec_process(process);
        if (handle == NULL)
        {
            return -PROCESS_NO_EXECUTED;
        }

        ESP_LOGV(PROCESS_MANAGER_TAG, "PROCESS_MODE_SKIP");
    }
    else if ((process->process_mode & PROCESS_MODE_EVAL) == PROCESS_MODE_EVAL)
    {
        handle = exec_process(process);

        if (handle == NULL)
        {
            return -PROCESS_NO_EXECUTED;
        }

        ESP_LOGV(PROCESS_MANAGER_TAG, "PROCESS_MODE_EVAL");

        while (status < 0)
        {
            status = eval();
        }

        vTaskDelete(handle);
    }

    if (process != NULL)
    {
        free_process(process);
    }
    ESP_LOGV(PROCESS_MANAGER_TAG, "returning");

    return status;
}
