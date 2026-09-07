#include <stdio.h>
#include <assert.h>

#include "scheduler.h"

/*
 * Test task functions.
 */
static void testTask1(void)
{
}

static void testTask2(void)
{
}


/*
 * Test 1:
 * Task should be added successfully.
 */
static void test_task_creation(void)
{
    Task tasks[2];
    int taskCount = 0;

    int result = scheduler_add_task(
        tasks,
        &taskCount,
        2,
        testTask1,
        100,
        1,
        10
    );

    assert(result == 0);
    assert(taskCount == 1);
    assert(tasks[0].period == 100);
    assert(tasks[0].priority == 1);
    assert(tasks[0].executionTime == 10);
    assert(tasks[0].state == TASK_READY);
    assert(tasks[0].runCount == 0);

    printf("PASS: Task creation\n");
}


/*
 * Test 2:
 * Scheduler should reject a zero period.
 */
static void test_zero_period(void)
{
    Task tasks[2];
    int taskCount = 0;

    int result = scheduler_add_task(
        tasks,
        &taskCount,
        2,
        testTask1,
        0,
        1,
        10
    );

    assert(result != 0);
    assert(taskCount == 0);

    printf("PASS: Zero period rejected\n");
}


/*
 * Test 3:
 * Scheduler should reject NULL task function.
 */
static void test_null_function(void)
{
    Task tasks[2];
    int taskCount = 0;

    int result = scheduler_add_task(
        tasks,
        &taskCount,
        2,
        NULL,
        100,
        1,
        10
    );

    assert(result != 0);
    assert(taskCount == 0);

    printf("PASS: NULL function rejected\n");
}


/*
 * Test 4:
 * Scheduler should reject tasks beyond
 * the maximum task limit.
 */
static void test_max_task_limit(void)
{
    Task tasks[1];
    int taskCount = 0;

    int result1 = scheduler_add_task(
        tasks,
        &taskCount,
        1,
        testTask1,
        100,
        1,
        10
    );

    int result2 = scheduler_add_task(
        tasks,
        &taskCount,
        1,
        testTask2,
        200,
        2,
        10
    );

    assert(result1 == 0);
    assert(result2 != 0);
    assert(taskCount == 1);

    printf("PASS: Maximum task limit\n");
}


/*
 * Test 5:
 * Disable and enable should correctly
 * change task state.
 */
static void test_enable_disable(void)
{
    Task tasks[1];
    int taskCount = 0;

    int result = scheduler_add_task(
        tasks,
        &taskCount,
        1,
        testTask1,
        100,
        1,
        10
    );

    assert(result == 0);
    assert(tasks[0].state == TASK_READY);

    scheduler_disable_task(&tasks[0]);

    assert(tasks[0].state == TASK_DISABLED);

    scheduler_enable_task(&tasks[0], 500);

    assert(tasks[0].state == TASK_READY);
    assert(tasks[0].lastRun == 500);

    printf("PASS: Enable/disable task\n");
}


/*
 * Test 6:
 * CPU utilization calculation.
 *
 * Task 1:
 * 10 / 100 = 10%
 *
 * Task 2:
 * 20 / 200 = 10%
 *
 * Total = 20%
 */
static void test_cpu_utilization(void)
{
    Task tasks[2];
    int taskCount = 0;

    int result1 = scheduler_add_task(
        tasks,
        &taskCount,
        2,
        testTask1,
        100,
        1,
        10
    );

    int result2 = scheduler_add_task(
        tasks,
        &taskCount,
        2,
        testTask2,
        200,
        2,
        20
    );

    assert(result1 == 0);
    assert(result2 == 0);

    float utilization =
        scheduler_calculate_utilization(
            tasks,
            taskCount
        );

    assert(utilization == 20.0f);

    printf("PASS: CPU utilization\n");
}


/*
 * Test 7:
 * A task should execute when its period
 * becomes due.
 */
static void test_task_execution(void)
{
    Task tasks[1];
    int taskCount = 0;

    int result = scheduler_add_task(
        tasks,
        &taskCount,
        1,
        testTask1,
        100,
        1,
        10
    );

    assert(result == 0);

    /*
     * At 0 ms the task is not due.
     */
    scheduler_run(
        tasks,
        taskCount,
        0
    );

    assert(tasks[0].runCount == 0);

    /*
     * At 100 ms the task is due.
     */
    scheduler_run(
        tasks,
        taskCount,
        100
    );

    assert(tasks[0].runCount == 1);

    /*
     * At 200 ms it executes again.
     */
    scheduler_run(
        tasks,
        taskCount,
        200
    );

    assert(tasks[0].runCount == 2);

    printf("PASS: Task execution count\n");
}


/*
 * Test 8:
 * A disabled task must not execute.
 */
static void test_disabled_task(void)
{
    Task tasks[1];
    int taskCount = 0;

    int result = scheduler_add_task(
        tasks,
        &taskCount,
        1,
        testTask1,
        100,
        1,
        10
    );

    assert(result == 0);

    scheduler_disable_task(&tasks[0]);

    scheduler_run(
        tasks,
        taskCount,
        100
    );

    assert(tasks[0].runCount == 0);

    printf("PASS: Disabled task does not execute\n");
}


/*
 * Main test runner.
 */
int main(void)
{
    scheduler_set_test_mode(1);
    printf("========================================\n");
    printf("       MiniRTOS Scheduler Tests\n");
    printf("========================================\n\n");

    test_task_creation();

    test_zero_period();

    test_null_function();

    test_max_task_limit();

    test_enable_disable();

    test_cpu_utilization();

    test_task_execution();

    test_disabled_task();

    printf("\n========================================\n");
    printf("ALL TESTS PASSED\n");
    printf("========================================\n");

    return 0;
}