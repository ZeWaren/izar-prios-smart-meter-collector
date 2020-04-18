/**
 * @file    S2LP_Fifo.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP Fifo.
 * @details
 *
 * This module allows the user to manage the linear FIFO. The functions exported
 * here can be used to set the thresholds for the FIFO almost full / empty alarm
 * interrupts or to get the total number of elements inside the FIFO.
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
#ifndef __S2LP_FIFO_H
#define __S2LP_FIFO_H


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
 * @defgroup S2LP_Fifo           FIFO
 * @brief Configuration and management of S2LP FIFO.
 * @details See the file <i>@ref S2LP_Fifo.h</i> for more details.
 * @{
 */

/**
 * @defgroup Fifo_Exported_Types   FIFO Exported Types
 * @{
 */


/**
 * @}
 */


/**
 * @defgroup Fifo_Exported_Constants       FIFO Exported Constants
 * @{
 */

/**
 * @}
 */


/**
 * @defgroup Fifo_Exported_Macros          FIFO Exported Macros
 * @{
 */


/**
 * @}
 */


/**
 * @defgroup Fifo_Exported_Functions                       FIFO Exported Functions
 * @{
 */

uint8_t S2LPFifoReadNumberBytesRxFifo(void);
uint8_t S2LPFifoReadNumberBytesTxFifo(void);
void S2LPFifoSetAlmostFullThresholdRx(uint8_t cThrRxFifo);
uint8_t S2LPFifoGetAlmostFullThresholdRx(void);
void S2LPFifoSetAlmostEmptyThresholdRx(uint8_t cThrRxFifo);
uint8_t S2LPFifoGetAlmostEmptyThresholdRx(void);
void S2LPFifoSetAlmostFullThresholdTx(uint8_t cThrTxFifo);
uint8_t S2LPFifoGetAlmostFullThresholdTx(void);
void S2LPFifoSetAlmostEmptyThresholdTx(uint8_t cThrTxFifo);
uint8_t S2LPFifoGetAlmostEmptyThresholdTx(void);
void S2LPFifoMuxRxFifoIrqEnable(SFunctionalState xNewState);

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
