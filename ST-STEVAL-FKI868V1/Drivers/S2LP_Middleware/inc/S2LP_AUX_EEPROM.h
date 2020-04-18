/**
* @file    SPIRIT_SDK_EEPROM.h
* @author  LowPower RF BU - AMG
* @version 3.2.1
* @date    March, 2020
* @brief   SDK EVAL eeprom management
* @details This module exports API to manage the EEPROM of the eval boards.
*          Data stored in the EEPROM are mainly some manifacturing infos,
*          and informations that can be useful when developing applications
*          with the daughter board. Some of them are the RF band, the offset
*          of the carrier from the nominal frequency and the XTAL frequency.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SPIRIT_SDK_EEPROM_H_
#define SPIRIT_SDK_EEPROM_H_

#include "cube_hal.h"

#ifdef __cplusplus
  "C" {
#endif

/**
 * @addtogroup S2LP_AUX_EEPROM
 * @{
 */

/**
 * @defgroup S2LP_AUX_EEPROM_MACROS				S2LP AUX EEPROM exported MACROs
 * @{
 */


/* EEPROM SPI commands */
#define EEPROM_CMD_WREN    0x06	// Write Enable
#define EEPROM_CMD_WRDI    0x04	// Write Disable
#define EEPROM_CMD_RDSR    0x05	// Read Status Register
#define EEPROM_CMD_WRSR    0x01	// Write Status Register
#define EEPROM_CMD_READ    0x03	// Read from Memory Array
#define EEPROM_CMD_WRITE   0x02	// Write to Memory Array

/* EEPROM SPI status */
#define EEPROM_STATUS_SRWD    0x80	// Status Register Write Disable
#define EEPROM_STATUS_BP      0x0C	// Block Protect
#define EEPROM_STATUS_WEL     0x02	// Write Enable
#define EEPROM_STATUS_WIP     0x01	// Write in Progress
/**
* @}
*/

/** @defgroup S2LP_AUX_EEPROM_Functions			S2LP AUX EEPROM exported functions
* @{
*/


/**
* @brief  Initializes the SPI for the EEPROM.
*         This function can be replaced by EepromCsPinInitialization if
*         S2LPSpiInit is called.
* @param  None
* @retval None
*/
void EepromSpiInitialization(void);

/**
* @brief  Initialization of the CSn pin of the EEPROM.
*         This function is called internally by EepromCsPinInitialization.
* @param  None
* @retval None
*/
void EepromCsPinInitialization(void);

/**
* @brief  Initialization of the CSn pin of the EEPROM for XNUCLEO boards.
*
* @param  None
* @retval None
*/
void EepromCsXnucleoPinInitialization(void);

/**
* @brief  Read a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
* @param  None
* @retval None
*/
uint8_t EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);

/**
* @brief  Write a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
*         It is allowed to write only a page for each operation. If the bytes
*         exceed the single page location, the other bytes are written at the
*         beginning.
* @param  None
* @retval None
*/
uint8_t EepromWrite(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);

/**
* @brief  Wait polling the SPI until the internal WIP flag is RESET.
*         The flag is SET when a write operation is running.
* @param  None
* @retval None
*/
void EepromWaitEndWriteOperation(void);

/**
* @brief EepromWriteEnable
* @param  None
* @retval None
*/
void EepromWriteEnable(void);

/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
uint8_t EepromStatus(void);

/**
* @brief  Set the ERSR status bit.
* @param  None
* @retval Status
*/
uint8_t EepromSetSrwd(void);

/**
* @brief  This function is to query if EEPROM is present or not.
* @param  None
* @retval 1 (yes) or 0 (no).
*/
uint8_t S2LPEvalGetHasEeprom(void);

/**
* @brief  This function is to set if EEPROM is present or not.
* @param  1 (yes) or 0 (no).
* @retval None
*/
void S2LPEvalSetHasEeprom(uint8_t eeprom);

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
 /******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

