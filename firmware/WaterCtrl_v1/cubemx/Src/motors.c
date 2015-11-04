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

void motTask100ms()
{

}

bool motControl(int motor, int time, int max_level)
{
    bool retval = false;
    if( (motor < MOT_ACTIVE_1) ||  (motor > MOT_ACTIVE_5))
    {
        LogUart(LogError,"wrong motor selected!);
    }
    else
    {

    }



    if (g_activeMotor != MOT_ACTIVE_NONE)
    {

    }

    return retval;
}


/**
  * @}
  */
