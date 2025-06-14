#include "TaskScheduler.h"

static TaskScheduler_Task task_list[TASKSCHEDULER_MAX_TASKS];
static uint32_t tick_per_second = 0; // Frekuensi scheduler (Hz)

void TaskScheduler_Init(uint32_t tick_frequency_hz) {
    tick_per_second = tick_frequency_hz;
    for (int i = 0; i < TASKSCHEDULER_MAX_TASKS; i++) {
        task_list[i].active = 0;
    }
}

int TaskScheduler_AddTask(uint32_t frequency_hz, TaskCallback callback, uint8_t priority) {
    if (frequency_hz == 0 || frequency_hz > tick_per_second || callback == 0) {
        return -1;
    }

    uint32_t interval_ticks = tick_per_second / frequency_hz;
    if (interval_ticks == 0) interval_ticks = 1;

    for (int i = 0; i < TASKSCHEDULER_MAX_TASKS; i++) {
        if (!task_list[i].active) {
            task_list[i].interval_ticks = interval_ticks;
            task_list[i].counter = interval_ticks;
            task_list[i].callback = callback;
            task_list[i].priority = priority;
            task_list[i].active = 1;
            return i;
        }
    }

    return -1;
}

void TaskScheduler_Update(void) {
    int ready_task_indices[TASKSCHEDULER_MAX_TASKS];
    int ready_count = 0;

    for (int i = 0; i < TASKSCHEDULER_MAX_TASKS; i++) {
        if (task_list[i].active) {
            if (--task_list[i].counter == 0) {
                ready_task_indices[ready_count++] = i;
            }
        }
    }

    // Urutkan task yang siap berdasarkan prioritas (ascending)
    for (int i = 0; i < ready_count - 1; i++) {
        for (int j = i + 1; j < ready_count; j++) {
            if (task_list[ready_task_indices[j]].priority < task_list[ready_task_indices[i]].priority) {
                int temp = ready_task_indices[i];
                ready_task_indices[i] = ready_task_indices[j];
                ready_task_indices[j] = temp;
            }
        }
    }

    // Eksekusi task yang siap
    for (int i = 0; i < ready_count; i++) {
        int idx = ready_task_indices[i];
        task_list[idx].callback();
        task_list[idx].counter = task_list[idx].interval_ticks;
    }
}
