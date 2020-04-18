/**
 * @file    SDK_EVAL_Button.h
 * @author  AMS VMA RF application team
 * @version V1.1.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage buttons for eval board.
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
#ifndef __SDK_EVAL_BUTTON_H
#define __SDK_EVAL_BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup SDK_EVAL_STM32       SDK EVAL STM32
 * @{
 */

/** @addtogroup SDK_EVAL_Button         SDK EVAL Button
 * @{
 */

/** @addtogroup SDK_EVAL_Button_Exported_Types          SDK EVAL Button Exported Types
 * @{
 */

/**
 * @brief  Enumeration of SDK EVAL Buttons
 * There is only one user configurable button on Nucleo boards
 * but here two buttons are configured the same in order to keep
 * compatibility with BlueNRG-1/2 evaluation boards
 *
 */
typedef enum {
  BUTTON_1 = 0,
  BUTTON_2 = BUTTON_1
} SdkEvalButton;

/**
 * @brief  Enumeration of SDK EVAL Buttons IRQ mode
 */
typedef enum {
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} SdkEvalButtonIrq;

/**
* @brief  Joystick for SDK EVAL enumeration
*/
typedef enum
{
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5

} SdkEvalJoyState;

/**
 * @}
 */

/** @defgroup SDK_EVAL_Button_Exported_Constants                SDK EVAL Button Exported Constants
 * @{
 */
#define EVAL_BOARD_BUTTONS          (1)

/**
 * @}
 */

/** @defgroup SDK_EVAL_Button_Exported_Macros           SDK EVAL Button Exported Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Button_Exported_Functions        SDK EVAL Button Exported Functions
 * @{
 */
uint32_t Get_ButtonGpioPin(SdkEvalButton xButton);

/**
* @brief  Configures Buttons.
* @param  xButton Specifies the Button to be configured, @ref SdkEvalButton
*         This parameter can be one of following parameters:
*         @arg BUTTON_1: Push1 button
*         @arg BUTTON_2: Push2 button
*         but in Nucleo boards is always set to BUTTON_1
* @retval None.
*/
void SdkEvalPushButtonInit(SdkEvalButton xButton);

/**
* @brief  Configures buttons IRQ mode.
* @param  xButton Specifies the Button to be configured, @ref SdkEvalButton
*         This parameter can be one of following parameters:
*         @arg BUTTON_1: Push1 button
* @param  SdkEvalButtonIrq Specifies the IRQ mode to be configured,
*         This parameter can be one of following parameters:
*         @arg IRQ_ON_RISING_EDGE: IRQ on rising edge
*         @arg IRQ_ON_FALLING_EDGE: IRQ on falling edge
*         @arg IRQ_ON_BOTH_EDGE: IRQ on both edges
* @retval None.
*/
void SdkEvalPushButtonIrq(SdkEvalButton xButton, SdkEvalButtonIrq xButtonIrq);

/**
* @brief  Returns the selected Button state.
* @param  xButton Specifies the Button to be configured, @ref SdkEvalButton
*         This parameter can be one of following parameters:
*         @arg BUTTON_1: Push1 button
* @retval FlagStatus: error status of the button @ref FlagStatus
*         This parameter can be: SET or RESET.
*/
FlagStatus SdkEvalPushButtonGetState(SdkEvalButton xButton);

/**
* @brief  Get the pending bit state.
* @param  xButton Specifies the Button to be configured, @ref SdkEvalButton
*         This parameter can be one of following parameters:
*         @arg BUTTON_1: Push1 button
* @retval FlagStatus: error status of the button @ref FlagStatus
*         This parameter can be: SET or RESET.
*/
FlagStatus SdkEvalPushButtonGetITPendingBit(SdkEvalButton xButton);

/**
* @brief  Clear the pending bit state.
* @param  xButton Specifies the Button to be configured, @ref SdkEvalButton
*         This parameter can be one of following parameters:
*         @arg BUTTON_1: Push1 button
*         @arg BUTTON_2: Push2 button
* @retval None
*/
void SdkEvalPushButtonClearITPendingBit(SdkEvalButton xButton);

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

#endif /* __SDK_EVAL_BUTTON_H */
/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
