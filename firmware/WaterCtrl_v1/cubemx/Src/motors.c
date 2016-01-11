/** @file motors.c
 *  @brief Prototypes for motor module
 *
 *  @author Dropedout
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup MotorControl
  * @{
  */

#include <stm32f1xx_hal.h>

#include <log.h>
#include <motors.h>

static eActiveMotor g_activeMotor = MOT_ACTIVE_INVALID;

void motInit(void) {
    g_activeMotor = MOT_ACTIVE_NONE;
    Log(LogInfo, "init motor control system");
}

void motTask100ms() {}

int motControl(int motor, int time, int max_level) {
    int retval = -1;
    if ((motor < MOT_ACTIVE_0) || (motor > MOT_ACTIVE_4)) {
        Log(LogError, "wrong motor selected!");
    } else {
        // calc a ramp
    }

    if (g_activeMotor != MOT_ACTIVE_NONE) {
        // HAL_TIMEx_PWMN_Start(TIM2,)
        HAL_TIMEx_PWMN_Start(TIM2,TIM_CHANNEL_2);
    } else {
        Log(LogInfo, "motor already active.");
    }

    return retval;
}

/**
  * @}
  */
