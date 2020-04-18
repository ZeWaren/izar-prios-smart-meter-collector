/**
 * @file    S2LP_TCXO_AUTO.c
 * @author  LowPower RF BU - AMG
 * @version V1.0.0
 * @date    March, 2020
 * @brief   This file provides an implementation of TCXO function when
 * TCXO is autodetected by the EEPROM in ST Evaluation kits. Users are normally
 * not interested in this file since it is useful only for ST Eval kit.
 * Please have a look at S2LP_TCXO_YES.c for a template file for users.
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
 */

/* Includes ------------------------------------------------------------------*/
#include "S2LP_Middleware_Config.h"
#include "S2LP_AUX_TCXO.h"

/** @addtogroup S2LP_MIDDLEWARE_STM32                     S2LP Middleware - STM32
* @{
*/

/** @defgroup S2LP_AUX_TCXO_AUTO						S2LP AUX TCXO AUTO
  * @brief  S2-LP TCXO handling module.
  * This module exports all the main operations to deal with TCXO.
  * @{
*/

/** @defgroup S2LP_AUX_TCXO_AUTO_Functions			S2LP AUX TCXO Auto exported functions
* @{
*/


/**
 * @brief  TCXO initialization function.
 * This function automatically sets the TCXO according to the information stored in the device EEPROM.
 * This function can be redefined for special needs.
 * @param  None
 * @retval None
 */
__weak void TCXO_Init()
{
  /* Not implemented: at the moment there isn't any eval kit with TCXO */
}


/**
 * @brief  TCXO Operation function.
 * This function configures the TCXO according to the desired status.
 * This function can be redefined for special needs.
 * @param  operation Specifies the operation to perform.
 *         This parameter can be one of following parameters:
 *         @arg TCXO_ON: Turns on TCXO
 *         @arg TCXO_OFF: Turns off TCXO
 * @retval None
 */
__weak void TCXO_Operation(TCXO_OperationType operation)
{
  /* Not implemented: at the moment there isn't any eval kit with TCXO */
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
