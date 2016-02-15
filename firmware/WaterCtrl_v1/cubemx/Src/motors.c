/** @file motors.c
 *  @brief Prototypes for motor module
 *
 *  @author Dropedout
 *  @author Jan Hieber <mail@janhieber.net>
 */

#include <stdint.h>
#include <stdbool.h>

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

#include <spicomm.h>
#include <log.h>
#include <motors.h>
#include <broker.h>

static eActiveState g_activeState = MOT_STATE_IDLE;
static eActiveMotor g_activeMotor = MOT_ACTIVE_INVALID;
static uint16_t g_maxPulse = 0;
static int32_t g_activeCounter = 0;
static TIM_HandleTypeDef * ptrTimer;

/* Private function prototypes -----------------------------------------------*/
extern uint16_t setState(eActiveState state);
extern uint16_t getPulse();
extern void setPulse(uint16_t maxPulse);
/* Private functions ---------------------------------------------------------*/
static void motBrokerMessage(char *buf, uint8_t length);
const char * getStateString();

void motBrokerMessage(char *buf, uint8_t length)
{
    char send[SPI_XFER_SIZE];
    switch(buf[0]) {
    case BRK_MSG_SPI_ID_MOT:
        if (-1 != (send[0] = motControlStart(buf[1],buf[3],buf[5])))
        {
            send[1] = 0xab;
        } else {
            send[1] = 0xac;
        }
        send[0] = buf[1];
        spiSend(BRK_MSG_SPI_ID_MOT_RSP,send);
        break;
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

    motControlStart(1,10,80);

    registerMessage(BRK_MSG_SPI_ID_MOT,motBrokerMessage);
}

void motTask1s() {
    static uint16_t counter;
    //Log(LogDebug,"state : %s",getStateString());
    //Log(LogDebug,"motor : %d",g_activeMotor);
    //Log(LogDebug,"counter : %d",g_activeCounter);
    //Log(LogDebug,"pulse : %d",getPulse());

    if (g_activeMotor == MOT_ACTIVE_NONE)
    {
        motControlStart((counter % 5)+1,10,70 );
        counter++;
    }
}

void motTask100ms() {
    if (g_activeMotor != MOT_ACTIVE_NONE)
    {
        switch (g_activeState) {
        case MOT_STATE_IDLE:
            g_activeState = MOT_STATE_RAMPUP;
            setPulse(g_maxPulse);
            break;
        case MOT_STATE_RAMPUP:
            setState(MOT_STATE_RAMPUP);
            HAL_TIM_PWM_Start_IT(ptrTimer,TIM_CHANNEL_2);
            g_activeState = MOT_STATE_WAIT_UP;
            break;
        case MOT_STATE_WAIT_UP:
            if (g_maxPulse <= getPulse()) {
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
            if (0 == getPulse()) {
                g_activeState = MOT_STATE_DONE;

                HAL_TIM_PWM_Stop_IT(ptrTimer,TIM_CHANNEL_2);
            }
            break;
        case MOT_STATE_DONE:
            g_activeMotor = MOT_ACTIVE_NONE;
            g_activeState = MOT_STATE_IDLE;
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
            HAL_GPIO_WritePin(MOT_PWM_PORT_A2,MOT_PWM_PIN_A2,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOT_PWM_PORT_A1,MOT_PWM_PIN_A0|MOT_PWM_PIN_A1,GPIO_PIN_RESET);
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
