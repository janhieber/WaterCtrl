/*
 * isr_moisture.c
 *
 *  Created on: Apr 28, 2015
 *      Author: micha
 */

#include <stm32f0xx_conf.h>

uint_fast64_t getFrequencyOfChannel(int channel);


uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
uint16_t CaptureNumber = 0;
uint32_t Capture = 0;
uint_fast64_t TIM1Freq = 0;

void TIM1_CC_IRQHandler(void);


uint_fast64_t getFrequencyOfChannel(int channel)
{
	return TIM1Freq;
}

/**
  * @brief  This function handles TIM1 Capture Compare interrupt request.
  * @param  None
  * @retval None
  */
void TIM1_CC_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM1, TIM_IT_CC2) == SET)
  {
    /* Clear TIM1 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    if(CaptureNumber == 0)
    {
      /* Get the Input Capture value */
      IC3ReadValue1 = TIM_GetCapture2(TIM1);
      CaptureNumber = 1;
    }
    else if(CaptureNumber == 1)
    {
      /* Get the Input Capture value */
      IC3ReadValue2 = TIM_GetCapture2(TIM1);

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
      TIM1Freq = (uint_fast64_t)((uint32_t) SystemCoreClock / Capture)*8;
      CaptureNumber = 0;
    }
  }
}
