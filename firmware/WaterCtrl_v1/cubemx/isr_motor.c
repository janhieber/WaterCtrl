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

#define MOT_PERIOD 1800
uint16_t setState(eActiveState state);
uint16_t getPulse();

volatile uint16_t newPulseLength;
eActiveState aState;

uint16_t getPulse(){
    return (newPulseLength*(uint16_t)100)/(uint16_t)MOT_PERIOD;
}

uint16_t setState(eActiveState state)
{
    aState = state;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (TIM2 == htim->Instance) {

        switch (aState) {
        case MOT_STATE_RAMPUP:
            if (newPulseLength < 1800)
                newPulseLength += 100;
            else
                aState = MOT_STATE_HIGH;
            break;
        case MOT_STATE_HIGH:

            break;
        case MOT_STATE_RAMPDOWN:
            if (newPulseLength > 0)
                newPulseLength -= 100;
            else
                aState = MOT_STATE_IDLE;
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
