/**
 * @file    S2LP_CORE_GPIO.h
 * @author  LowPower RF BU - AMG
 * @version 4.0.0
 * @date    March, 2020
 * @brief   GPIO Configuration used in the Software Development Kit eval board to drive GPIOs.
 * @details
 *
 * This module exports API to manage the GPIO from the micro
 * side.
 * The following example shows how to configure the shutdown pin
 * and the GPIO 3 as an EXTI input.
 *
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
 * <h2><center>&copy; COPYRIGHT 2020 STMicroelectronics</center></h2>
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S2LP_CORE_GPIO_H
#define __S2LP_CORE_GPIO_H

#include "cube_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup S2LP_CORE_GPIO
 * @{
 */

/**
 * @defgroup S2LP_CORE_GPIO_MACROS				S2LP CORE GPIO exported MACROs
 * @{
 */

/**
 * @brief  MCU GPIO pin enumeration for GPIO
 */
typedef enum
{
  M2S_GPIO_0     = 0x00, /*!< GPIO_0 selected */
  M2S_GPIO_1     = 0x01, /*!< GPIO_1 selected */
  M2S_GPIO_2     = 0x02, /*!< GPIO_2 selected */
  M2S_GPIO_3     = 0x03, /*!< GPIO_3 selected */
  M2S_GPIO_SDN   = 0x04,  /*!< GPIO_SDN selected */
}M2SGpioPin;

#define IS_M2S_GPIO_PIN(PIN) (((PIN) == M2S_GPIO_0) || \
                              ((PIN) == M2S_GPIO_1) || \
                              ((PIN) == M2S_GPIO_2) || \
                              ((PIN) == M2S_GPIO_3) || \
                              ((PIN) == M2S_GPIO_SDN))

/**
 * @brief  MCU GPIO pin working mode for GPIO
 */
typedef enum
{
  M2S_MODE_GPIO_IN  = 0x00,   /*!< Work as GPIO input */
  M2S_MODE_EXTI_IN,           /*!< Work as EXTI */
  M2S_MODE_GPIO_OUT,          /*!< Work as GPIO output */
}M2SGpioMode;

#define IS_M2S_GPIO_MODE(MODE) (((MODE) == M2S_MODE_GPIO_IN) || \
                                ((MODE) == M2S_MODE_EXTI_IN) || \
                                ((MODE) == M2S_MODE_GPIO_OUT))

/**
 * @}
 */


/**
 * @}
 */

/**
 * @brief  Configures MCU GPIO and EXTI Line for GPIOs.
 * @param  None.
 * @retval None.
 */
void S2LPIRQInit(void);

/**
 * @brief  Enables or disables the interrupt on GPIO .
 * @param  state Specifies the State.
 *         This parameter can be one of following parameters:
 *         @arg 0: disable
 *         @arg >0: enable
 * @param  edge_direction Specifies the sensitive edge.
 *         This parameter can be one of following parameters:
 *         @arg 0: falling edge
 *         @arg 1: rising edge
 * @retval None.
 */
void S2LPIRQEnable(uint8_t state, uint8_t edge_direction);

/**
 * @brief  Init shutdown pin on the uC and set it to 1 (Shutdown active).
 * @param  None.
 * @retval None.
 */
void S2LPShutdownInit(void);

/**
 * @brief  Puts at logic 1 the SDN pin.
 * @param  None.
 * @retval None.
 */
void S2LPShutdownEnter(void);

/**
 * @brief  Put at logic 0 the SDN pin.
 * @param  None.
 * @retval None.
 */
void S2LPShutdownExit(void);

/**
* @brief  check the logic (0 or 1) at the SDN pin.
* @param  None.
* @retval FlagStatus.
*/
uint8_t S2LPShutdownCheck(void);

void S2LP_Middleware_GpioInit(M2SGpioPin xGpio, M2SGpioMode xGpioMode);
void S2LP_Middleware_GpioInterruptCmd(M2SGpioPin xGpio, uint8_t nPreemption, uint8_t nSubpriority, uint8_t enable);
void S2LP_Middleware_GpioTriggerRising(M2SGpioPin xGpio, uint8_t enable);
uint8_t S2LP_Middleware_GetTriggerRising(M2SGpioPin xGpio);
void S2LP_Middleware_GpioTriggerFalling(M2SGpioPin xGpio, uint8_t enable);
uint8_t S2LP_Middleware_GpioGetTriggerFalling(M2SGpioPin xGpio);
uint8_t S2LP_Middleware_GpioGetLevel(M2SGpioPin xGpio);
void S2LP_Middleware_GpioSetLevel(M2SGpioPin xGpio, uint8_t level);
uint32_t S2LP_Middleware_GpioGetPin(M2SGpioPin xGpio);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
