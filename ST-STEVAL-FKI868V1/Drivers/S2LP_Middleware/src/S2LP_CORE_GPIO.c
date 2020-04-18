/**
 * @file    S2LP_CORE_GPIO.c
 * @author  LowPower RF BU - AMG
 * @version 4.0.0
 * @date    March, 2020
 * @brief   This file provides all the low level API to manage SDK eval pin to drive GPIOs.
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
 * <h2><center>&copy; COPYRIGHT 2020 STMicroelectronics</center></h2>
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "S2LP_Middleware_Config.h"
#include "S2LP_CORE_GPIO.h"

/** @addtogroup S2LP_CORE_STM32			S2LP CORE - STM32
* @{
*/

/** @defgroup S2LP_CORE_GPIO					S2LP CORE GPIO
  * @brief  S2-LP GPIO handling module.
  * This module exports all the main operations to deal with GPIO.
  * @details See the file <i>@ref S2LP_CORE_GPIO.h</i> for more details.
  * @{
*/

/**
 * @brief  M2S GPio Port array
 */

GPIO_TypeDef* vectpxM2SGpioPort[4] = {M2S_GPIO_0_PORT_NUCLEO,M2S_GPIO_1_PORT_NUCLEO,M2S_GPIO_2_PORT_NUCLEO,M2S_GPIO_3_PORT_NUCLEO};

/**
 * @brief  M2S GPio Pin array
 */
static const uint16_t s_vectnM2SGpioPin[4] = {
        M2S_GPIO_0_PIN,
        M2S_GPIO_1_PIN,
        M2S_GPIO_2_PIN,
        M2S_GPIO_3_PIN
};

/**
 * @brief  M2S GPio Speed array
 */
static const uint32_t s_vectxM2SGpioSpeed[4] = {
        M2S_GPIO_0_SPEED,
        M2S_GPIO_1_SPEED,
        M2S_GPIO_2_SPEED,
        M2S_GPIO_3_SPEED
};

/**
 * @brief  M2S GPio PuPd array
 */
static const uint32_t s_vectxM2SGpioPuPd[4] = {
        M2S_GPIO_0_PUPD,
        M2S_GPIO_1_PUPD,
        M2S_GPIO_2_PUPD,
        M2S_GPIO_3_PUPD
};

/**
 * @brief  M2S Exti Mode array
 */
static const uint32_t s_vectxM2sGpioExtiMode[4] = {
        M2S_GPIO_0_EXTI_MODE,
        M2S_GPIO_1_EXTI_MODE,
        M2S_GPIO_2_EXTI_MODE,
        M2S_GPIO_3_EXTI_MODE
};


/**
 * @brief  M2S Exti IRQn array
 */
static const IRQn_Type s_vectcM2SGpioExtiIrqn[4] = {
        M2S_GPIO_0_EXTI_IRQN,
        M2S_GPIO_1_EXTI_IRQN,
        M2S_GPIO_2_EXTI_IRQN,
        M2S_GPIO_3_EXTI_IRQN
};


/** @defgroup S2LP_CORE_GPIO_Functions			S2LP CORE GPIO exported functions
* @{
*/



/**
 * @brief  Configures MCU GPIO and EXTI Line for GPIOs.
 * @param  xGpio Specifies the GPIO to be configured.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 *         @arg M2S_GPIO_SDN: GPIO_SDN
 * @param  xGpioMode Specifies GPIO mode.
 *         This parameter can be one of following parameters:
 *         @arg M2S_MODE_GPIO_IN: MCU GPIO will be used as simple input.
 *         @arg M2S_MODE_EXTI_IN: MCU GPIO will be connected to EXTI line with interrupt
 *         generation capability.
 *         @arg M2S_MODE_GPIO_OUT: MCU GPIO will be used as simple output.
 * @retval None.
 */
