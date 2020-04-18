/**
 * @file    S2LP_PktBasic.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP Basic packets.
 * @details
 *
 * This module can be used to manage the configuration of S2LP Basic
 * packets.
 * The user can obtain a packet configuration filling the structure
 * <i>@ref PktBasicInit</i>, defining in it some general parameters
 * for the S2-LP Basic packet format.
 * Another structure the user can fill is <i>@ref PktBasicAddressesInit</i>
 * to define the addresses which will be used during the communication.
 * Moreover, functions to set the payload length and the destination address
 * are provided.
 *
 * <b>Example:</b>
 * @code
 *
 * PktBasicInit basicInit={
 *   32,       // preamble length in bits
 *   32,            // sync word length in bits
 *   0x88888888,                        // sync word
 *   S_ENABLE,                          // variable or fixed payload length
 *   S_DISABLE,                         // extended length field width (used only for variable length)
 *   PKT_NO_CRC,                        // CRC mode
 *   S_ENABLE,                          // address field
 *   S_DISABLE,                         // FEC
 *   S_ENABLE                           // whitening
 * };
 *
 * PktBasicAddressesInit addressInit={
 *   S_ENABLE,                          // enable/disable filtering on my address
 *   0x34,                              // my address (address of the current node)
 *   S_DISABLE,                         // enable/disable filtering on multicast address
 *   0xEE,                              // multicast address
 *   S_DISABLE,                         // enable/disable filtering on broadcast address
 *   0xFF                               // broadcast address
 * };
 *
 * ...
 *
 * S2LPPktBasicInit(&basicInit);
 * S2LPPktBasicAddressesInit(&addressInit);
 *
 * ...
 *
 * S2LPPktBasicSetPayloadLength(20);
 * S2LPPktBasicSetDestinationAddress(0x44);
 *
 * ...
 *
 * @endcode
 *
 * The module provides some other functions that can be used to modify
 * or read only some configuration parameters.
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
#ifndef __S2LP_PKT_BASIC_H
#define __S2LP_PKT_BASIC_H

/* Includes ------------------------------------------------------------------*/

#include "S2LP_Regs.h"
#include "S2LP_Types.h"
#include "S2LP_PacketHandler.h"
#include "S2LP_CORE_SPI.h"

