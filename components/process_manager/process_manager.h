#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Process Mode List
This definitions are necessary to run a process
with process_manager.h. process_manager.h will be
the only SAFE way to run processes that require
proper physical conditions outside the medium.
This means, every movement the system does needs
to be evaluated before running the current process.

This list has the type of priority of the process
that will be run.
*/
#undef PROCESS_MANAGER_SAFE_MODE

#define PROCESS_TYPE_LENGTH 3

#define PROCESS_MODE_SKIP               0x00 // 0b00000000
#define PROCESS_MODE_EVAL               0x01 // 0b00000001
#define PROCESS_MODE_HIGH_PRIORITY      0x02 // 0b00000010
#define PROCESS_MODE_HIGH_PRIORITY_EVAL 0x03 // 0b00000011

#define PROCESS_EXIT_SUCCESS            0x00
#define PROCESS_NO_EXECUTED             0x01

typedef uint8_t process_type_t;

typedef int (*eval_fn)(void);

struct process
{
    TaskFunction_t process_code;
    process_type_t process_mode;
    const char *process_name;
    void *process_params;
    UBaseType_t process_priority;
};

/* Create a new process struct (allocated in the heap, run *free_process to free it)*/
struct process *new_process(TaskFunction_t function, void *params, const char *name, process_type_t process_type);
/* run a process with an evaluation function if required (NULL if not, but might be unsafe if not killed it manually) */
int run_process(eval_fn eval, struct process *process);
/* free a process struct */
void free_process(void *process);

#endif