void S2LP_Middleware_GpioInit(M2SGpioPin xGpio, M2SGpioMode xGpioMode)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Check the parameters */
  assert_param(IS_M2S_GPIO_PIN(xGpio));
  assert_param(IS_M2S_GPIO_MODE(xGpioMode));

  switch(xGpio)
  {
  case M2S_GPIO_0:
    M2S_GPIO_0_CLOCK_NUCLEO();
    break;
  case M2S_GPIO_1:
    M2S_GPIO_1_CLOCK_NUCLEO();
    break;
  case M2S_GPIO_2:
    M2S_GPIO_2_CLOCK_NUCLEO();
    break;
  case M2S_GPIO_3:
    M2S_GPIO_3_CLOCK_NUCLEO();
    break;
  }

  /* Configures MCU GPIO */
  switch (xGpioMode) {
  case M2S_MODE_GPIO_OUT:
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    break;
  case M2S_MODE_GPIO_IN:
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    break;
  case M2S_MODE_EXTI_IN:
    GPIO_InitStructure.Mode = s_vectxM2sGpioExtiMode[xGpio];
    break;
  default:
    break;
  }

  GPIO_InitStructure.Pin = s_vectnM2SGpioPin[xGpio];
  GPIO_InitStructure.Pull = s_vectxM2SGpioPuPd[xGpio];
  GPIO_InitStructure.Speed = s_vectxM2SGpioSpeed[xGpio];

  HAL_GPIO_Init(vectpxM2SGpioPort[xGpio], &GPIO_InitStructure);

  if (xGpioMode == M2S_MODE_EXTI_IN) {
    __HAL_GPIO_EXTI_CLEAR_IT(s_vectnM2SGpioPin[xGpio]);
  }
}

/**
 * @brief  Enables or disables the interrupt on GPIO .
 * @param  xGpio Specifies the GPIO whose priority shall be changed.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @param  nPreemption Specifies Preemption Priority.
 * @param  nSubpriority Specifies Subgroup Priority.
 * @param  xNewState Specifies the State.
 *         This parameter can be one of following parameters:
 *         @arg 0: Interrupt is disabled
 *         @arg any value != 0: Interrupt is enabled
 * @retval None.
 */
void S2LP_Middleware_GpioInterruptCmd(M2SGpioPin xGpio, uint8_t nPreemption, uint8_t nSubpriority, uint8_t enable)
{
  if (enable)  {
    HAL_NVIC_SetPriority(s_vectcM2SGpioExtiIrqn[xGpio], nPreemption, nSubpriority);
    HAL_NVIC_EnableIRQ(s_vectcM2SGpioExtiIrqn[xGpio]);
  }
  else {
    HAL_NVIC_DisableIRQ(s_vectcM2SGpioExtiIrqn[xGpio]);
  }
}

/**
 * @brief  Enables or disables trigger on rising edge for that GPIO .
 * @param  xGpio Specifies the GPIO.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @param  xNewState Specifies the State.
 *         This parameter can be one of following parameters:
 *         @arg 0: Rising trigger is disabled
 *         @arg any value != 0: Rising trigger is enabled
 * @retval None.
 */
void S2LP_Middleware_GpioTriggerRising(M2SGpioPin xGpio, uint8_t enable)
{
  if(enable)
    EXTI->RTSR |= (uint16_t)s_vectnM2SGpioPin[xGpio];
  else
    EXTI->RTSR &= ~(uint16_t)s_vectnM2SGpioPin[xGpio];
}

/**
 * @brief  To assert if the rising edge IRQ is enabled for that GPIO .
 * @param  xGpio Specifies the GPIO.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @retval  Specifies the State.
 *         @arg 0: Rising trigger is disabled
 *         @arg 1: Rising trigger is enabled
 */
uint8_t S2LP_Middleware_GetTriggerRising(M2SGpioPin xGpio)
{
  if(EXTI->RTSR & (uint16_t)s_vectnM2SGpioPin[xGpio])
    return 1;

  return 0;
}

/**
 * @brief  Enables or disables trigger on falling edge for that GPIO .
 * @param  xGpio Specifies the GPIO.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @param  xNewState Specifies the State.
 *         This parameter can be one of following parameters:
 *         @arg 0: Falling trigger is disabled
 *         @arg any value != 0: Falling trigger is enabled
 * @retval None.
 */
void S2LP_Middleware_GpioTriggerFalling(M2SGpioPin xGpio, uint8_t enable)
{
  if(enable)
    EXTI->FTSR |= (uint16_t)s_vectnM2SGpioPin[xGpio];
  else
    EXTI->FTSR &= ~(uint16_t)s_vectnM2SGpioPin[xGpio];
}

