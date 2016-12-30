/** @file scheduler.c
 *  @brief Scheduler stuff
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup Stuff
  * @{
  */

/** @addtogroup Scheduler
  * @{
  */

#include <scheduler.h>

#include <stm32f1xx_hal.h>


#include <main.h>
#include <stdint.h>

static uint32_t tick = 0;
static TaskType *TaskPtr;
static uint8_t TaskIndex = 0;
static uint8_t NumTasks = 0;

void initScheduler(TaskType *Tasks, uint8_t count) {
    TaskPtr = Tasks;
    NumTasks = count;
    Log(LogDebug, "Scheduler: %d tasks scheduled", NumTasks);
}

void doScheduling(void) {
    tick = HAL_GetTick();
    for (TaskIndex = 0; TaskIndex < NumTasks; TaskIndex++) {
        if (unlikely(TaskPtr[TaskIndex].Interval == 0))
            (*TaskPtr[TaskIndex].Func)();
        else if ((tick - TaskPtr[TaskIndex].LastTick) >=
                 TaskPtr[TaskIndex].Interval) {
            TaskPtr[TaskIndex].LastTick = tick;
            (*TaskPtr[TaskIndex].Func)();
        }
    }
}

/**
  * @}
  */
/**
  * @}
  */
