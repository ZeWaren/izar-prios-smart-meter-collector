/**
 * @file    S2LP_Config.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   S2LP Configuration and useful defines .
 * @details
 *
 * This file is used to include all or a part of the S2LP
 * libraries into the application program which will be used.
 * Moreover some important parameters are defined here and the
 * user is allowed to edit them.
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
#ifndef __S2LP_CONFIG_H
#define __S2LP_CONFIG_H


  /* Includes ------------------------------------------------------------------*/
#include "S2LP_Regs.h"
#include "S2LP_Commands.h"
#include "S2LP_Csma.h"
#include "S2LP_General.h"
#include "S2LP_Gpio.h"
#include "S2LP_Timer.h"
#include "S2LP_Fifo.h"
#include "S2LP_PacketHandler.h"
#include "S2LP_PktBasic.h"
#include "S2LP_PktWMbus.h"
#include "S2LP_PktStack.h"
#include "S2LP_Radio.h"
#include "S2LP_Qi.h"
#include "S2LP_Types.h"
#include "S2LP_CORE_SPI.h"


#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup S2LP_Libraries        S2LP Libraries
 * @brief This firmware implements libraries which allow the user
 * to manage the features of S2LP without knowing the hardware details.
 * @details The <i>S2LP_Libraries</i> modules are totally platform independent. The library provides one
 * module for each device feature. Each module refers to some functions whose prototypes are located in the
 * header file <i>@ref MCU_Interface.h</i>. The user who want to use these libraries on a particular
 * platform has to implement these functions respecting them signatures.
 * @{
 */

/** @defgroup S2LP_Configuration      Configuration
 * @brief S2LP Configuration and useful defines.
 * @details See the file <i>@ref S2LP_Config.h</i> for more details.
 * @{
 */


/** @defgroup Configuration_Exported_Types      Configuration Exported Types
 * @{
 */

/**
 * @}
 */


/** @defgroup Configuration_Exported_Constants  Configuration Exported Constants
 * @{
 */
#define DIG_DOMAIN_XTAL_THRESH  30000000        /*!< Digital domain logic threshold for XTAL in MHz */

/**
 * @}
 */


/** @defgroup Configuration_Exported_Macros     Configuration Exported Macros
 * @{
 */

  
/**
 * @}
 */


/** @defgroup Configuration_Exported_Functions  Configuration Exported Functions
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

#endif

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
