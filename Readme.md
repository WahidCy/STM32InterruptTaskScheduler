# STM32 Task Scheduler

A lightweight task scheduler for STM32 microcontrollers written in C. This scheduler allows periodic task execution based on a tick timer with support for execution priorities.

## Features

- Schedule up to 10 periodic tasks
- Task prioritization (lower value = higher priority)
- Executes based on timer tick frequency (e.g., SysTick)
- Lightweight and suitable for bare-metal systems

## Files

- `TaskScheduler.c` — Scheduler implementation
- `TaskScheduler.h` — Header file with structure and function declarations
- No external dependencies other than `stdint.h`

## Usage

### 1. Initialize the Scheduler

```c
TaskScheduler_Init(1000); // For example, 1000 ticks per second (1 ms)
```

Call this during system initialization. Ensure the timer (e.g., SysTick) is configured to generate interrupts at the specified tick frequency.

### 2. Add a Task

```c
void MyTask(void) {
    // This function will be called periodically
}

int task_id = TaskScheduler_AddTask(10, MyTask, 1); // 10 Hz, priority 1
```

### 3. Update the Scheduler

Call `TaskScheduler_Update()` inside your timer interrupt handler:

```c
void SysTick_Handler(void) {
    TaskScheduler_Update();
}
```

## STM32CubeMX Configuration

To use this scheduler properly, configure your timer as follows:

- **Enable SysTick** with a base time of **1 ms** (1000 Hz)
- **Enable interrupt generation**
- Avoid using blocking delays (e.g., `HAL_Delay`) in task functions

### Example SysTick Configuration in CubeMX:

- Timebase Source: **SysTick**
- Tick Frequency: **1000 Hz**
- Interrupt: **Enabled**

## Task Structure

Each task consists of:

- `interval_ticks`: Number of ticks between executions
- `counter`: Internal countdown counter
- `callback`: Pointer to the task function
- `active`: Activation status
- `priority`: Lower number = higher priority

Tasks that are due to run in the same tick are executed in order of priority.

## License

MIT License

## Author

This scheduler was developed as a minimal cooperative task scheduler for STM32 bare-metal applications, especially useful when not using an RTOS.
