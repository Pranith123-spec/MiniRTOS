#ifndef SCHEDULER_H
#define SCHEDULER_H


#define MAX_TASKS_PER_TICK 10

typedef void (*TaskFunction)(void);

typedef enum
{
    TASK_READY,
    TASK_DISABLED
} TaskState;

typedef struct
{
    TaskFunction function;
    unsigned int period;
    unsigned int lastRun;
    unsigned int priority;
    unsigned int executionTime;
    unsigned int runCount;
    TaskState state;

} Task;

int scheduler_add_task(
    Task tasks[],
    int *taskCount,
    int maxTasks,
    TaskFunction function,
    unsigned int period,
    unsigned int priority,
    unsigned int executionTime
);

void scheduler_run(
    Task tasks[],
    int taskCount,
    unsigned int systemTime
);

void scheduler_enable_task(Task *task);
void scheduler_disable_task(Task *task);

float scheduler_calculate_utilization(
    Task tasks[],
    int taskCount
);

#endif
