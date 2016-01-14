/** @file motors.c
 *  @brief Prototypes for motor module
 *
 *  @author Dropedout
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup MotorControl
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define MOT_PWM_PIN_A0 GPIO_PIN_14
#define MOT_PWM_PORT_A0 GPIOC
#define MOT_PWM_PIN_A1 GPIO_PIN_15
#define MOT_PWM_PORT_A1 GPIOC
#define MOT_PWM_PIN_A2 GPIO_PIN_0
#define MOT_PWM_PORT_A2 GPIOA

#define MOT_PWM_PIN_ENABLE GPIO_PIN_15
#define MOT_PWM_PORT_ENABLE GPIOB

#include <log.h>
#include <motors.h>

static eActiveState g_activeState = MOT_STATE_IDLE;
static eActiveMotor g_activeMotor = MOT_ACTIVE_INVALID;
static uint16_t g_maxPulse = 0;
static int32_t g_activeCounter = 0;
static TIM_HandleTypeDef * ptrTimer;

/* Private function prototypes -----------------------------------------------*/
extern uint16_t setState(eActiveState state);
extern uint16_t getPulse();
/* Private functions ---------------------------------------------------------*/

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
    case MOT_STATE_INVALID:
        return "INVALID";
    default:
        return "unknown";
    }
}

void motInit(TIM_HandleTypeDef * ref) {
    g_activeMotor = MOT_ACTIVE_NONE;
    Log(LogInfo, "init motor control system");

    ptrTimer = ref;

    HAL_TIM_Base_Start_IT(ptrTimer);

    motControlStart(1,10,80);
}

void motTask5s() {
    Log(LogDebug,"state : %s",getStateString());
    Log(LogDebug,"motor : %d",g_activeMotor);
    Log(LogDebug,"counter : %d",g_activeCounter);
    Log(LogDebug,"pulse : %d",getPulse());
}

void motTask100ms() {
    if (g_activeMotor != MOT_ACTIVE_NONE)
    {
        switch (g_activeState) {
        case MOT_STATE_IDLE:
            g_activeState = MOT_STATE_RAMPUP;
            break;
        case MOT_STATE_RAMPUP:
            setState(MOT_STATE_RAMPUP);
            HAL_TIM_PWM_Start_IT(ptrTimer,TIM_CHANNEL_2);
            g_activeState = MOT_STATE_WAIT_UP;
            break;
        case MOT_STATE_WAIT_UP:
            if (g_maxPulse <= getPulse())
                g_activeState = MOT_STATE_HIGH;
            break;
        case MOT_STATE_HIGH:
            g_activeCounter--;
            if (g_activeCounter <= 0 )
                g_activeState = MOT_STATE_RAMPDOWN;
            break;
        case MOT_STATE_RAMPDOWN:
            setState(MOT_STATE_RAMPDOWN);
            g_activeState = MOT_STATE_WAIT_DOWN;
            break;
        case MOT_STATE_WAIT_DOWN:
            if (10 >= getPulse()) {
                g_activeState = MOT_STATE_IDLE;
                g_activeMotor = MOT_ACTIVE_NONE;
                HAL_TIM_PWM_Stop_IT(ptrTimer,TIM_CHANNEL_2);
            }
            break;
        default:
            break;
        }
    }
}

int motControlStart(eActiveMotor motor, int time, int max_level) {
    int retval = -1;
    if ((motor < MOT_ACTIVE_0) || (motor > MOT_ACTIVE_4)) {
        Log(LogError, "wrong motor selected!");
    } else {

        if (g_activeMotor != motor) {
            g_activeMotor = motor;
            HAL_GPIO_WritePin(GPIOA,MOT_PWM_PIN_A2,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOC,MOT_PWM_PIN_A0|MOT_PWM_PIN_A1,GPIO_PIN_RESET);
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
            g_activeCounter = time*10;
            g_maxPulse = max_level;

        } else {
            Log(LogInfo, "motor already active.");
        }
    }

    return retval;
}
#include <stm32f1xx_hal.h>
/**
  * @}
  */
