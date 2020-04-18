/**
 * @file    SDK_EVAL_Led.h
 * @author  AMS VMA RF application team
 * @version V2.0.0
 * @date    November 29, 2018
 * @brief   This file provides all the low level API to manage LEDs for eval board.
 * @details
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
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_LED_H
#define __SDK_EVAL_LED_H

/* Includes ------------------------------------------------------------------*/

#include "SDK_EVAL_Config.h"

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup SDK_EVAL_BlueNRG1       SDK EVAL BlueNRG1
 * @{
 */

/** @addtogroup SDK_EVAL_Led            SDK EVAL Led
 * @{
 */

/** @defgroup SDK_EVAL_Led_Exported_Types               SDK EVAL Led Exported Types
 * @{
 */

/**
 * @brief  Enumeration of SDK EVAL LEDs
 * Only one LED on Nucleo Boards, but three LED here defined to mantain compatibility
 * with BlueNRG-1/2 evaluation boards.
 */
typedef enum {
  LED1 = 0,
  LED2 = 0,
  LED3 = 0
} SdkEvalLed;

/**
 * @}
 */

/** @defgroup SDK_EVAL_Led_Exported_Constants               SDK EVAL Led Exported Constants
 * @{
 */

#define EVAL_BOARD_LEDS LEDn

/**
 * @brief  SDK Eval leds polarity
 */
#define LED_ON                          GPIO_PIN_SET
#define LED_OFF                         GPIO_PIN_RESET

/**
 * @}
 */

/**
 * @defgroup SDK_EVAL_Led_Exported_Macros                   SDK EVAL Led Exported Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Led_Exported_Functions               SDK EVAL Led Exported Functions
 * @{
 */
uint32_t Get_LedGpioPin(SdkEvalLed xLed);
void SdkEvalLedInit(SdkEvalLed xLed);
void SdkEvalLedOn(SdkEvalLed xLed);
void SdkEvalLedOff(SdkEvalLed xLed);
void SdkEvalLedToggle(SdkEvalLed xLed);
FlagStatus SdkEvalLedGetState(SdkEvalLed xLed);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SDK_EVAL_LED_H */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
