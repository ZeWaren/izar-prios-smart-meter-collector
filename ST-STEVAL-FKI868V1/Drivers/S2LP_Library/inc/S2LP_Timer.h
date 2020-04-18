/**
 * @file    S2LP_Timer.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP Timers.
 * @details
 *
 * This module provides API to configure the S2-LP timing mechanisms.
 * They allow the user to set the timer registers using raw values or
 * compute them since the desired timer value is expressed in ms.
 * Moreover the management of the S2-LP LDCR mode can be done using
 * these API.
 *
 * <b>Example:</b>
 * @code
 *   ...
 *
 *   S2LPTimerSetRxTimeoutMs(50.0);
 *   S2LPTimerSetWakeUpTimerUs(150000);
 *
 *   // IRQ configuration for RX_TIMEOUT and WAKEUP_TIMEOUT
 *   ...
 *
 *   S2LPTimerLdcrMode(S_ENABLE);
 *
 *   ...
 *
 * @endcode
 *
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
#ifndef __S2LP1_TIMER_H
#define __S2LP1_TIMER_H


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
 * @defgroup S2LP_Timer               Timer
 * @brief Configuration and management of S2LP Timers.
 * @details See the file <i>@ref S2LP_Timer.h</i> for more details.
 * @{
 */


/**
 * @defgroup Timer_Exported_Types       Timer Exported Types
 * @{
 */

/**
 * @brief  All the possible RX timeout stop conditions enumeration.
 */
typedef enum {
     NO_TIMEOUT_STOP = 0x00,                /*!< Timeout never stopped */
     PQI_ABOVE_THRESHOLD = 0x01,            /*!< Timeout stopped on PQI above threshold */
     SQI_ABOVE_THRESHOLD = 0x02,            /*!< Timeout stopped on SQI above threshold */
     SQI_AND_PQI_ABOVE_THRESHOLD = 0x03,    /*!< Timeout stopped on both SQI and PQI above threshold */
     RSSI_ABOVE_THRESHOLD = 0x04,           /*!< Timeout stopped on RSSI above threshold */
     RSSI_AND_PQI_ABOVE_THRESHOLD = 0x05,   /*!< Timeout stopped on both RSSI and PQI above threshold */
     RSSI_AND_SQI_ABOVE_THRESHOLD = 0x06,   /*!< Timeout stopped on both RSSI and SQI above threshold */
     ALL_ABOVE_THRESHOLD = 0x07,            /*!< Timeout stopped only if RSSI, SQI and PQI are above threshold */
     TIMEOUT_ALWAYS_STOPPED = 0x08,         /*!< Timeout always stopped (default) */
     SQI_OR_PQI_ABOVE_THRESHOLD = 0x0B,     /*!< Timeout stopped if one between SQI or PQI are above threshold */
     RSSI_OR_PQI_ABOVE_THRESHOLD = 0x0D,    /*!< Timeout stopped if one between RSSI or PQI are above threshold */
     RSSI_OR_SQI_ABOVE_THRESHOLD = 0x0E,    /*!< Timeout stopped if one between RSSI or SQI are above threshold */
     ANY_ABOVE_THRESHOLD = 0x0F             /*!< Timeout stopped if one among RSSI, SQI or SQI are above threshold */
} RxTimeoutStopCondition;


/**
 * @}
 */


/**
 * @defgroup Timer_Exported_Constants   Timer Exported Constants
 * @{
 */


/**
 * @}
 */


/**
 * @defgroup Timer_Exported_Macros              Timer Exported Macros
 * @{
 */

#define SET_INFINITE_RX_TIMEOUT()     S2LPTimerSetRxTimerCounter(0)

/**
 * @}
 */


/**
 * @defgroup Timer_Exported_Functions           Timer Exported Functions
 * @{
 */

void S2LPTimerSetRxTimerStopCondition(RxTimeoutStopCondition xStopCondition);

void S2LPTimerLdcrMode(SFunctionalState xNewState);
void S2LPTimerLdcrAutoReload(SFunctionalState xNewState);
SFunctionalState S2LPTimerLdcrGetAutoReload(void);
void S2LpTimerFastRxTermTimer(SFunctionalState xNewState);
void S2LpSetTimerFastRxTermTimer(uint8_t fast_rx_word);
void S2LpSetTimerFastRxTermTimerUs(uint32_t fast_rx_us);

void S2LPTimerLdcrMode(SFunctionalState xNewState);
void S2LPTimerLdcrAutoReload(SFunctionalState xNewState);
SFunctionalState S2LPTimerLdcrGetAutoReload(void);

void S2LPTimerSetRxTimer(uint8_t cCounter , uint8_t cPrescaler);
void S2LPTimerSetRxTimerUs(uint32_t lDesiredUsec);
void S2LPTimerSetRxTimerCounter(uint8_t cCounter);
void S2LPTimerSetRxTimerPrescaler(uint8_t cPrescaler);
void S2LPTimerGetRxTimerUs(uint32_t* plTimeoutUsec, uint8_t* pcCounter , uint8_t* pcPrescaler);

void S2LPTimerSetWakeUpTimer(uint8_t cCounter , uint8_t cPrescaler);
void S2LPTimerSetWakeUpTimerUs(uint32_t lDesiredUsec);
void S2LPTimerSetWakeUpTimerCounter(uint8_t cCounter);
void S2LPTimerSetWakeUpTimerPrescaler(uint8_t cPrescaler);
void S2LPTimerSetWakeUpTimerReloadUs(uint32_t lDesiredUsec);
void S2LPTimerGetWakeUpTimerUs(uint32_t* plWakeUpUsec, uint8_t* pcCounter, uint8_t* pcPrescaler, uint8_t* pcMulti);
void S2LPTimerSetWakeUpTimerReload(uint8_t cCounter , uint8_t cPrescaler, uint8_t cMulti);
void S2LPTimerSetWakeUpTimerReloadCounter(uint8_t cCounter);
void S2LPTimerSetWakeUpTimerReloadPrescaler(uint8_t cPrescaler);
void S2LPTimerGetWakeUpTimerReloadUs(uint32_t* plWakeUpReloadUsec, uint8_t* pcCounter, uint8_t* pcPrescaler, uint8_t* pcMulti);

uint16_t S2LPTimerGetRcoFrequency(void);
void S2LPTimerCalibrationRco(SFunctionalState xCalibration);
void S2LPTimerSleepB(SFunctionalState en);
void S2LPTimerLdcIrqWa(SFunctionalState en);

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
