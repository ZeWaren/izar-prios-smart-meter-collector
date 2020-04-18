/**
* @file    Platform_Configuration_NucleoL0xx.h
* @author  AMS RF application team
* @version V1.0.0
* @date    December, 2018
* @brief   This file contains definitions for STM32L0xx_NUCLEO Eval Platform
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
* <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIGURATION_H
#define __PLATFORM_CONFIGURATION_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

#if !defined (USE_STM32L0xx_NUCLEO)
 #define USE_STM32L0xx_NUCLEO
#endif

#ifdef __cplusplus
extern "C" {
#endif

  /**
  * @brief MCU XO frequency(in KHz) definition
  */
#define CLOCK_FREQUENCY					32000
#define CLOCK_FREQUENCY_MHZ				32

/*****************************************************************************/
/*                              BUTTON(s) SECTION                            */
/*****************************************************************************/
  /**
  * @brief Buttons definitions
  */
#define PUSH_BUTTON1_PIN                      	GPIO_PIN_13
#define PUSH_BUTTON1_GPIO_PORT                	GPIOC
#define PUSH_BUTTON1_GPIO_CLK()               	__GPIOC_CLK_ENABLE()
#define PUSH_BUTTON1_EXTI_IRQn                	EXTI4_15_IRQn
#define PUSH_BUTTON1_EXTI_IRQ_HANDLER         	EXTI4_15_IRQHandler
#define PUSH_BUTTON1_KEY                      	BUTTON_1
#define PUSH_BUTTON1_IRQ_PREEMPTION_PRIORITY  	0
#define PUSH_BUTTON1_IRQ_SUB_PRIORITY         	0


/*****************************************************************************/
/*                              UART SECTION                                 */
/*****************************************************************************/
  /**
  * @brief UART definitions
  */
#define VCOM_YES                        		0
#define VCOM_NO                         		1
#define VCOM_PRESENT                    		VCOM_YES
#define SDK_EVAL_UART_BAUDRATE          		115200
#define SDK_EVAL_UART_TX_PIN            		GPIO_PIN_3
#define SDK_EVAL_UART_RX_PIN            		GPIO_PIN_2
#define NUCLEO_UARTx_RX_QUEUE_SIZE      		(400)
#define NUCLEO_UARTx_TX_QUEUE_SIZE      		(400)

#define NUCLEO_UARTx                            USART2
#define NUCLEO_UARTx_AF                         GPIO_AF4_USART2
#define NUCLEO_UARTx_PORT                       GPIOA
#define NUCLEO_UARTx_RX_PIN                     GPIO_PIN_2
#define NUCLEO_UARTx_TX_PIN                     GPIO_PIN_3

#define NUCLEO_UARTx_GPIO_CLK_ENABLE()          __GPIOA_CLK_ENABLE()
#define NUCLEO_UARTx_GPIO_CLK_DISABLE()         __GPIOA_CLK_DISABLE()
#define NUCLEO_UARTx_CLK_ENABLE()              	__USART2_CLK_ENABLE()
#define NUCLEO_UARTx_IRQHandler			USART2_IRQHandler
#define NUCLEO_UARTx_IRQn                       USART2_IRQn
#define NUCLEO_UARTx_PRIORITY                   2

#define NUCLEO_UARTx_TX_DMA_CHANNEL_IRQHandler  DMA1_Channel4_5_6_7_IRQHandler
#define NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn        DMA1_Channel4_5_6_7_IRQn
#define NUCLEO_UARTx_TX_DMA_CHANNEL             DMA1_Channel7
#define NUCLEO_UARTx_TX_DMA_REQUEST             DMA_REQUEST_4

#define NUCLEO_UARTx_RX_DMA_CHANNEL_IRQHandler  DMA1_Channel4_5_6_7_IRQHandler
#define NUCLEO_UARTx_RX_DMA_CHANNEL_IRQn        DMA1_Channel4_5_6_7_IRQn
#define NUCLEO_UARTx_RX_DMA_CHANNEL             DMA1_Channel6
#define NUCLEO_UARTx_RX_DMA_REQUEST             DMA_REQUEST_4

#define NUCLEO_UARTx_DMA_CLK_ENABLE()           __DMA1_CLK_ENABLE()


/*****************************************************************************/
/*                               TIM and CK SECTION                          */
/*****************************************************************************/

/* Remapping for STM32L0x3 */
#define NUCLEO_TIMx_PRIORITY                    1
#define TIM1                                    TIM21
#define TIM1_IRQn                               TIM21_IRQn
#define __HAL_RCC_TIM1_CLK_ENABLE()     		__TIM21_CLK_ENABLE()

#define TIM3                                    TIM2
#define TIM3_IRQn                               TIM2_IRQn
#define __HAL_RCC_TIM3_CLK_ENABLE()     		__TIM2_CLK_ENABLE()


#define TIM4                                    TIM21
#define TIM4_IRQn                               TIM1_IRQn
#define __HAL_RCC_TIM4_CLK_ENABLE()     		__TIM21_CLK_ENABLE()

#define TIM5                                    TIM22
#define TIM5_IRQn                               TIM22_IRQn
#define __HAL_RCC_TIM5_CLK_ENABLE()             __TIM22_CLK_ENABLE()

#define TIM7                                    TIM6
#define TIM7_IRQn                               TIM6_IRQn
#define __HAL_RCC_TIM7_CLK_ENABLE()             __TIM6_CLK_ENABLE()

#define TIM6_IRQHandler                         TIM6_DAC_IRQHandler

#define STM32_RTC_IRQHandler				RTC_IRQHandler
#define STM32_RTC_IRQn					RTC_IRQn

#define  STM32_GPIO_CLK_DISABLE()			{__GPIOB_CLK_DISABLE();__GPIOC_CLK_DISABLE();__GPIOD_CLK_DISABLE();__GPIOA_CLK_DISABLE();}
#define  STM32_GPIO_CLK_ENABLE()			{__GPIOA_CLK_ENABLE();__GPIOB_CLK_ENABLE();__GPIOC_CLK_ENABLE();__GPIOD_CLK_ENABLE();}


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

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
