#include <stdio.h>
#include "scheduler.h"
#include "tasks.h"

#define MAX_TASKS 3

int main(void)
{
    Task tasks[MAX_TASKS];

    int taskCount = 0;
    int result;

    /*
     * Add LED Task.
     *
     * Period      = 500 ms
     * Priority    = 2
     * Execution   = 5 ms
     */
    result = scheduler_add_task(
        tasks,
        &taskCount,
        MAX_TASKS,
        ledTask,
        500,
        2,
        5
    );

    if (result != 0)
    {
        printf(
            "ERROR: Failed to add LED task. Code: %d\n",
            result
        );

        return 1;
    }

    /*
     * Add Sensor Task.
     *
     * Period      = 100 ms
     * Priority    = 1
     * Execution   = 20 ms
     */
    result = scheduler_add_task(
        tasks,
        &taskCount,
        MAX_TASKS,
        sensorTask,
        100,
        1,
        20
    );

    if (result != 0)
    {
        printf(
            "ERROR: Failed to add Sensor task. Code: %d\n",
            result
        );

        return 1;
    }

    /*
     * Add UART Task.
     *
     * Period      = 1000 ms
     * Priority    = 3
     * Execution   = 30 ms
     */
    result = scheduler_add_task(
        tasks,
        &taskCount,
        MAX_TASKS,
        uartTask,
        1000,
        3,
        30
    );

    if (result != 0)
    {
        printf(
            "ERROR: Failed to add UART task. Code: %d\n",
            result
        );

        return 1;
    }

    /*
     * Calculate CPU utilization.
     */
    float utilization =
        scheduler_calculate_utilization(
            tasks,
            taskCount
        );

    printf(
        "CPU Utilization: %.2f%%\n\n",
        utilization
    );

    /*
     * Check CPU load.
     */
    if (utilization > 100.0f)
    {
        printf(
            "WARNING: CPU OVERLOADED!\n\n"
        );
    }
    else
    {
        printf(
            "CPU LOAD is within limit.\n\n"
        );
    }

    /*
     * Simulated system clock.
     */
    unsigned int systemTime = 0;

    /*
     * Run scheduler from 0 to 2000 ms.
     */
    while (systemTime <= 2000)
    {
        /*
         * Disable Sensor Task at 1000 ms.
         */
        if (systemTime == 1000)
        {
            printf(
                "---Disabling Sensor Task ---\n"
            );

            scheduler_disable_task(
                &tasks[1]
            );
        }

        /*
         * Enable Sensor Task at 1500 ms.
         *
         * Missed executions are discarded.
         */
        if (systemTime == 1500)
        {
            printf(
                "\n---Enabling Sensor Task ---\n"
            );

            scheduler_enable_task(
                &tasks[1],
                systemTime
            );
        }

        /*
         * Run scheduler.
         */
        scheduler_run(
            tasks,
            taskCount,
            systemTime
        );

        /*
         * Advance simulated clock
         * by 100 ms.
         */
        systemTime += 100;
    }

    /*
     * Print task statistics.
     */
    printf("\nTask Statistics:\n");

    for (int i = 0;
         i < taskCount;
         i++)
    {
        printf(
            "Task %d executed %u times\n",
            i + 1,
            tasks[i].runCount
        );
    }

    return 0;
}