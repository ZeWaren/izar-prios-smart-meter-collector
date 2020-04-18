/**
 * @file    S2LP_General.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP General functionalities.
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
#ifndef __S2LP_GENERAL_H
#define __S2LP_GENERAL_H


/* Includes ------------------------------------------------------------------*/

#include "S2LP_Regs.h"
#include "S2LP_Types.h"
#include "S2LP_CORE_SPI.h"



#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @defgroup S2LP_General     General
 * @brief Configuration and management of S2LP General functionalities.
 * @details See the file <i>@ref S2LP_General.h</i> for more details.
 * @{
 */

/**
 * @defgroup General_Exported_Types     General Exported Types
 * @{
 */


/**
 * @brief  S2LP version type enumeration
 */

typedef enum {
  MODE_EXT_XO  = 0,
  MODE_EXT_XIN = 0x80,
} ModeExtRef;


/**
 * @}
 */


/**
 * @defgroup General_Exported_Constants         General Exported Constants
 * @{
 */


/**
 * @}
 */


/**
 * @defgroup General_Exported_Macros            General Exported Macros
 * @{
 */
#define S2LPGeneralLibraryVersion() "S2LP_Libraries_v.1.3.0"


/**
 * @}
 */


/**
 * @defgroup General_Exported_Functions         General Exported Functions
 * @{
 */

uint8_t S2LPGeneralGetDevicePN(void);
uint8_t S2LPGeneralGetVersion(void);
void S2LPGeneralSetExtRef(ModeExtRef xExtMode);
ModeExtRef S2LPGeneralGetExtRef(void);
void S2LPRadioSetExternalSmpsMode(SFunctionalState xNewState);
void S2LPRefreshStatus(void);

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

#endif

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
