/**
 * @file    S2LP_Csma.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP CSMA.
 * @details
 *
 * The S2LP CSMA feature, when configured and enabled, is transparent
 * for the user. It means the user has only to call the <i>@ref S2LPCsmaInit()</i>
 * function on a filled structure and then enable the CSMA policy using the <i>@ref S2LPCsma()</i>
 * function.
 *
 * <b>Example:</b>
 * @code
 *
 * CsmaInit csmaInit={
 *   S_DISABLE,         // persistent mode
 *   TBIT_TIME_64,      // Tbit time
 *   TCCA_TIME_3,       // Tcca time
 *   5,                 // max number of backoffs
 *   0xFA21,            // BU counter seed
 *   32                 // CU prescaler
 * };
 *
 * ...
 *
 * S2LPCsmaInit(&csmaInit);
 * S2LPCsma(S_ENABLE);
 *
 *
 * @endcode
 *
 * @note The CS status depends of the RSSI threshold set. Please see the S2LP_Qi
 * module for details.
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
#ifndef __S2LP_CSMA_H
#define __S2LP_CSMA_H


/* Includes ------------------------------------------------------------------*/
#include "S2LP_Types.h"
#include "S2LP_Regs.h"
#include "S2LP_CORE_SPI.h"

#ifdef __cplusplus
 extern "C" {
#endif


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @defgroup S2LP_Csma        CSMA
 * @brief Configuration and management of S2LP CSMA.
 * @details See the file <i>@ref S2LP_Csma.h</i> for more details.
 * @{
 */

/**
 * @defgroup Csma_Exported_Types        CSMA Exported Types
 * @{
 */


/**
 * @brief  Multiplier for Tcca time enumeration (Tcca = Multiplier*Tbit).
 */

typedef enum {
  CSMA_PERIOD_64TBIT,      /*!< CSMA/CA: Sets CCA period to 64*TBIT */
  CSMA_PERIOD_128TBIT,    /*!< CSMA/CA: Sets CCA period to 128*TBIT */
  CSMA_PERIOD_256TBIT,    /*!< CSMA/CA: Sets CCA period to 256*TBIT */
  CSMA_PERIOD_512TBIT    /*!< CSMA/CA: Sets CCA period to 512*TBIT */
}SCsmaPeriod;


/**
  * @brief  S2LP CSMA Init structure definition
  */
typedef struct {
  SFunctionalState  xCsmaPersistentMode;    /*!< Enable the CSMA persistent mode */
  SCsmaPeriod       xMultiplierTbit;        /*!< Set the Tbit multiplier to obtain the Tcca. @ref CcaPeriod */
  uint8_t           xCcaLength;             /*!< Set the Tcca multiplier to determinate the Tlisten. From 0 to 15. */
  uint8_t           cMaxNb;                 /*!< Specifies the max number of backoff cycles. From 0 to 7. */
  uint16_t          nBuCounterSeed;         /*!< Specifies the BU counter seed. */
  uint8_t           cBuPrescaler;           /*!< Specifies the BU prescaler. From 0 to 63. */
} SCsmaInit;


/**
 *@}
 */


/**
 * @defgroup Csma_Exported_Constants    CSMA Exported Constants
 * @{
 */

/**
 * @defgroup Csma_Parameters            CSMA Parameters
 * @{
 */


/**
 *@}
 */

/**
 *@}
 */


/**
 * @defgroup Csma_Exported_Macros       CSMA Exported Macros
 * @{
 */


/**
 *@}
 */


/**
 * @defgroup Csma_Exported_Functions    CSMA Exported Functions
 * @{
 */

void S2LPCsmaInit(SCsmaInit* pxSCsmaInit);
void S2LPCsmaGetInfo(SCsmaInit* pxSCsmaInit);
void S2LPCsma(SFunctionalState xNewState);
SFunctionalState S2LPCsmaGetCsma(void);
void S2LPCsmaPersistentMode(SFunctionalState xNewState);
SFunctionalState S2LPCsmaGetPersistentMode(void);
void S2LPCsmaSeedReloadMode(SFunctionalState xNewState);
SFunctionalState S2LPCsmaGetSeedReloadMode(void);
void S2LPCsmaSetBuCounterSeed(uint16_t nBuCounterSeed);
uint16_t S2LPCsmaGetBuCounterSeed(void);
void S2LPCsmaSetBuPrescaler(uint8_t cBuPrescaler);
uint8_t S2LPCsmaGetBuPrescaler(void);
void S2LPCsmaSetCcaPeriod(SCsmaPeriod xMultiplierTbit);
uint8_t S2LPCsmaGetCcaPeriod(void);
void S2LPCsmaSetCcaLength(uint8_t xCcaLength);
uint8_t S2LPCsmaGetCcaLength(void);
void S2LPCsmaSetMaxNumberBackoff(uint8_t cMaxNb);
uint8_t S2LPCsmaGetMaxNumberBackoff(void);

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
