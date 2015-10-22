/** @file isr_moisture.c
 *  @brief Frequency measurement
 *
 *  @author Dropedout
 */

/** @addtogroup MoistureMeasure
  * @{
  */

#include "stm32f1xx_hal.h"

uint_fast64_t getFrequencyOfChannel(int channel);


uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
uint16_t CaptureNumber = 0;
uint32_t Capture = 0;
uint_fast64_t TIM1Freq = 0;


uint_fast64_t getFrequencyOfChannel(int channel)
{
    return TIM1Freq;
}

/**
 * @brief  This function handles TIM1 Capture Compare interrupt request.
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    /* Clear TIM1 Capture compare interrupt pending bit */
    if (TIM3 == htim->Instance)
    {
        if(CaptureNumber == 0)
        {
            /* Get the Input Capture value */
            htim->Lock = HAL_LOCKED;
            IC3ReadValue1 = htim->Instance->CCR3;
            htim->Lock = HAL_UNLOCKED;
            CaptureNumber = 1;
        }
        else if(CaptureNumber == 1)
        {
            /* Get the Input Capture value */
            htim->Lock = HAL_LOCKED;
            IC3ReadValue2 = htim->Instance->CCR3;
            htim->Lock = HAL_UNLOCKED;

            /* Capture computation */
            if (IC3ReadValue2 > IC3ReadValue1)
            {
                Capture = (IC3ReadValue2 - IC3ReadValue1);
            }
            else if (IC3ReadValue2 < IC3ReadValue1)
            {
                Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2);
            }
            else
            {
                Capture = 0;
            }
            /* Frequency computation */
            TIM1Freq = ((uint_fast64_t) SystemCoreClock /  Capture)*8ul;
            CaptureNumber = 0;
        }
    }
}


/**
  * @}
  */
