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

typedef enum _activeMotor {
    MOT_ACTIVE_NONE = 0x00,
    MOT_ACTIVE_0,
    MOT_ACTIVE_1,
    MOT_ACTIVE_2,
    MOT_ACTIVE_3,
    MOT_ACTIVE_4,
    MOT_ACTIVE_INVALID = 0xff
} eActiveMotor;

void motInit(void);

void motTask100ms();

void motTask5s();

int motControl(int motor, int time, int max_level);

#endif  // MOTORS_H

/**
  * @}
  */
