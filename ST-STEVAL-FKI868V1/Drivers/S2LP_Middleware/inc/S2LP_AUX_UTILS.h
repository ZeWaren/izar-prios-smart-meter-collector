/**
* @file    S2LP_EVAL_Utils.h
* @author  VMA division - AMS
* @version 4.0.1
* @date    March, 2020
* @brief   Identification and utils functions for S2LP DK.
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

#ifndef __S2LP_AUX_UTILS_H
#define __S2LP_AUX_UTILS_H

/* Includes ------------------------------------------------------------------*/
#include "S2LP_AUX_EEPROM.h"

/**
 * @addtogroup S2LP_AUX_UTILS
 * @{
 */

/**
* @defgroup SDK_S2LP_MANAGEMENT_FUNCTIONS    SDK S2LP Management Functions
* @{
*/

/**
 * @defgroup S2LP_AUX_UTILS_TYPES				S2LP AUX UTILS exported types
 * @{
 */

typedef enum
{
  RANGE_EXT_NONE = 0,
  RANGE_EXT_SKY_66100_11 = 1,
  RANGE_EXT_SKYWORKS_SE2435L = 2,
  RANGE_EXT_SKYWORKS_SKY66420 = 3,
  RANGE_EXT_CUSTOM = 4
} RangeExtType;

typedef enum
{
  S2LP_CUT_2_1 = 0x91,
  S2LP_CUT_2_0 = 0x81,
  S2LP_CUT_3_0 = 0xC1,
} S2LPCutType;

/**
* @}
*/

/**
* @}
*/


S2LPCutType S2LPManagementGetCut(void);
uint8_t S2LPManagementGetBand(void);
void S2LPManagementSetBand(uint8_t band);
RangeExtType S2LPManagementGetRangeExtender(void);
void S2LPManagementSetRangeExtender(RangeExtType value);
uint32_t S2LPManagementComputeXtalFrequency(void);
void S2LPManagementIdentificationRFBoard(void);
uint8_t S2LPManagementGetTcxo(void);
uint32_t S2LPManagementGetXtalFrequency(void);
uint32_t S2LPGetFrequencyBand(void);
void S2LPRadioSetXtalFrequency(uint32_t xtal);
uint8_t EepromIdentification(void);
uint32_t S2LPManagementComputeRcoFrequency(void);
void S2LPManagementRcoCalibration(void);
uint32_t S2LPManagementComputeRcoFrequency(void);
uint32_t S2LPGetSPIPrescaler(void);
/**
* @}
*/



/**
* @}
*/


/**
* @}
*/

/**
* @}
*/

#endif /*__S2LP_AUX_UTILS_H */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
