/**
* @file    SDK_EVAL_Button.c
* @author  AMS VMA RF application team
* @version V2.0.0
* @date    December, 2018
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
* <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
*/

/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Button.h"

/** @addtogroup SDK_EVAL_STM32         SDK EVAL STM
* @{
*/

/** @addtogroup SDK_EVAL_Button           SDK EVAL Button
* @{
*/

/** @defgroup SDK_EVAL_Button_Private_TypesDefinitions          SDK EVAL Button Private Types Definitions
* @{
*/

/**
* @}
*/

/** @defgroup SDK_EVAL_Button_Private_Defines                   SDK EVAL Button Private Defines
* @{
*/

/**
* @}
*/

/** @defgroup SDK_EVAL_Button_Private_Macros                    SDK EVAL Button Private Macros
* @{
*/

/**
* @}
*/

/** @defgroup SDK_EVAL_Button_Private_FunctionPrototypes                        SDK EVAL Button Private Function Prototypes
* @{
*/

/**
* @}
*/

/** @defgroup SDK_EVAL_Button_Private_Functions                                 SDK EVAL Button Private Functions
* @{
*/

/* Buttons IRQ Pin */
uint32_t Get_ButtonGpioPin(SdkEvalButton xButton)
{
  uint32_t button_pin;
  switch (xButton)
  {
    case(BUTTON_1):
	button_pin = PUSH_BUTTON1_PIN;
	break;
  default:
    button_pin = PUSH_BUTTON1_PIN;
    break;
  }
  return (button_pin);
}

/**
* @}
*/

/** @defgroup SDK_EVAL_Button_Public_Functions                                 SDK EVAL Button Public Functions
* @{
*/

void SdkEvalPushButtonInit(SdkEvalButton xButton)
{
  PUSH_BUTTON1_GPIO_CLK();
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configures Button pin as input */
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = PUSH_BUTTON1_PIN;
  HAL_GPIO_Init(PUSH_BUTTON1_GPIO_PORT, &GPIO_InitStructure);
}

void SdkEvalPushButtonIrq(SdkEvalButton xButton, SdkEvalButtonIrq xButtonIrq)
{
  PUSH_BUTTON1_GPIO_CLK();

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitTypeDef EXTI_InitStructure;

  /* Configures Button EXTI line */
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = PUSH_BUTTON1_PIN;
  HAL_GPIO_Init(PUSH_BUTTON1_GPIO_PORT, &GPIO_InitStructure);

  if (xButtonIrq == BUTTON_MODE_EXTI)
  {
    /* Configures Button EXTI line */
    EXTI_InitStructure.Mode = GPIO_MODE_IT_FALLING;
    EXTI_InitStructure.Pull = GPIO_NOPULL;
    EXTI_InitStructure.Pin = PUSH_BUTTON1_PIN;
    HAL_GPIO_Init(PUSH_BUTTON1_GPIO_PORT, &EXTI_InitStructure);

    HAL_NVIC_SetPriority(PUSH_BUTTON1_EXTI_IRQn, PUSH_BUTTON1_IRQ_PREEMPTION_PRIORITY, PUSH_BUTTON1_IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(PUSH_BUTTON1_EXTI_IRQn);
  }
}

FlagStatus SdkEvalPushButtonGetState(SdkEvalButton xButton)
{
  GPIO_PinState ret = HAL_GPIO_ReadPin(PUSH_BUTTON1_GPIO_PORT, Get_ButtonGpioPin(BUTTON_1));
  return (FlagStatus)ret;
}

FlagStatus SdkEvalPushButtonGetITPendingBit(SdkEvalButton xButton)
{
  if(__HAL_GPIO_EXTI_GET_IT(Get_ButtonGpioPin(xButton)))
    return SET;
  else
    return RESET;
}

void SdkEvalPushButtonClearITPendingBit(SdkEvalButton xButton)
{
  __HAL_GPIO_EXTI_CLEAR_IT(Get_ButtonGpioPin(xButton));
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


/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
