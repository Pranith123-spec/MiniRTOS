#include <stdio.h>
#include "scheduler.h"

static int testMode = 0;

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
    /*
     * Validate pointers.
     */
    if (tasks == NULL ||
        taskCount == NULL ||
        function == NULL)
    {
        return -1;
    }

    /*
     * Validate task capacity.
     */
    if (maxTasks <= 0 ||
        *taskCount < 0)
    {
        return -1;
    }

    /*
     * Check whether task table is full.
     */
    if (*taskCount >= maxTasks)
    {
        return -2;
    }

    /*
     * A periodic task cannot have
     * a zero period.
     */
    if (period == 0)
    {
        return -3;
    }

    /*
     * Initialize task.
     */
    tasks[*taskCount].function = function;
    tasks[*taskCount].period = period;
    tasks[*taskCount].lastRun = 0;

    tasks[*taskCount].priority = priority;
    tasks[*taskCount].executionTime = executionTime;

    tasks[*taskCount].runCount = 0;

    tasks[*taskCount].state = TASK_READY;

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

    /*
     * Validate scheduler input.
     */
    if (tasks == NULL ||
        taskCount <= 0)
    {
        return;
    }

    /*
     * Execute at most
     * MAX_TASKS_PER_TICK tasks.
     */
    for (int count = 0;
         count < MAX_TASKS_PER_TICK;
         count++)
    {
        int selectedTask = -1;

        /*
         * Find the highest-priority
         * ready task.
         *
         * Smaller priority number
         * means higher priority.
         */
        for (int i = 0;
             i < taskCount;
             i++)
        {
            if (tasks[i].state == TASK_READY &&
                tasks[i].period > 0 &&
                (systemTime - tasks[i].lastRun)
                    >= tasks[i].period)
            {
                if (selectedTask == -1 ||
                    tasks[i].priority <
                    tasks[selectedTask].priority)
                {
                    selectedTask = i;
                }
            }
        }

        /*
         * No task is ready.
         */
        if (selectedTask == -1)
        {
            break;
        }

        /*
         * Advance task's schedule by
         * one period.
         */
        tasks[selectedTask].lastRun +=
            tasks[selectedTask].period;

        /*
         * Update execution statistics.
         */
        tasks[selectedTask].runCount++;

      if (!testMode)
{
    printf(
        "[Time: %u ms] ",
        systemTime
    );
}
        /*
         * Execute selected task.
         */
        tasks[selectedTask].function();

        tasksExecuted++;
    }

    /*
     * Display CPU idle when no task
     * was executed during this tick.
     */
    if (tasksExecuted == 0 && !testMode)
    {
    printf(
        "[Time: %u ms] CPU IDLE\n",
        systemTime
    );
  }
}


void scheduler_enable_task(
    Task *task,
    unsigned int systemTime
)
{
    if (task == NULL)
    {
        return;
    }

    /*
     * Discard missed executions while
     * the task was disabled.
     *
     * The task will execute on its
     * next normal period.
     */
    task->lastRun = systemTime;

    task->state = TASK_READY;
}


void scheduler_disable_task(Task *task)
{
    if (task == NULL)
    {
        return;
    }

    task->state = TASK_DISABLED;
}


float scheduler_calculate_utilization(
    Task tasks[],
    int taskCount
)
{
    float utilization = 0.0f;

    if (tasks == NULL ||
        taskCount <= 0)
    {
        return 0.0f;
    }

    for (int i = 0;
         i < taskCount;
         i++)
    {
        if (tasks[i].period > 0)
        {
            utilization +=
                (float)tasks[i].executionTime /
                tasks[i].period;
        }
    }

    return utilization * 100.0f;
}

void scheduler_set_test_mode(int enabled)
{
    testMode = enabled;
}