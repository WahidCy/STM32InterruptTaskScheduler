# STM32 Task Scheduler

A lightweight and efficient cooperative task scheduler for STM32 microcontrollers written in C. Designed for use in bare-metal systems or alongside STM32 HAL, this scheduler allows timed periodic execution of multiple tasks using a timer interrupt such as `SysTick` or a general-purpose timer (e.g., TIM2).

---

## ✅ Features

- Supports up to `10` concurrent periodic tasks
- Task prioritization (lower number = higher priority)
- Runs from timer interrupt (SysTick or TIMx)
- Uses fixed-tick intervals (e.g., 1ms)
- Non-blocking and minimal resource usage
- Compatible with STM32 HAL

---

## 📁 File Structure

| File               | Description                                      |
|--------------------|--------------------------------------------------|
| `TaskScheduler.c`  | Core implementation of the task scheduler        |
| `TaskScheduler.h`  | Public API and task data structure definitions   |

---

## ⚙️ How It Works

The scheduler maintains an internal list of tasks. Each task is executed periodically at a defined frequency. Every time the timer interrupt fires (usually every 1 ms), the scheduler updates its internal counters. When a task is due to run, it is executed from within the interrupt handler, sorted by priority.

---

## 🔧 STM32CubeMX Configuration

To use this scheduler, follow these steps:

1. **Enable a Timer (e.g., TIM2)** in *Timer Mode (Internal Clock)*.
2. Set **Prescaler and Period** so that the timer triggers an update interrupt every 1ms (1000 Hz).
3. **Enable the TIMx global interrupt** in the NVIC settings.
4. Generate the code, and ensure HAL_TIM_Base_Start_IT(&htimx) is called in `main()`.

### Example TIM2 Setup (1ms Tick):
If `APB1 Timer Clock = 84 MHz`:
- **Prescaler** = 8399
- **Period** = 9  
Result: `((8399 + 1) * (9 + 1)) / 84MHz = 0.001s = 1ms`

---

## 🧠 Internal Task Structure

Each task is stored in a structure like the following:

```c
typedef struct {
    uint32_t interval_ticks;  // Number of ticks between executions
    uint32_t counter;         // Countdown to next execution
    TaskCallback callback;    // Pointer to task function
    uint8_t active;           // Is task active (1) or not (0)
    uint8_t priority;         // 0 = highest priority
} TaskScheduler_Task;
````

---

## 🚀 Quick Start Example

```c
#include "TaskScheduler.h"

void BlinkLED(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void SendMessage(void) {
    printf("Message sent\n");
}

void LogSensor(void) {
    printf("Sensor logged\n");
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();  // Timer 2 will be used to trigger scheduler

    HAL_TIM_Base_Start_IT(&htim2);   // Start TIM2 with interrupt enabled
    TaskScheduler_Init(1000);        // Tick frequency = 1000 Hz (1 ms)

    // Add tasks with desired frequency and priority
    TaskScheduler_AddTask(1, BlinkLED, 1);     // 1 Hz, priority 1
    TaskScheduler_AddTask(10, SendMessage, 0); // 10 Hz, priority 0 (highest)
    TaskScheduler_AddTask(2, LogSensor, 2);    // 2 Hz, priority 2

    while (1) {
        // Main loop remains idle or used for non-time-critical operations
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        TaskScheduler_Update();  // Called every 1 ms
    }
}
```

---

## 🧪 API Reference

### `void TaskScheduler_Init(uint32_t tick_frequency_hz);`

Initializes the task scheduler with the specified tick frequency (in Hz). This must match the frequency of your timer interrupt (e.g., 1000 for 1ms).

### `int TaskScheduler_AddTask(uint32_t frequency_hz, TaskCallback callback, uint8_t priority);`

Adds a new periodic task to the scheduler.

* `frequency_hz`: How often the task runs.
* `callback`: Pointer to the function to be executed.
* `priority`: 0 = highest, higher numbers = lower priority.
* Returns the task ID on success or `-1` on failure.

### `void TaskScheduler_Update(void);`

Called inside the timer interrupt. It checks for tasks due for execution, sorts them by priority, and executes them.

---

## ⚠️ Important Notes

* Task functions are executed **inside the timer interrupt**. Keep them short and non-blocking.
* Avoid using `HAL_Delay()` or blocking I/O inside task callbacks.
* Do not modify shared variables without proper synchronization if you access them in both main loop and tasks.
* If you need longer or more flexible task management, consider using FreeRTOS.

---

## 📌 Tips

* If you want to execute tasks from the main loop instead of from the interrupt, you can modify the scheduler to set task flags and poll them in `while(1)`.
* You can increase `TASKSCHEDULER_MAX_TASKS` in `TaskScheduler.h` to support more concurrent tasks.

---

## 📝 License

This project is licensed under the **MIT License**.

---

## 👨‍💻 Author

This lightweight task scheduler was developed for bare-metal STM32 applications that require predictable task timing without a full RTOS.

Feel free to fork, use, and improve!

---

Let me know if you'd like me to package this into a downloadable `README.md` file or add GitHub Actions, badges, or Doxygen integration.
```
