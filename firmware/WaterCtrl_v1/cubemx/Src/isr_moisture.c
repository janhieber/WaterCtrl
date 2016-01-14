/** @file isr_moisture.c
 *  @brief Frequency measurement
 *
 *  @author Dropedout
 */

/** @addtogroup MoistureMeasure
  * @{
  */

#include "stm32f1xx_hal.h"
#include <log.h>

uint32_t getFrequencyOfChannel();
void resetFrequencyOfChannel();


volatile uint32_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
volatile uint32_t CaptureNumber = 0;
volatile uint32_t Capture = 0;
volatile uint32_t TIM1Freq = 0;

volatile uint16_t newPulseLength;

#define PERI_PRESCALAR 1
#define TIMER_CLOCK (SystemCoreClock/((uint32_t)PERI_PRESCALAR))
#define IC_SAMPLE_FILTER 0x04u

uint32_t getFrequencyOfChannel()
{
    return TIM1Freq;
}
uint32_t getCapture()
{
    return Capture;
}
void resetFrequencyOfChannel()
{
    CaptureNumber = 0;
    TIM1Freq = 0;
}

/**
 * @brief  This function handles TIM1 Capture Compare interrupt request.
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (TIM3 == htim->Instance)  {
        if(CaptureNumber == 0) {
            /* Get the Input Capture value */
            htim->Lock = HAL_LOCKED;
            IC3ReadValue1 = htim->Instance->CCR3;
            htim->Lock = HAL_UNLOCKED;
            CaptureNumber = 1;
        } else if(CaptureNumber == 1) {
            /* Get the Input Capture value */
            htim->Lock = HAL_LOCKED;
            IC3ReadValue2 = htim->Instance->CCR3;
            htim->Lock = HAL_UNLOCKED;

            /* Capture computation */
            if (IC3ReadValue2 > IC3ReadValue1) {
                Capture = (IC3ReadValue2 - IC3ReadValue1);
            } else if (IC3ReadValue2 < IC3ReadValue1) {
                Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2);
            } else {
                Capture = 0;
            }
            /* Frequency computation */

            TIM1Freq = ((uint32_t) TIMER_CLOCK /  Capture)*(uint32_t)IC_SAMPLE_FILTER;
            CaptureNumber = 0;
        }
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (TIM2 == htim->Instance) {
        if (newPulseLength < 1800)
            newPulseLength += 100;
        else
            newPulseLength = 100;

        htim->Lock = HAL_LOCKED;
        htim->Instance->CCR2 = newPulseLength;
        htim->Lock = HAL_UNLOCKED;
    }
}

/**
  * @}
  */
