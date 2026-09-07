#include <stdio.h>
#include "scheduler.h"
#include "tasks.h"

#define MAX_TASKS 3

int main(void)
{
    Task tasks[MAX_TASKS];

    int taskCount = 0;

    scheduler_add_task(
        tasks,
        &taskCount,
        MAX_TASKS,
        ledTask,
        500,
        2,
        5
    );

    scheduler_add_task(
        tasks,
        &taskCount,
        MAX_TASKS,
        sensorTask,
        100,
        1,
        20
    );

    scheduler_add_task(
        tasks,
        &taskCount,
        MAX_TASKS,
        uartTask,
        1000,
        3,
        30
    );

    float utilization;

    utilization = scheduler_calculate_utilization(
      tasks,
      taskCount
);

printf("CPU Utilization: %.2f%%\n\n", utilization);

if(utilization > 100.0f){
    printf("WaARNING: CPU OVERLOADED!\n");
}
else
{
   printf("CPU LOAD is within limit.\n");
}
printf("\n");
    unsigned int systemTime = 0;

    while (systemTime <= 2000)
    {
        if(systemTime == 1000)
        {
            printf("\n---Disabling Sensor Task ---\n");
            scheduler_disable_task(&tasks[1]);
        }

        if(systemTime == 1500)
        {
            printf("\n---Enabling Sensor Task ---\n");
            tasks[1].lastRun = systemTime;
            scheduler_enable_task(&tasks[1]);
        }
        scheduler_run(
            tasks,
            taskCount,
            systemTime
        );

        systemTime += 100;
    }
    printf("\nTask Statistics:\n");
    for(int i=0; i<taskCount; i++)
    {
        printf("Task %d executed %u times\n", i+1, tasks[i].runCount);
    }

    return 0;
}