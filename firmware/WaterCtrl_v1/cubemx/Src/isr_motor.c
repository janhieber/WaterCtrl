/** @file isr_moisture.c
 *  @brief Frequency measurement
 *
 *  @author Dropedout
 */

/** @addtogroup MotorControl
  * @{
  */

#include "stm32f1xx_hal.h"
#include <log.h>
#include "motors.h"

#define MOT_PERIOD 1800u
#define MOT_STEP 90u
uint16_t setState(eActiveState state);
uint16_t getPulse();
void setPulse(uint16_t pulse);

volatile uint16_t newPulseLength;
volatile uint16_t maxPulseLength;
eActiveState aState;

uint16_t getPulse(){
    return (newPulseLength*(uint16_t)100)/(uint16_t)MOT_PERIOD;
}
void setPulse(uint16_t pulse) {
    maxPulseLength = pulse*(uint16_t)MOT_PERIOD/(uint16_t)100;
}

uint16_t setState(eActiveState state)
{
    aState = state;
    return aState;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (TIM2 == htim->Instance) {
        switch (aState) {
        case MOT_STATE_RAMPUP:
            if (newPulseLength < (maxPulseLength - MOT_STEP))
                newPulseLength += MOT_STEP;
            else {
                newPulseLength = maxPulseLength;
                aState = MOT_STATE_HIGH;
            }
            break;
        case MOT_STATE_HIGH:

            break;
        case MOT_STATE_RAMPDOWN:
            if (newPulseLength > MOT_STEP)
                newPulseLength -= MOT_STEP;
            else {
                aState = MOT_STATE_IDLE;
                newPulseLength = 0;
            }
            break;
        default:
            break;
        }
        htim->Lock = HAL_LOCKED;
        htim->Instance->CCR2 = newPulseLength;
        htim->Lock = HAL_UNLOCKED;
    }
}

/**
  * @}
  */
