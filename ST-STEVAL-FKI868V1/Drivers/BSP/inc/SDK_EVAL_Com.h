/**
* @file    SDK_EVAL_Com.h
* @author  AMG - RF Application team
* @version 3.2.1
* @date    26-September-2016
* @brief   This file contains definitions for Software Development Kit eval board COM ports.
* @details
*
* Configuration of the COM peripheral for the SDK motherboard.
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
#ifndef __SDK_EVAL_COM_H
#define __SDK_EVAL_COM_H

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup SDK_EVAL_NUCLEO
* @{
*/


/** @addtogroup SDK_EVAL_Com            SDK EVAL Com
* @brief Management of Software Development Kit eval board COM ports.
* @details See the file <i>@ref SDK_EVAL_Com.h</i> for more details.
* @{
*/


/** @defgroup SDK_EVAL_Com_Exported_Types               SDK EVAL Com Exported Types
* @{
*/







/** @defgroup SDK_EVAL_Com_Exported_Functions           SDK EVAL Com Exported Functions
* @{
*/


void SdkEvalComInit(void);
void SdkEvalComBaudrate(uint32_t baudrate);
void SdkEvalComTriggerTx(void);
unsigned char __io_getcharNonBlocking(unsigned char *data);
void __io_putchar( char c );
int __io_getchar(void);
void __io_flush( void );

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
