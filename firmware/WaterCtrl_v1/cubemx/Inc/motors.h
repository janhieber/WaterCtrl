/** @file motors.h
 *  @brief Prototypes for motor module
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup MotorControl
  * @{
  */

#ifndef MOTORS_H
#define MOTORS_H

#include <stm32f1xx_hal.h>

typedef enum _activeMotor {
    MOT_ACTIVE_NONE = 0x00,
    MOT_ACTIVE_0,
    MOT_ACTIVE_1,
    MOT_ACTIVE_2,
    MOT_ACTIVE_3,
    MOT_ACTIVE_4,
    MOT_ACTIVE_INVALID = 0xff
} eActiveMotor;

typedef enum _activeState {
    MOT_STATE_IDLE = 0x00,
    MOT_STATE_RAMPUP,
    MOT_STATE_WAIT_UP,
    MOT_STATE_HIGH,
    MOT_STATE_WAIT_DOWN,
    MOT_STATE_RAMPDOWN,
    MOT_STATE_INVALID = 0xff
} eActiveState;

void motInit(TIM_HandleTypeDef *);

void motTask100ms();

void motTask5s();

int motControlStart(eActiveMotor motor, int time, int max_level);

#endif // MOTORS_H

/**
  * @}
  */
