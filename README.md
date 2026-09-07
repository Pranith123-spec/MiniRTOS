# MiniRTOS

A lightweight simulated real-time operating system scheduler written in C.

## Features

* Periodic task scheduling
* Priority-based task scheduling
* Task enable/disable
* CPU utilization calculation
* CPU overload detection
* CPU idle detection
* Task execution statistics
* Maximum tasks per scheduler tick
* Modular C architecture

## Project Structure

```text
MiniRTOS/
│
├── src/
│   ├── main.c
│   ├── scheduler.c
│   └── tasks.c
│
├── include/
│   ├── scheduler.h
│   └── tasks.h
│
├── README.md
└── .gitignore
```

## Tasks

The project currently contains three simulated tasks:

| Task   |  Period | Priority | Execution Time |
| ------ | ------: | -------: | -------------: |
| Sensor |  100 ms |        1 |          20 ms |
| LED    |  500 ms |        2 |           5 ms |
| UART   | 1000 ms |        3 |          30 ms |

Smaller priority number means higher priority.

## CPU Utilization

The scheduler calculates theoretical CPU utilization using:

```text
Utilization = Σ (Execution Time / Period) × 100
```

Current utilization:

```text
Sensor = 20 / 100 = 20%
LED    = 5 / 500  = 1%
UART   = 30 / 1000 = 3%

Total = 24%
```

## Build

Using GCC:

```bash
gcc src/main.c src/scheduler.c src/tasks.c -Iinclude -o scheduler.exe
```

## Run

On Windows:

```bash
.\scheduler.exe
```

## Example Output

```text
CPU Utilization: 24.00%
CPU LOAD is within limit.

[Time: 100 ms] Sensor Task
[Time: 200 ms] Sensor Task
[Time: 500 ms] Sensor Task
[Time: 500 ms] LED Task
...
```

## Concepts Demonstrated

This project demonstrates fundamental RTOS concepts such as:

* Task Control Blocks
* Periodic scheduling
* Priority scheduling
* Task states
* Scheduler tick
* CPU utilization
* CPU overload
* Idle CPU detection
* Runtime task control

## Note

This is a software simulation of an RTOS scheduler running on a PC. It does not currently control physical hardware.
