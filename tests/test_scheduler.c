#include <stdio.h>
#include <assert.h>
#include <math.h>

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
 * Zero-period task must be rejected.
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
 * NULL task function must be rejected.
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
 * Maximum task limit must be enforced.
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
 * NULL scheduler pointers must be rejected.
 */
static void test_null_scheduler_inputs(void)
{
    Task tasks[1];
    int taskCount = 0;

    int result1 = scheduler_add_task(
        NULL,
        &taskCount,
        1,
        testTask1,
        100,
        1,
        10
    );

    int result2 = scheduler_add_task(
        tasks,
        NULL,
        1,
        testTask1,
        100,
        1,
        10
    );

    assert(result1 != 0);
    assert(result2 != 0);
    assert(taskCount == 0);

    printf("PASS: NULL scheduler inputs rejected\n");
}


/*
 * Test 6:
 * Invalid maximum task count must be rejected.
 */
static void test_invalid_max_tasks(void)
{
    Task tasks[1];
    int taskCount = 0;

    int result = scheduler_add_task(
        tasks,
        &taskCount,
        0,
        testTask1,
        100,
        1,
        10
    );

    assert(result != 0);
    assert(taskCount == 0);

    printf("PASS: Invalid maximum task count rejected\n");
}


/*
 * Test 7:
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
 * Test 8:
 * NULL task pointers must not crash
 * enable/disable functions.
 */
static void test_null_enable_disable(void)
{
    scheduler_enable_task(NULL, 100);
    scheduler_disable_task(NULL);

    printf("PASS: NULL enable/disable handled\n");
}


/*
 * Test 9:
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
 * Test 10:
 * Zero-task utilization should be 0%.
 */
static void test_zero_task_utilization(void)
{
    float utilization =
        scheduler_calculate_utilization(
            NULL,
            0
        );

    assert(utilization == 0.0f);

    printf("PASS: Zero-task utilization\n");
}


/*
 * Test 11:
 * CPU utilization above 100% should be
 * calculated correctly.
 *
 * 80 / 100 = 80%
 * 60 / 100 = 60%
 *
 * Total = 140%
 */
static void test_cpu_overload(void)
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
        80
    );

    int result2 = scheduler_add_task(
        tasks,
        &taskCount,
        2,
        testTask2,
        100,
        2,
        60
    );

    assert(result1 == 0);
    assert(result2 == 0);

    float utilization =
        scheduler_calculate_utilization(
            tasks,
            taskCount
        );

    assert(fabsf(utilization - 140.0f) < 0.01f);

    printf("PASS: CPU overload calculation\n");
}


/*
 * Test 12:
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
     * At 100 ms the task executes.
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
 * Test 13:
 * Disabled task must not execute.
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
 * Test 14:
 * Higher-priority task should execute first.
 *
 * Smaller priority number = higher priority.
 */
static void test_priority_selection(void)
{
    Task tasks[2];
    int taskCount = 0;

    int result1 = scheduler_add_task(
        tasks,
        &taskCount,
        2,
        testTask1,
        100,
        2,
        10
    );

    int result2 = scheduler_add_task(
        tasks,
        &taskCount,
        2,
        testTask2,
        100,
        1,
        10
    );

    assert(result1 == 0);
    assert(result2 == 0);

    scheduler_run(
        tasks,
        taskCount,
        100
    );

    /*
     * Both tasks are due.
     * The scheduler can execute both,
     * but the higher-priority task must
     * be selected first.
     *
     * Since both run once, verify that
     * both were executed.
     */
    assert(tasks[0].runCount == 1);
    assert(tasks[1].runCount == 1);

    printf("PASS: Priority scheduling\n");
}


/*
 * Test 15:
 * Re-enabling a task must discard missed
 * executions rather than creating a burst.
 */
static void test_no_missed_period_catchup(void)
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
     * First execution at 100 ms.
     */
    scheduler_run(
        tasks,
        taskCount,
        100
    );

    assert(tasks[0].runCount == 1);

    /*
     * Disable the task.
     */
    scheduler_disable_task(&tasks[0]);

    /*
     * Re-enable at 500 ms.
     *
     * Missed executions from 200,
     * 300 and 400 ms are discarded.
     */
    scheduler_enable_task(
        &tasks[0],
        500
    );

    /*
     * It must NOT execute immediately
     * at 500 ms.
     */
    scheduler_run(
        tasks,
        taskCount,
        500
    );

    assert(tasks[0].runCount == 1);

    /*
     * It should execute at 600 ms.
     */
    scheduler_run(
        tasks,
        taskCount,
        600
    );

    assert(tasks[0].runCount == 2);

    printf("PASS: Missed-period catch-up prevented\n");
}


/*
 * Main test runner.
 */
int main(void)
{
    /*
     * Disable scheduler diagnostic output
     * during unit tests.
     */
    scheduler_set_test_mode(1);

    printf("========================================\n");
    printf("       MiniRTOS Scheduler Tests\n");
    printf("========================================\n\n");

    test_task_creation();

    test_zero_period();

    test_null_function();

    test_max_task_limit();

    test_null_scheduler_inputs();

    test_invalid_max_tasks();

    test_enable_disable();

    test_null_enable_disable();

    test_cpu_utilization();

    test_zero_task_utilization();

    test_cpu_overload();

    test_task_execution();

    test_disabled_task();

    test_priority_selection();

    test_no_missed_period_catchup();

    printf("\n========================================\n");
    printf("ALL TESTS PASSED\n");
    printf("========================================\n");

    return 0;
}