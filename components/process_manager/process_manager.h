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

#define PROCESS_TYPE_LENGTH 3 // update along with PML

#define PROCESS_MODE_SKIP                   0x00
#define PROCESS_MODE_HIGH_PRIORITY          0x01
#define PROCESS_MODE_WAIT_FOR_CONDITIONS    0x02
// ... could go on

#define PROCESS_EXIT_SUCCESS    0x00
#define PROCESS_NO_EXECUTED     0x01

typedef uint8_t process_type_t;

struct process
{
    TaskFunction_t process_code;
    process_type_t process_mode;
    const char *process_name;
    void *process_params;
};

struct process *new_process(TaskFunction_t function, void *params, const char *name, process_type_t process_type);
int run_process(int (*eval)(void), struct process *process);


#endif