/**
 * @brief  To assert if the falling edge IRQ is enabled for that GPIO .
 * @param  xGpio Specifies the GPIO.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @retval  Specifies the State.
 *         @arg 0: Falling trigger is disabled
 *         @arg 1: Falling trigger is enabled
 */
uint8_t S2LP_Middleware_GpioGetTriggerFalling(M2SGpioPin xGpio)
{
  if(EXTI->FTSR & (uint16_t)s_vectnM2SGpioPin[xGpio])
    return 1;

  return 0;
}

/**
 * @brief  Returns the level of a specified GPIO.
 * @param  xGpio Specifies the GPIO to be read.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @retval Level of the GPIO. This parameter can be:
 *         1 or 0.
 */
uint8_t S2LP_Middleware_GpioGetLevel(M2SGpioPin xGpio)
{
  /* Gets the GPIO level */
  GPIO_PinState ret = HAL_GPIO_ReadPin(vectpxM2SGpioPort[xGpio], s_vectnM2SGpioPin[xGpio]);

  return ((ret==GPIO_PIN_SET) ? 1 : 0);
}

/**
 * @brief  Sets the level of a specified GPIO.
 * @param  xGpio Specifies the GPIO to be set.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 *         @arg M2S_GPIO_SDN: SDN
 * @param  FlagStatus Level of the GPIO. This parameter can be:
 *         SET or RESET.
 * @retval None.
 */
void S2LP_Middleware_GpioSetLevel(M2SGpioPin xGpio, uint8_t level)
{
  /* Sets the GPIO level */
  HAL_GPIO_WritePin(vectpxM2SGpioPort[xGpio], s_vectnM2SGpioPin[xGpio], (level == 0) ? GPIO_PIN_RESET: GPIO_PIN_SET);
}

/**
 * @brief  Gets the GPIO_PIN of the M2SGpioPin.
 * @param  xGpio: M2S GPIO.
 * @retval uint16_t GPIO_PIN value.
 */
uint32_t S2LP_Middleware_GpioGetPin(M2SGpioPin xGpio)
{
  return s_vectnM2SGpioPin[xGpio];
}

void S2LPShutdownInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  S2LP_M2S_SDN_CLOCK();

  /* Configures MCU GPIO */
  GPIO_InitStructure.Pin   = S2LP_M2S_SDN_PIN;
  GPIO_InitStructure.Mode  = S2LP_M2S_SDN_MODE;
  GPIO_InitStructure.Pull  = S2LP_M2S_SDN_PUPD;
  GPIO_InitStructure.Speed = S2LP_M2S_SDN_SPEED;

  HAL_GPIO_Init(S2LP_M2S_SDN_PORT, &GPIO_InitStructure);

  S2LPShutdownEnter();
}

void S2LPShutdownEnter(void)
{
  /* Puts high the GPIO connected to shutdown pin */
  HAL_GPIO_WritePin(S2LP_M2S_SDN_PORT, S2LP_M2S_SDN_PIN, GPIO_PIN_SET);
}

void S2LPShutdownExit(void)
{
  /* Puts low the GPIO connected to shutdown pin */
  HAL_GPIO_WritePin(S2LP_M2S_SDN_PORT, S2LP_M2S_SDN_PIN, GPIO_PIN_RESET);

  /* Delay to allow the circuit POR, about 700 us */
  for(volatile uint32_t i=0;i<0x1E00;i++);
}

uint8_t S2LPShutdownCheck(void)
{
  /* Gets the GPIO level */
  return (uint8_t)HAL_GPIO_ReadPin(S2LP_M2S_SDN_PORT, S2LP_M2S_SDN_PIN);
}

void S2LPIRQInit(void)
{
  S2LP_Middleware_GpioInit((M2SGpioPin)S2LP_GPIO_IRQ_PIN, M2S_MODE_EXTI_IN);
}

void S2LPIRQEnable(uint8_t state, uint8_t edge_direction)
{
  S2LP_Middleware_GpioInit((M2SGpioPin)S2LP_GPIO_IRQ_PIN, M2S_MODE_EXTI_IN);
  S2LP_Middleware_GpioTriggerRising((M2SGpioPin)S2LP_GPIO_IRQ_PIN, edge_direction);
  S2LP_Middleware_GpioInterruptCmd((M2SGpioPin)S2LP_GPIO_IRQ_PIN, 0x00, 0x00, state);
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


/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
