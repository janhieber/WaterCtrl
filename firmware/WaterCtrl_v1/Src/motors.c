/** @file motors.c
 *  @brief Prototypes for motor module
 *
 *  * Uses TIM2 to generate moderate pwm output based on AHP frequency.
 *
 *  @todo:
 * 	Make input pin configurable. We need to measure 8 sensor inputs
 * 	Find algorithm which less stress the output
 * 	- lower measurement period time
 * 	- combine capture inputs to gain more precision/range
 *
 * 	Multiplex inputs
 * 	- GPIO for input selection
 * 	- disable capture while inputs switch
 *
 * 	@attention:
 * 	Concurrent access to global frequency by interrupt and output loop!
 *
 * 	Questions for implementation:
 * 	1. which frequency range is expected -> prescaler
 * 	2. How to implement self stopping measure interval?
 * 		a. How to stop the measurement between channels
 *  3. Calculate delta pulse length to linear ramp up/down
 *  @author Dropedout
 *  @author Jan Hieber <mail@janhieber.net>
 */

#include <stdint.h>
#include <stdbool.h>

/** @addtogroup MotorControl
  * @{
  */

/* Private define ------------------------------------------------------------*/

#include <spicomm.h>
#include <log.h>
#include <motors.h>
#include <broker.h>

static eActiveState g_activeState = MOT_STATE_IDLE;
static eActiveMotor g_activeMotor = MOT_ACTIVE_INVALID;
static uint16_t g_maxPulse = 0;
static uint32_t g_stepUp = MOT_PWM_RELOAD_CNTR;
static uint32_t g_stepDown = MOT_PWM_RELOAD_CNTR;
static int32_t g_activeCounter = 0;
static TIM_HandleTypeDef * ptrTimer;

/* Private function prototypes -----------------------------------------------*/
extern void setState(eActiveState state);
extern void setPulse(uint16_t maxPulse);
extern void setPulseStepDown(uint8_t step);
extern void setPulseStepUp(uint8_t step);

extern eActiveState getState();
extern uint16_t getPulse();

/* Private functions ---------------------------------------------------------*/
static void motBrokerMessage(char *buf, uint8_t length);
const char * getStateString();

int motControlStart(eActiveMotor motor, stMotCfg *cfg) {
    int retval = -1;
    if ((motor < MOT_ACTIVE_0) || (motor > MOT_ACTIVE_4)) {
        Log(LogError, "wrong motor selected!");
    } else {
        if (g_activeMotor != motor) {
            g_activeCounter = cfg->high_time*10;
            g_maxPulse = cfg->max_level;

            if ((MOT_MAX_FLANK_TIME > cfg->up_time )
                    && (MOT_MAX_FLANK_TIME > cfg->down_time )) {
                if (0 == cfg->up_time)
                    g_stepUp = (MOT_PWM_RELOAD_CNTR * g_maxPulse)/100;
                else
                    g_stepUp = ((MOT_PWM_RELOAD_CNTR/(cfg->up_time*MOT_PWM_FREQ))*g_maxPulse)/100;

                if (0 == cfg->down_time)
                    g_stepDown = (MOT_PWM_RELOAD_CNTR * g_maxPulse)/100;
                else
                    g_stepDown = ((MOT_PWM_RELOAD_CNTR/(cfg->up_time*MOT_PWM_FREQ))*g_maxPulse)/100;

                g_activeMotor = motor;

                switch (g_activeMotor) {
                case MOT_ACTIVE_0:
                    break;
                case MOT_ACTIVE_1:
                    HAL_GPIO_WritePin(GPIOC,MOT_PWM_PIN_A0,GPIO_PIN_SET);
                    break;
                case MOT_ACTIVE_2:
                    HAL_GPIO_WritePin(GPIOC,MOT_PWM_PIN_A1,GPIO_PIN_SET);
                    break;
                case MOT_ACTIVE_3:
                    HAL_GPIO_WritePin(GPIOC,MOT_PWM_PIN_A0|MOT_PWM_PIN_A1,GPIO_PIN_SET);
                    break;
                case MOT_ACTIVE_4:
                    HAL_GPIO_WritePin(GPIOA,MOT_PWM_PIN_A2,GPIO_PIN_SET);
                    break;
                default:
                    break;
                }

                retval = g_activeMotor;
            } else {
                //Log(LogError, "bad up: %d or down: %d time!",cfg->up_time,cfg->down_time);
                Log(LogError,"bad timing parameter");
            }
        } else {
            Log(LogInfo, "motor already active.");
        }
    }
    return retval;
}

