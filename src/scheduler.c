#include <stdio.h>
#include "scheduler.h"

int scheduler_add_task(
    Task tasks[],
    int *taskCount,
    int maxTasks,
    TaskFunction function,
    unsigned int period,
    unsigned int priority,
    unsigned int executionTime
)
{
    if (*taskCount >= maxTasks)
    {
        return -1;
    }

    tasks[*taskCount].function = function;
    tasks[*taskCount].period = period;
    tasks[*taskCount].lastRun = 0;
    tasks[*taskCount].priority = priority;
    tasks[*taskCount].executionTime = executionTime;
    tasks[*taskCount].state = TASK_READY;
    tasks[*taskCount].runCount = 0;

    (*taskCount)++;

    return 0;
}

void scheduler_run(
    Task tasks[],
    int taskCount,
    unsigned int systemTime
)
{
    int tasksExecuted = 0;
    for (int count = 0; count < MAX_TASKS_PER_TICK; count++)
    {
        int selectedTask = -1;

        for (int i = 0; i < taskCount; i++)
        {
            if (tasks[i].state == TASK_READY &&
                (systemTime - tasks[i].lastRun) >= tasks[i].period)
            {
                if (selectedTask == -1 ||
                    tasks[i].priority < tasks[selectedTask].priority)
                {
                    selectedTask = i;
                }
            }
        }

        if (selectedTask == -1)
        {
            break;
        }

        tasks[selectedTask].lastRun +=
            tasks[selectedTask].period;
        tasks[selectedTask].runCount++;

        printf("[Time: %u ms] ", systemTime);

        tasks[selectedTask].function();

        tasksExecuted++;
    }
    if(tasksExecuted == 0)
    {
        printf("[Time: %u ms] CPU IDLE\n", systemTime);
    }
}

void scheduler_enable_task(Task *task)
{
    task->state = TASK_READY;
}

void scheduler_disable_task(Task *task)
{
    task->state = TASK_DISABLED;
}

float scheduler_calculate_utilization(
    Task tasks[],
    int taskCount
)
{
    float utilization = 0.0f;

    for (int i = 0; i < taskCount; i++)
    {
        utilization +=
            (float)tasks[i].executionTime / tasks[i].period;
    }

    return utilization * 100.0f;
}