#ifdef __cplusplus
 extern "C" {
#endif


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @defgroup S2LP_PktBasic    Pkt Basic
 * @brief Configuration and management of S2LP Basic packets.
 * @details See the file <i>@ref S2LP_PktBasic.h</i> for more details.
 * @{
 */

/**
 * @defgroup PktBasic_Exported_Types    Pkt Basic Exported Types
 * @{
 */


/**
 * @brief  CRC length in bytes enumeration.
 */
typedef PktCrcMode       BasicCrcMode;



/**
 * @brief  S2LP Basic Packet Init structure definition.
 */
typedef struct {
  uint16_t           xPreambleLength;         /*!< Set the preamble length of packet. From 1 to 1024 chip sequence. */
  uint8_t            xSyncLength;             /*!< Set the sync word length of packet in bits. From 1 to 64 bits. */
  uint32_t           lSyncWords;              /*!< Set the sync words in MSB. */
  SFunctionalState   xFixVarLength;           /*!< Enable the variable length mode. */
  SFunctionalState   cExtendedPktLenField;    /*!< Extend the length field from 1 byte to 2 bytes. Variable length mode only. */
  BasicCrcMode       xCrcMode;                /*!< Set the CRC type. @ref StackCrcMode */
  SFunctionalState   xAddressField;           /*!< Enable the destination address field. */
  SFunctionalState   xFec;                    /*!< Enable the FEC/Viterbi. */
  SFunctionalState   xDataWhitening;          /*!< Enable the data whitening. */
} PktBasicInit;


/**
 * @brief  S2LP Basic packet address structure definition. This structure allows users to specify
 *         the node/multicast/broadcast addresses and the correspondent filtering options.
 */
typedef struct {
  SFunctionalState   xFilterOnMyAddress;         /*!< If set packet is received if its destination address matches with cMyAddress. */
  uint8_t            cMyAddress;                 /*!< Set the MyAddress. */
  SFunctionalState   xFilterOnMulticastAddress;  /*!< If set packet is received if its destination address matches with cMulticastAddress. */
  uint8_t            cMulticastAddress;          /*!< Set the Multicast address */
  SFunctionalState   xFilterOnBroadcastAddress;  /*!< If set packet is received if its destination address matches with cBroadcastAddress. */
  uint8_t            cBroadcastAddress;          /*!< Set the Broadcast address */
} PktBasicAddressesInit;


/**
 *@}
 */


/**
 * @defgroup PktBasic_Exported_Macros   Pkt Basic Exported Macros
 * @{
 */

/**
 * @brief  Sets the PREAMBLE field length for S2LP Basic packets.
 * @param  xPreambleLength length of PREAMBLE field in bytes.
 *         This parameter can be any value of @ref BasicPreambleLength.
 * @retval None.
 */
#define S2LPPktBasicSetPreambleLength(xPreambleLength)                S2LPSetPreambleLength(xPreambleLength)


/**
 * @brief  Returns the PREAMBLE field length mode for S2LP Basic packets.
 * @param  None.
 * @retval uint8_t Preamble field length in bytes.
 */
#define S2LPPktBasicGetPreambleLength()                               S2LPGetPreambleLength()


/**
 * @brief  Sets the SYNC field length for S2LP Basic packets.
 * @param  xSyncLength length of SYNC field in bytes.
 *         This parameter can be any value of @ref BasicSyncLength.
 * @retval None.
 */
#define S2LPPktBasicSetSyncLength(xSyncLength)                        S2LPSetSyncLength((PktSyncLength)xSyncLength)


/**
 * @brief  Returns the SYNC field length for S2LP Basic packets.
 * @param  None.
 * @retval uint8_t SYNC field length in bytes.
 */
#define S2LPPktBasicGetSyncLength()                                   S2LPGetSyncLength()


/**
 * @brief  Enables or Disables the CRC filtering.
 * @param  xNewState new state for CRC_CHECK.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define S2LPPktBasicFilterOnCrc(xNewState)                            S2LPPktCommonFilterOnCrc(xNewState)


/**
 * @brief  Enables or Disables WHITENING for S2LP packets.
 * @param  xNewState new state for WHITENING mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define S2LPPktBasicWhitening(xNewState)                               S2LPWhitening(xNewState)


/**
 * @brief  Enables or Disables FEC for S2LP Basic packets.
 * @param  xNewState new state for FEC mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define S2LPPktBasicFec(xNewState)                                     S2LPFec(xNewState)



/**
 * @brief  Sets multiple SYNC words for S2LP Basic packets.
 * @param  lSyncWords SYNC words to be set with format: 0x|SYNC1|SYNC2|SYNC3|SYNC4|.
 *         This parameter is a uint32_t.
 * @param  xSyncLength SYNC length in bytes. The 32bit word passed will be stored in the SYNCx registers from the MSb
 *         until the number of bytes in xSyncLength has been stored.
 *         This parameter is a @ref BasicSyncLength.
 * @retval None.
 */
#define S2LPPktBasicSetSyncWords(lSyncWords, xSyncLength)              S2LPSetSyncWords(lSyncWords, xSyncLength)


/**
 *@}
 */


/**
 * @defgroup PktBasic_Exported_Functions        Pkt Basic Exported Functions
 * @{
 */

void S2LPPktBasicInit(PktBasicInit* pxPktBasicInit);
void S2LPPktBasicGetInfo(PktBasicInit* pxPktBasicInit);
void S2LPPktBasicAddressesInit(PktBasicAddressesInit* pxPktBasicAddresses);
void S2LPPktBasicGetAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses);
void S2LPPktBasicSetFormat(void);
void S2LPPktBasicAddressField(SFunctionalState xAddressField);
SFunctionalState S2LPPktBasicGetAddressField(void);
void S2LPPktBasicSetPayloadLength(uint16_t nPayloadLength);
uint16_t S2LPPktBasicGetPayloadLength(void);
uint16_t S2LPPktBasicGetReceivedPktLength(void);

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
