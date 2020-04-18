/**
 * @file    S2LP_Qi.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2LP QI.
 * @details
 *
 * This module can be used to configure and read some quality indicators
 * used by S2-LP.
 * API to set thresholds and to read values in raw mode or in dBm are
 * provided.
 *
 * <b>Example:</b>
 * @code
 *
 *   int32_t rssiValuedBm;
 *   uint8_t pqiValue, sqiValue;
 *
 *   S2LPQiPqiCheck(S_ENABLE);
 *   S2LPQiSqiCheck(S_ENABLE);
 *
 *   ...
 *
 *   rssiValueDbm = S2LPQiGetRssidBm();
 *   pqiValue = S2LPQiGetPqi();
 *   sqiValue = S2LPQiGetSqi();
 *
 *   ...
 *
 * @endcode
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
#ifndef __S2LP_QI_H
#define __S2LP_QI_H


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
 * @defgroup S2LP_Qi          QI
 * @brief Configuration and management of S2LP QI.
 * @details See the file <i>@ref S2LP_Qi.h</i> for more details.
 * @{
 */

/**
 * @defgroup Qi_Exported_Types  QI Exported Types
 * @{
 */

/**
 * @brief  S2LP RSSI mode enumeration
 */
typedef enum {
  RSSI_STATIC_MODE = 0,         /* static mode */
  RSSI_DYNAMIC_6DB_STEP_MODE,   /* dynamic mode 6 dB above threshold*/
  RSSI_DYNAMIC_12DB_STEP_MODE,  /* dynamic mode 12 dB above threshold */
  RSSI_DYNAMIC_18DB_STEP_MODE   /* dynamic mode 18 dB above threshold */
} SRssiMode;


/**
 * @brief  S2LP RSSI Init structure definition
 */
typedef struct {
  uint8_t      cRssiFlt;       /*!< Set the RSSI filter gain. From 0 to 15. */
  SRssiMode    xRssiMode;      /*!< Set the RSSI mode. @ref SRssiMode */
  int32_t        cRssiThreshdBm; /*!< Set the RSSI threshold in dBm. From -130 to -2.5 dBm. */
} SRssiInit;


/**
  *@}
  */


/**
 * @defgroup Qi_Exported_Constants      QI Exported Constants
 * @{
 */


/**
  *@}
  */


/**
 * @defgroup Qi_Exported_Macros         QI Exported Macros
 * @{
 */


/**
 *@}
 */


/**
 * @defgroup Qi_Exported_Functions       QI Exported Functions
 * @{
 */

int32_t S2LPRadioGetRssidBm(void);
int32_t S2LPRadioGetRssidBmRun(void);
void S2LPRadioSetRssiThreshdBm(int32_t wRssiThrehsold);
void S2LPRadioCsBlanking(SFunctionalState xCsBlank);
void S2LPRadioRssiInit(SRssiInit* xSRssiInit);
void S2LPRadioGetRssiInfo(SRssiInit* xSRssiInit);
void S2LPRadioAntennaSwitching(SFunctionalState xAntennaSwitch);
void S2LPRadioSetPqiCheck(uint8_t cPqiLevel);
SFlagStatus S2LPQiGetCs(void);

/**
 *@}
 */

/**
 *@}
 */


/**
 *@}
 */


#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
