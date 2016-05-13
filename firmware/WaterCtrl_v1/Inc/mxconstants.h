/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define RPI_NRST_Pin GPIO_PIN_13
#define RPI_NRST_GPIO_Port GPIOC
#define PWM_A0_Pin GPIO_PIN_14
#define PWM_A0_GPIO_Port GPIOC
#define PWM_A1_Pin GPIO_PIN_15
#define PWM_A1_GPIO_Port GPIOC
#define PWM_A2_Pin GPIO_PIN_0
#define PWM_A2_GPIO_Port GPIOA
#define PWM_Pin GPIO_PIN_1
#define PWM_GPIO_Port GPIOA
#define FREQ_Pin GPIO_PIN_0
#define FREQ_GPIO_Port GPIOB
#define SEN_A0_Pin GPIO_PIN_1
#define SEN_A0_GPIO_Port GPIOB
#define SEN_A1_Pin GPIO_PIN_10
#define SEN_A1_GPIO_Port GPIOB
#define SEN_A2_Pin GPIO_PIN_11
#define SEN_A2_GPIO_Port GPIOB
#define SEN_ENABLE_Pin GPIO_PIN_15
#define SEN_ENABLE_GPIO_Port GPIOB
#define LED_ERR_Pin GPIO_PIN_9
#define LED_ERR_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */



/*! array size calculation */
#define ARRAYSIZE(x) (sizeof(x) / sizeof((x)[0]))

/*! optimizations */
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)




/*! enable/disable watchdog */
//#define USEWATCHDOG  // enable this in production



// these are macros to have a unique way of debug output
/** use at begin of init function */
#define INITBEGIN		printf_("  :: Init %s system...", SYSNAME)
/** use at end of init function */
#define INITEND		printf_(" [DONE]\r\n")
/** use before endless loop in process */
#define PROCRUNNING	printf_("  :: %s running\r\n", __func__)



#define VER_MAJOR (uint8_t)00
#define VER_MINOR (uint8_t)01



// debug messages
#define D(...)		printf_("DD %s:%s: ", SYSNAME, __func__); \
					printf_(__VA_ARGS__); \
					printf_("\r\n");
// info messages
#define I(...)		printf_("II %s:%s: ", SYSNAME, __func__); \
					printf_(__VA_ARGS__); \
					printf_("\r\n");
// warning messages
#define W(...)		printf_("WW %s:%s: ", SYSNAME, __func__); \
					printf_(__VA_ARGS__); \
					printf_("\r\n");
// error messages
#define E(...)		printf_("EE %s:%s: ", SYSNAME, __func__); \
					printf_(__VA_ARGS__); \
					printf_("\r\n");


// this is a dummy for the old log functions,
// we recode them some time later
#define Log(...)


// protocoll definitions for SPI transfer
// this is whats in the first byte
#define SPI_ID_NOP 0x00
#define SPI_ID_ERROR 0x01
#define SPI_ID_MESSAGE 0x02
#define SPI_ID_MOTOR_CTRL 0x10
#define SPI_ID_MOTOR_RESP 0x11
#define SPI_ID_MOIST_REQ 0x12
#define SPI_ID_MOIST_VALUE 0x13

// error codes
#define ERROR_HARDFAULT 0x01
#define ERROR_STACKOVERFLOW 0x02

#define MESSAGE_PING 0x01
#define MESSAGE_PONG 0x02

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