void motBrokerMessage(char *buf, uint8_t length)
{
    stMotCfg cfg = {0,0,0,0};
    char send[SPI_XFER_SIZE];
    switch(buf[0]) {
    case BRK_MSG_SPI_ID_MOT:
    {
        send[1] = 0xac;
        send[0] = buf[1];
        if (length == 5) {
            cfg.max_level = buf[5];
            cfg.high_time = buf[3];
            cfg.up_time = buf[2];
            cfg.down_time = buf[4];

            if (-1 !=  motControlStart(buf[1],&cfg))
            {
                send[1] = 0xab;
            } else {
                send[2] = BRK_ERR_MOTOR_START_FAILED;
            }
        } else {
            send[2] = BRK_ERR_MSG_PARAM_INVALID;
        }
        spiSend(BRK_MSG_SPI_ID_MOT_RSP,send);
        break;
    }
    case BRK_MSG_SPI_ID_MOT_RSP:
        break;
    default:
        break;
    }
}

void motInit(TIM_HandleTypeDef * ref) {
    g_activeMotor = MOT_ACTIVE_NONE;
    Log(LogInfo, "init motor control system");

    ptrTimer = ref;

    HAL_TIM_Base_Start_IT(ptrTimer);

    registerMessage(BRK_MSG_SPI_ID_MOT,motBrokerMessage);
}

void motTask1s() {
    stMotCfg cfg =  {0,0,0,0};
    static uint16_t counter;
    Log(LogDebug,"state : %s",getStateString());
    Log(LogDebug,"motor : %d",g_activeMotor);
    Log(LogDebug,"counter : %d",g_activeCounter);
    Log(LogDebug,"pulse : %d",getPulse());

    if (g_activeMotor == MOT_ACTIVE_NONE)
    {
        cfg.down_time = 5;
        cfg.up_time = 5;
        cfg.high_time = 10;
        cfg.max_level = 100;
        motControlStart((counter % 2)+1,&cfg);
        counter++;
    }
}

void motTask100ms() {
    if (g_activeMotor != MOT_ACTIVE_NONE)
    {
        switch (g_activeState) {
        case MOT_STATE_IDLE:
            setPulse(g_maxPulse);
            setPulseStepUp(g_stepUp);
            setPulseStepDown(g_stepDown);
            g_activeState = MOT_STATE_RAMPUP;
            break;
        case MOT_STATE_RAMPUP:
            setState(MOT_STATE_RAMPUP);
            HAL_TIM_PWM_Start_IT(ptrTimer,TIM_CHANNEL_2);
            g_activeState = MOT_STATE_WAIT_UP;
            break;
        case MOT_STATE_WAIT_UP:
            if (MOT_STATE_HIGH == getState()) {
                g_activeState = MOT_STATE_HIGH;
            }
            break;
        case MOT_STATE_HIGH:
            setState(MOT_STATE_HIGH);
            g_activeCounter--;
            if (g_activeCounter <= 0 )
                g_activeState = MOT_STATE_RAMPDOWN;
            break;
        case MOT_STATE_RAMPDOWN:
            setState(MOT_STATE_RAMPDOWN);
            g_activeState = MOT_STATE_WAIT_DOWN;
            break;
        case MOT_STATE_WAIT_DOWN:
            if (MOT_STATE_IDLE == getState()) {
                g_activeState = MOT_STATE_DONE;
                HAL_TIM_PWM_Stop_IT(ptrTimer,TIM_CHANNEL_2);
            }
            break;
        case MOT_STATE_DONE:
            g_activeMotor = MOT_ACTIVE_NONE;
            g_activeState = MOT_STATE_IDLE;
            HAL_GPIO_WritePin(MOT_PWM_PORT_A2,MOT_PWM_PIN_A2,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOT_PWM_PORT_A1,MOT_PWM_PIN_A0|MOT_PWM_PIN_A1,GPIO_PIN_RESET);
            break;
        default:
            break;
        }
    }
}

const char * getStateString(){
    switch (g_activeState) {
    case MOT_STATE_IDLE:
        return "IDLE";
    case MOT_STATE_RAMPUP:
        return "RAMPUP";
    case MOT_STATE_WAIT_UP:
        return "WAIT_UP";
    case MOT_STATE_HIGH:
        return "HIGH";
    case MOT_STATE_RAMPDOWN:
        return "RAMPDOWN";
    case MOT_STATE_WAIT_DOWN:
        return "WAIT_DOWN";
    case MOT_STATE_DONE:
        return "DONE";
    case MOT_STATE_INVALID:
        return "INVALID";
    default:
        return "unknown";
    }
}
/**
  * @}
  */
