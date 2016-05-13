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

#define MOT_PWM_PIN_A0 GPIO_PIN_14
#define MOT_PWM_PORT_A0 GPIOC
#define MOT_PWM_PIN_A1 GPIO_PIN_15
#define MOT_PWM_PORT_A1 GPIOC
#define MOT_PWM_PIN_A2 GPIO_PIN_0
#define MOT_PWM_PORT_A2 GPIOA

#define MOT_PWM_FREQ (uint32_t)100 //Hz
#define MOT_PWM_RELOAD_CNTR (uint32_t)20000
#define MOT_MAX_FLANK_TIME (uint32_t)(MOT_PWM_RELOAD_CNTR/MOT_PWM_FREQ)



extern osMessageQId motorCtrlQueue;
extern osPoolId  motorCtrlPool;



typedef struct MotorCmd {
	uint8_t motor;
	uint8_t time;
	uint8_t speed;
}MotorCmd;

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
    MOT_STATE_DONE,
    MOT_STATE_INVALID = 0xff
} eActiveState;

typedef struct _stMotCfg{
    uint8_t up_time;
    uint8_t down_time;
    uint8_t high_time;
    uint8_t max_level;
}stMotCfg;

void motInit(TIM_HandleTypeDef *);

void motTask100ms();

void motTask1s();

int motControlStart(eActiveMotor motor, stMotCfg * cfg);

void procMotor(void const * argument);

#endif // MOTORS_H

/**
  * @}
  */
