/**
* @file    Platform_Configuration_NucleoF4xx.h
* @author  AMS RF application team
* @version V1.0.0
* @date    December, 2018
* @brief   This file contains definitions for STM32F4xx_NUCLEO Eval Platform
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*
* <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIGURATION_H
#define __PLATFORM_CONFIGURATION_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#if !defined (USE_STM32F4xx_NUCLEO)
 #define USE_STM32F4xx_NUCLEO
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SDK_EVAL_NUCLEO_VER				0x84

/*****************************************************************************/
/*                              BUTTON(s) SECTION                            */
/*****************************************************************************/
  /**
  * @brief Buttons definitions
  */
#define PUSH_BUTTON1_PIN				GPIO_PIN_13
#define PUSH_BUTTON1_GPIO_PORT			GPIOC
#define PUSH_BUTTON1_GPIO_CLK()			__GPIOC_CLK_ENABLE()
#define PUSH_BUTTON1_EXTI_IRQn			EXTI15_10_IRQn
#define PUSH_BUTTON1_EXTI_IRQ_HANDLER		EXTI15_10_IRQHandler
#define PUSH_BUTTON1_KEY				BUTTON_1
#define PUSH_BUTTON1_IRQ_PREEMPTION_PRIORITY	0
#define PUSH_BUTTON1_IRQ_SUB_PRIORITY		0


/*****************************************************************************/
/*                              UART SECTION                                 */
/*****************************************************************************/
  /**
  * @brief UART definitions
  */
#define VCOM_YES						0
#define VCOM_NO						1
#define VCOM_PRESENT					VCOM_YES

#define SDK_EVAL_UART_BAUDRATE			115200

#define SDK_EVAL_UARTx					USART2
#define SDK_EVAL_UARTx_AF				GPIO_AF7_USART2
#define SDK_EVAL_UARTx_PORT				GPIOA
#define SDK_EVAL_UART_RX_PIN				GPIO_PIN_2
#define SDK_EVAL_UART_TX_PIN				GPIO_PIN_3

#define NUCLEO_UARTx_RX_QUEUE_SIZE			(400)
#define NUCLEO_UARTx_TX_QUEUE_SIZE			(400)

#define NUCLEO_UARTx					USART2
#define NUCLEO_UARTx_AF					GPIO_AF7_USART2
#define NUCLEO_UARTx_PORT				GPIOA
#define NUCLEO_UARTx_RX_PIN				GPIO_PIN_2
#define NUCLEO_UARTx_TX_PIN				GPIO_PIN_3

#define NUCLEO_UARTx_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE()
#define NUCLEO_UARTx_GPIO_CLK_DISABLE()		__GPIOA_CLK_DISABLE()
#define NUCLEO_UARTx_CLK_ENABLE()			__USART2_CLK_ENABLE()
#define NUCLEO_UARTx_IRQHandler			USART2_IRQHandler
#define NUCLEO_UARTx_IRQn				USART2_IRQn
#define NUCLEO_UARTx_PRIORITY				2

#define NUCLEO_UARTx_TX_DMA_CHANNEL_IRQHandler	DMA1_Stream6_IRQHandler
#define NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn		DMA1_Stream6_IRQn

#define NUCLEO_UARTx_TX_DMA_CHANNEL			DMA_CHANNEL_4
#define NUCLEO_UARTx_TX_DMA_STREAM			DMA1_Stream6

#define NUCLEO_UARTx_RX_DMA_CHANNEL_IRQHandler	DMA1_Stream5_IRQHandler
#define NUCLEO_UARTx_RX_DMA_CHANNEL_IRQn		DMA1_Stream5_IRQn
#define NUCLEO_UARTx_RX_DMA_CHANNEL			DMA_CHANNEL_4
#define NUCLEO_UARTx_RX_DMA_STREAM			DMA1_Stream5

#define NUCLEO_UARTx_DMA_CLK_ENABLE()		__DMA1_CLK_ENABLE()


/*****************************************************************************/
/*                               TIM and CK SECTION                          */
/*****************************************************************************/
#define NUCLEO_TIMx_PRIORITY				1

#define TIM6						TIM4
#define TIM6_IRQn						TIM4_IRQn
#define __HAL_RCC_TIM6_CLK_ENABLE()			__TIM4_CLK_ENABLE()
#define TIM6_IRQHandler					TIM4_IRQHandler

#define TIM7						TIM5
#define TIM7_IRQn						TIM5_IRQn
#define __HAL_RCC_TIM7_CLK_ENABLE()			__TIM5_CLK_ENABLE()
#define TIM7_IRQHandler					TIM5_IRQHandler

#define STM32_RTC_IRQHandler				RTC_WKUP_IRQHandler
#define STM32_RTC_IRQn					RTC_WKUP_IRQn

#define STM32_GPIO_CLK_DISABLE()			{__GPIOB_CLK_DISABLE();__GPIOC_CLK_DISABLE();__GPIOD_CLK_DISABLE();__GPIOA_CLK_DISABLE();}
#define STM32_GPIO_CLK_ENABLE()			{__GPIOA_CLK_ENABLE();__GPIOB_CLK_ENABLE();__GPIOC_CLK_ENABLE();__GPIOD_CLK_ENABLE();}

/*****************************************************************************/
/*                               LED SECTION                                 */
/*****************************************************************************/
  /**
  * @brief LEDs definitions
  */
#define LEDn						1
#define SDK_EVAL_LED1_PIN				GPIO_PIN_5
#define SDK_EVAL_LED1_PORT				GPIOA
#define SDK_EVAL_LED1_CLK				__GPIOA_CLK_ENABLE

#ifdef __cplusplus
}
#endif

#endif /* __PLATFORM_CONFIGURATION_H */

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE*****/
