/**
* @file    SDK_UTILS_Timers.c
* @author  LowPower RF BU - AMG
* @version 4.1.0
* @date    September, 2019
* @brief   SDK EVAL timers configuration.
* @details
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
#ifndef __SDK_EVAL_TIMERS_H
#define __SDK_EVAL_TIMERS_H

/* Includes ------------------------------------------------------------------*/

#include "cube_hal.h"
#include "SDK_EVAL_Config.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @addtogroup SDK_EVAL_NUCLEO
 * @{
 */

/**
 * @defgroup SDK_EVAL_Timers            SDK EVAL Timers
 * @brief Management of STM32L timers.
 * @details See the file <i>@ref SDK_EVAL_Timers.h</i> for more details.
 * @{
 */




/**
 * @defgroup SDK_EVAL_Timers_Exported_Constants         SDK EVAL Timers Exported Constants
 * @{
 */


/**
* @brief  Absolute value macro.
* @param  x: Value on which apply the abs function.
* @retval None
*/
#define ABS(x)  (x>0?x:-x)

/**
 *@}
 */


/**
 * @defgroup SDK_EVAL_Timers_Exported_Macros            SDK EVAL Timers Exported Macros
 * @{
 */
typedef enum TimerID_Enum {
  TIMESTAMP_TIMER_ID = 0,
  APPL_TIMER_ID,
  GP_TIMER_ID
} TimerID_Type;

typedef void (*TimerCallbackType)(uint32_t time);

typedef struct TIMER_InfoS {
  TimerCallbackType callback;
  uint32_t timeout;
  uint16_t delay;
  uint8_t enabled;
} TIMER_InfoType;


/**
 *@}
 */


/**
 * @defgroup SDK_EVAL_Timers_Exported_Functions         SDK EVAL Timers Exported Functions
 * @{
 */

#ifdef USE_SYSTICK_DELAY
void SysTick_Handler(void);
void SdkDelayMs(volatile uint32_t lTimeMs);
uint32_t SdkGetCurrentSysTick(void);
void SdkStartSysTick(void);
#endif


uint32_t SdkEvalGetTimerValue(void);
void SdkEvalTimersResetCounter(TimerID_Type timerID);
uint32_t SdkEvalTimersState(TimerID_Type timerID, TIM_HandleTypeDef *tim, uint8_t enable);
void SdkEvalTimersTimConfig_ms(TimerID_Type timerID, uint16_t ms);
void SdkEvalTimerTimestampReset(void);
void SdkEvalTimersFindFactors(uint32_t n, uint16_t *pnPrescaler, uint16_t *pnCounter);
void SdkEvalTimersTimConfig(TIM_HandleTypeDef*  TIM_TimeBaseStructure, uint16_t nPrescaler, uint16_t nPeriod);

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

#endif /*__SDK_EVAL_TIMERS_H*/

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
