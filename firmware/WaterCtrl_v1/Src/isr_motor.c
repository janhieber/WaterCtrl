/** @file isr_moisture.c
 *  @brief Frequency measurement
 *
 *  @author Dropedout
 */

/** @addtogroup MotorControl
  * @{
  */

#include "stm32f1xx_hal.h"
#include "motors.h"

uint32_t newPulseLength;
uint32_t maxPulseLength;
uint32_t stepPulseUp;
uint32_t stepPulseDown;
eActiveState isrState;

void setPulseStepDown(uint32_t step) {
    stepPulseDown = step;
}

void setPulseStepUp(uint32_t step) {
    stepPulseUp = step;
}

uint32_t getPulse(){
    if (isrState == MOT_STATE_HIGH)
        return (maxPulseLength*100)/MOT_PWM_RELOAD_CNTR;
    return (newPulseLength*100)/MOT_PWM_RELOAD_CNTR;
}

void setPulse(uint32_t pulse) {
    maxPulseLength = (pulse*MOT_PWM_RELOAD_CNTR)/100;
}

void setState(eActiveState state) {
    isrState = state;
}

eActiveState getState() {
    return isrState;
}

void custom_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (TIM2 == htim->Instance) {
        switch (isrState) {
        case MOT_STATE_RAMPUP:
            if (newPulseLength < (maxPulseLength - stepPulseUp))
                newPulseLength += stepPulseUp;
            else {
                newPulseLength = maxPulseLength;
                isrState = MOT_STATE_HIGH;
            }
            break;
        case MOT_STATE_RAMPDOWN:
            if (newPulseLength > stepPulseDown)
                newPulseLength -= stepPulseDown;
            else {
                isrState = MOT_STATE_IDLE;
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
