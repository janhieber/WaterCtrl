/** @file scheduler.h
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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>


// intervals
#define INTERVAL_ALWAYS 0
#define INTERVAL_10MS 10
#define INTERVAL_50MS 50
#define INTERVAL_100MS 100
#define INTERVAL_250MS 100
#define INTERVAL_500MS 500
#define INTERVAL_1S 1000
#define INTERVAL_2S 2000
#define INTERVAL_3S 3000
#define INTERVAL_4S 4000
#define INTERVAL_5S 5000

/*! Define parameters to configure a task */
typedef struct {
  uint16_t Interval;   // how often the task will run
  uint32_t LastTick;   // stores last tick the task was ran
  void (*Func)(void);  // function ptr to task
} TaskType;


/**
 * @brief init the scheduler
 * @param Tasks pointer to tasks array
 * @param count number of lements in tasks array
 */
void initScheduler(TaskType *Tasks, uint8_t count);


/**
 * @brief do the scheduling of tasks
 */
void doScheduling(void);

#endif // SCHEDULER_H



/**
  * @}
  */
/**
  * @}
  */
