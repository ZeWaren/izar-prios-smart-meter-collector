/**
* @file    S2LP_TCXO.h
* @author  AMG - RF Application team
* @version 1.0.0
* @date    March, 2020
* @brief   TCXO (Temperature controlled crystal oscillator) management
* @details This module exports API to manage a TCXO possibly present on an
* S2-LP board.
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
#ifndef __S2LP_TCXO_H__
#define __S2LP_TCXO_H__

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
  "C" {
#endif

/**
 * @addtogroup S2LP_AUX_TCXO_AUTO
 * @{
 */


/**
 * @defgroup S2LP_AUX_TCXO_AUTO_TYPES				S2LP AUX TCXO Auto exported types
 * @{
 */



/**
 * @brief  Opcode for TCXO management
 */

typedef enum
{
  TCXO_OFF        = 0x00,
  TCXO_ON         = 0x01,
} TCXO_OperationType;

/**
* @brief  Initializes the TCXO control lines. If not present is implemented by dummy function.
* @param  None
* @retval None
*/
void TCXO_Init(void);

/**
* @brief  Turn on/off TCXO.
* @param  operation: operation requested, if TCXO is not present this is implemented by dummy function
* @retval None
*/
void TCXO_Operation(TCXO_OperationType operation);

/**
* @}
*/


/**
* @}
*/

#ifdef __cplusplus
}
#endif


#endif
 /******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

