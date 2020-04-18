/**
* @file    SDK_EVAL_Led.c
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


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Led.h"

/**
* @brief  Get the IO pin associate to the required LED
* @param  xLed: LED name @ref SdkEvalLed
*         This parameter can be one of following parameters:
*         @arg LED1
* @retval led_pin: IO pin name associated to that LED
*/
uint32_t Get_LedGpioPin(SdkEvalLed xLed)
{
  uint32_t led_pin;
  switch (xLed)
  {
    case(LED1):
	led_pin = SDK_EVAL_LED1_PIN;
	break;
  default:
    led_pin = SDK_EVAL_LED1_PIN;
    break;
  }
  return (led_pin);
}

/**
* @brief  Configures LEDs.
* @param  xLed Specifies the Led to be configured @ref SdkEvalLed.
*         This parameter can be one of following parameters:
*         @arg LED1
* @retval None.
*/
void SdkEvalLedInit(SdkEvalLed xLed)
{
  SDK_EVAL_LED1_CLK();
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.Pin = SDK_EVAL_LED1_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(SDK_EVAL_LED1_PORT, &GPIO_InitStructure);

  HAL_GPIO_WritePin(SDK_EVAL_LED1_PORT, SDK_EVAL_LED1_PIN, GPIO_PIN_RESET);
}

/**
* @brief  Turns selected LED On.
* @param  xLed Specifies the Led to be set on  @ref SdkEvalLed.
*         This parameter can be one of following parameters:
*         @arg LED1
* @retval None.
*/
void SdkEvalLedOn(SdkEvalLed xLed)
{
  HAL_GPIO_WritePin(SDK_EVAL_LED1_PORT, SDK_EVAL_LED1_PIN, GPIO_PIN_SET);
}

/**
* @brief  Turns selected LED Off.
* @param  xLed Specifies the Led to be set off @ref SdkEvalLed.
*         This parameter can be one of following parameters:
*         @arg LED1
* @retval None.
*/
void SdkEvalLedOff(SdkEvalLed xLed)
{
  HAL_GPIO_WritePin(SDK_EVAL_LED1_PORT, SDK_EVAL_LED1_PIN, GPIO_PIN_RESET);
}

/**
* @brief  Toggles the selected LED.
* @param  xLed Specifies the Led to be toggled @ref SdkEvalLed.@ref SdkEvalLed.
*         This parameter can be one of following parameters:
*         @arg LED1
* @retval None.
*/
void SdkEvalLedToggle(SdkEvalLed xLed)
{
  SDK_EVAL_LED1_PORT->ODR ^= SDK_EVAL_LED1_PIN;
}

/**
* @brief  Returns the status of a specified led.
* @param  xLed Specifies the Led to be read @ref SdkEvalLed.
*         This parameter can be one of following parameters:
*         @arg LED1
* @retval FlagStatus: status of the LED @ref FlagStatus
*         This parameter can be: SET or RESET.
*/
FlagStatus SdkEvalLedGetState(SdkEvalLed xLed)
{
  if(SDK_EVAL_LED1_PORT->IDR & SDK_EVAL_LED1_PIN)
    return RESET;
  else
    return SET;
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
