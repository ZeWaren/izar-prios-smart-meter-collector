/**
 * @file    SDK_EVAL_Config.h
 * @author  AMS VMA RF application team
 * @version V2.0.0
 * @date    November 29, 2018
 * @brief   This file contains SDK EVAL configuration and useful defines.
 * @details
 *
 * This file is used to include all or a part of the SDK Eval
 * libraries into the application program which will be used.
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
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_CONFIG_H
#define __SDK_EVAL_CONFIG_H

/* User defined custom platform */
/*************************** USER CUSTOM PALTFORM ****************************/
#ifdef USER_EVAL_PLATFORM
/* Adding the symbol USER_EVAL_PLATFORM an user platform can be supported
 * at compile time, by following these steps:
 *
 * 1) Create a file  "USER_Platform_Configuration.h" with specific user
 *    platform configuration.
 *
 * 2) Place the "USER_Platform_Configuration.h" on the
 *    \Drivers\BSP\SDK_Eval_STM32\inc folder.
 */
	#include "USER_Platform_Configuration.h"

/*************************** SUPPORTED PALTFORM ****************************/
#else
	/* NUCLEO64_L0xx */
	#ifdef USE_STM32L0XX_NUCLEO
	#include "Platform_Configuration_NucleoL0xx.h"
	#endif
	/* NUCLEO64_L1xx */
	#ifdef USE_STM32L1XX_NUCLEO
	#include "Platform_Configuration_NucleoL1xx.h"
	#endif
	/* NUCLEO64_F0xx */
	#ifdef USE_STM32F0XX_NUCLEO
	#include "Platform_Configuration_NucleoF0xx.h"
	#endif
	/* NUCLEO64_F4xx */
	#ifdef USE_STM32F4XX_NUCLEO
	#include "Platform_Configuration_NucleoF4xx.h"
	#endif
#endif

#include "SDK_EVAL_Button.h"
#include "SDK_EVAL_Led.h"
#include "SDK_EVAL_Com.h" /* Add if you need COM port access */

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SDK_EVAL_BlueNRG1
 * @{
 */

/** @addtogroup SDK_EVAL_Config         SDK EVAL Config
 * @brief SDK EVAL configuration.
 * @details See the file <i>@ref SDK_EVAL_Config.h</i> for more details.
 * @{
 */

/** @addtogroup SDK_EVAL_Config_Exported_Types          SDK EVAL Config Exported Types
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Config_Exported_Constants        SDK EVAL Config Exported Constants
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Config_Exported_Macros           SDK EVAL Config Exported Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Config_Exported_Functions        SDK EVAL Config Exported Functions
 * @{
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

#ifdef __cplusplus
}
#endif

#endif /*__SDK_EVAL_CONFIG_H */
/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
