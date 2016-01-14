/** @file motors.c
 *  @brief Prototypes for motor module
 *
 *  @author Dropedout
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup MotorControl
  * @{
  */



#include <log.h>
#include <motors.h>

static eActiveMotor g_activeMotor = MOT_ACTIVE_INVALID;
static TIM_HandleTypeDef * ptrTimer;

void motInit(TIM_HandleTypeDef * ref) {
    g_activeMotor = MOT_ACTIVE_NONE;
    Log(LogInfo, "init motor control system");

    ptrTimer = ref;
    motControlStart(1,0,0);
}

void motTask100ms() {}

int motControlStart(eActiveMotor motor, int time, int max_level) {
    int retval = -1;
    if ((motor < MOT_ACTIVE_0) || (motor > MOT_ACTIVE_4)) {
        Log(LogError, "wrong motor selected!");
    } else {
        // calc a ramp

    }

    if (g_activeMotor != motor) {
        HAL_TIM_Base_Start_IT(ptrTimer);

        HAL_TIM_PWM_Start_IT(ptrTimer,TIM_CHANNEL_2);
        g_activeMotor = MOT_ACTIVE_1;
    } else {
        Log(LogInfo, "motor already active.");
    }

    return retval;
}
#include <stm32f1xx_hal.h>
/**
  * @}
  */
