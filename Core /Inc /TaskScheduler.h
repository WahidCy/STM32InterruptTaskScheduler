#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "stdint.h"

#define TASKSCHEDULER_MAX_TASKS 10  // Maksimum jumlah task

typedef void (*TaskCallback)(void);

/**
 * @brief Struktur task internal
 */
typedef struct {
    uint32_t interval_ticks;  // Berapa tick antara eksekusi
    uint32_t counter;         // Counter internal untuk menghitung tick
    TaskCallback callback;    // Fungsi callback yang akan dieksekusi
    uint8_t active;           // Status aktif
    uint8_t priority;         // Prioritas eksekusi (0 = tertinggi)
} TaskScheduler_Task;

/**
 * @brief Inisialisasi scheduler
 * @param tick_frequency_hz Frekuensi update dari timer (Hz), misalnya 1000 Hz
 */
void TaskScheduler_Init(uint32_t tick_frequency_hz);

/**
 * @brief Tambahkan task
 * @param frequency_hz Frekuensi eksekusi task (misal: 10 = 10 Hz)
 * @param callback Fungsi callback task
 * @param priority Prioritas task (0 = paling tinggi)
 * @return ID task jika sukses, -1 jika gagal
 */
int TaskScheduler_AddTask(uint32_t frequency_hz, TaskCallback callback, uint8_t priority);

/**
 * @brief Dipanggil setiap tick (biasanya dari timer interrupt)
 */
void TaskScheduler_Update(void);

#endif // TASKSCHEDULER_H
