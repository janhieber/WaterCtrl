/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
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
#define ANALOG_Pin GPIO_PIN_2
#define ANALOG_GPIO_Port GPIOA
#define DHT22_Pin GPIO_PIN_3
#define DHT22_GPIO_Port GPIOA
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
#define RELAIS_0_Pin GPIO_PIN_3
#define RELAIS_0_GPIO_Port GPIOB
#define RELAIS_1_Pin GPIO_PIN_4
#define RELAIS_1_GPIO_Port GPIOB
#define RELAIS_2_Pin GPIO_PIN_5
#define RELAIS_2_GPIO_Port GPIOB
#define RELAIS_3_Pin GPIO_PIN_6
#define RELAIS_3_GPIO_Port GPIOB
#define RELAIS_4_Pin GPIO_PIN_7
#define RELAIS_4_GPIO_Port GPIOB
#define LED_ERR_Pin GPIO_PIN_9
#define LED_ERR_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */



/*! array size calculation */
#define ARRAYSIZE(a) (sizeof(a) / sizeof(*(a)))


/*! optimizations */
#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif




/*! enable/disable watchdog */
//#define USEWATCHDOG  // enable this in production



// these are macros to have a unique way of debug output
/** use at begin of init function */
#define INITBEGIN		printf_("  :: Init %s system...", SYSNAME)
/** use at end of init function */
#define INITEND		printf_(" [DONE]\r\n")
/** use before endless loop in process */
#define PROCRUNNING	printf_("  :: %s running\r\n", __func__)
/** use before endless loop in process */
#define FUNCRUNNING	printf_("  :: %s caled\r\n", __func__)


#define VER_MAJOR (uint8_t)00
#define VER_MINOR (uint8_t)01

int printf_(const char *format, ...);
int sprintf_(char *buffer, const char *format, ...);

#define printf(...) printf_(__VA_ARGS__)
#define sprintf(...) sprintf_(__VA_ARGS__)

// debug messages
#ifdef DEBUG
#define D(...)		{printf_("DD %s: ",  __func__); \
					printf_(__VA_ARGS__); \
					printf_("\r\n");}
#else
#define D(...)
#endif

// info messages
#define I(...)		{printf_("II %s: ",  __func__); \
					printf_(__VA_ARGS__); \
					printf_("\r\n");}
// warning messages
#define W(...)		{printf_("WW %s: ",  __func__); \
					printf_(__VA_ARGS__); \
					printf_("\r\n");}
// error messages
#define E(...)		{printf_("EE %s: ",  __func__); \
					printf_(__VA_ARGS__); \
					printf_("\r\n");}

#define CHECKPOOL(x) if(x == NULL){ E("no memory available in pool"); }

// this is a dummy for the old log functions,
// we recode them some time later
#define Log(...)
#define LogInfo
#define LogDebug
#define LogError


// protocoll definitions for SPI transfer
// this is whats in the first byte
#define SPI_ID_NOP 0x00
#define SPI_ID_ERROR 0x01
#define SPI_ID_MESSAGE 0x02
#define SPI_ID_MOTOR_CTRL 0x10
#define SPI_ID_MOTOR_RESP 0x11
#define SPI_ID_SEN_MEA_REQ 0x12
#define SPI_ID_SEN_MEA_VALUE 0x13
#define SPI_ID_SEN_TYP_REQ 0x14
#define SPI_ID_SEN_TYP_RES 0x15

// error codes
#define ERROR_HARDFAULT 0x01
#define ERROR_STACKOVERFLOW 0x02

#define MESSAGE_PING 0x01
#define MESSAGE_PONG 0x02

#define MOTOR_RESP_FINISH 0xab
#define MOTOR_RESP_ERROR 0xac

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
