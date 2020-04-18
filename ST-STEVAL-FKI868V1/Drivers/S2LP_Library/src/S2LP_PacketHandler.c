/**
 * @file    S2LP_PacketHandler.c
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of the common features of S2-LP packets.
 * @details
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

/* Includes ------------------------------------------------------------------*/
#include "S2LP_PacketHandler.h"
#include "S2LP_PktWMbus.h"


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_PktCommon
 * @{
 */


/**
 * @defgroup PktCommon_Private_TypesDefinitions         Pkt Common Private Types Definitions
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup PktCommon_Private_Defines                  Pkt Common Private Defines
 * @{
 */


/**
 *@}
 */


/**
 * @defgroup PktCommon_Private_Macros                   Pkt Common Private Macros
 * @{
 */


#define IS_DIRECT_TX(MODE)  (((MODE) == NORMAL_TX_MODE) || \
			     ((MODE) == DIRECT_TX_FIFO_MODE) || \
                             ((MODE) == DIRECT_TX_GPIO_MODE)  || \
                             ((MODE) == PN9_TX_MODE))

#define IS_DIRECT_RX(MODE)  (((MODE) == NORMAL_RX_MODE) || \
		             ((MODE) == DIRECT_RX_FIFO_MODE) || \
		             ((MODE) == DIRECT_RX_GPIO_MODE))


#define IS_PKT_SEQ_NUMBER_RELOAD(SEQN)                (SEQN<=3)

/**
 *@}
 */


/**
 * @defgroup PktCommon_Private_Variables                Pkt Common Private Variables
 * @{
 */

/**
 *@}
 */



/**
 * @defgroup PktCommon_Private_FunctionPrototypes       Pkt Common Private Function Prototypes
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup PktCommon_Private_Functions                Pkt Common Private Functions
 * @{
 */


/**
 * @brief  Get the packet format.
 * @param  None.
 * @retval Packet format code.
 */
uint8_t S2LPGetPacketFormat(void)
{
  uint8_t tmp;

  /* Reads the PCKTCTRL2 register value */
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmp);

  tmp &= PCKT_FRMT_REGMASK;
  tmp >>= 6;

  /* wmbus or basic */
  if(tmp==0x00) {
    if(S2LPPktWMbusGetSubmode() != WMBUS_SUBMODE_NOT_CONFIGURED) {
      tmp = 4; // wmbus
    }
  }
  return tmp;
}


/**
 * @brief  Set the PREAMBLE field Length mode for S2LP packets.
 * @param  xPreambleLength length of PREAMBLE field in chips sequence (bit pairs).
 * @retval None.
 */
void S2LPSetPreambleLength(uint16_t cPreambleLength)
{
  uint8_t tmpBuffer[2];

  s_assert_param(IS_PREAMBLE_LEN(cPreambleLength));

  S2LPSpiReadRegisters(PCKTCTRL6_ADDR, 1, &tmpBuffer[0]);

  /* Set the preamble length */
  tmpBuffer[0] &= ~PREAMBLE_LEN_9_8_REGMASK;
  tmpBuffer[0] |= (cPreambleLength>>8) & PREAMBLE_LEN_9_8_REGMASK;
  tmpBuffer[1] = cPreambleLength & PREAMBLE_LEN_7_0_REGMASK;

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL6_ADDR, 2, tmpBuffer);

}


/**
 * @brief  Return the PREAMBLE field Length mode for S2LP packets.
 * @param  None.
 * @retval uint8_t Preamble field length in chips sequence (bit pairs).
 */
uint16_t S2LPGetPreambleLength(void)
{
  uint8_t tmpBuffer[2];

  /* Reads the PCKTCTRL2 register value */
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKTCTRL6_ADDR, 2, tmpBuffer);

  /* Rebuild and return value */
  return ( ((((uint16_t)tmpBuffer[0])&PREAMBLE_LEN_9_8_REGMASK)<<8) | ((uint16_t)tmpBuffer[1]));

}


/**
 * @brief  Set the SYNC field Length for S2LP packets.
 * @param  xSyncLength length of SYNC field in bits.
 * @retval None.
 */
void S2LPSetSyncLength(uint8_t cSyncLength)
{
  uint8_t tmp;

  s_assert_param(IS_SYNC_LEN(cSyncLength));

  S2LPSpiReadRegisters(PCKTCTRL6_ADDR, 1, &tmp);

  tmp &= ~SYNC_LEN_REGMASK;
  tmp |= (cSyncLength<<2);

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL6_ADDR, 1, &tmp);

}


/**
 * @brief  Return the SYNC field Length for S2LP packets.
 * @param  None.
 * @retval uint8_t Sync field length in bits.
 */
uint8_t S2LPGetSyncLength(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKTCTRL6_ADDR, 1, &tmp);
  return (((tmp & SYNC_LEN_REGMASK)>>2) );

}


/**
 * @brief  Enable or Disable WHITENING for S2LP packets.
 * @param  xNewState new state for WHITENING mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LPPacketHandlerWhitening(SFunctionalState xNewState)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= WHIT_EN_REGMASK;
  }
  else {
    tmp &= ~WHIT_EN_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL1_ADDR, 1, &tmp);

}


/**
* @brief  Enable or Disable the FEC encoding.
* @param  xNewState enable (S_ENABLED) or disable (S_DISABLE).
* @retval None.
*/
void S2LPPacketHandlerFec(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= FEC_EN_REGMASK;
  }
  else {
    tmp &= ~FEC_EN_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL1_ADDR, 1, &tmp);

}


/**
* @brief  Enable or Disable the MANCHESTER encoding.
* @param  xNewState enable (S_ENABLED) or disable (S_DISABLE).
* @retval None.
*/
void S2LPPacketHandlerManchester(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LPSpiReadRegisters(PCKTCTRL2_ADDR, 1, &tmp);

  tmp &= ~MANCHESTER_EN_REGMASK;
  tmp |= (((uint8_t)xNewState)<<1);

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL2_ADDR, 1, &tmp);

}


/**
* @brief  Enable or Disable the 3o6 encoding.
* @param  xNewState enable (S_ENABLED) or disable (S_DISABLE).
* @retval None.
*/
void S2LPPacketHandler3OutOf6(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LPSpiReadRegisters(PCKTCTRL2_ADDR, 1, &tmp);

  tmp &= ~MBUS_3OF6_EN_REGMASK;
  tmp |= (((uint8_t)xNewState)<<2);

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL2_ADDR, 1, &tmp);

}


/**
* @brief  Set the SYNC_WORD.
* @param  lSyncWords SYNC_WORD given a 32 bits aligned word.
* @param  SyncLength length of the sync word in bits.
* @retval None.
*/
void S2LPSetSyncWords(uint32_t lSyncWords, uint8_t cSyncLength)
{
  uint8_t tmpBuffer[4], tmp;

  /* Split the 32-bit value in 4 8-bit values */
  for(uint8_t i=0 ; i<4 ; i++) {
    tmpBuffer[i]=(uint8_t)(lSyncWords>>(8*i));
  }

  S2LPSpiReadRegisters(PCKTCTRL6_ADDR, 1, &tmp);

  tmp &= ~SYNC_LEN_REGMASK;
  tmp |= (cSyncLength<<2);

  S2LPSpiWriteRegisters(PCKTCTRL6_ADDR, 1, &tmp);

  /* Writes the new value on the PCKTCTRL2 register */
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(SYNC3_ADDR, 4, tmpBuffer);

}


/**
* @brief  Get the SYNC_WORD.
* @param  lSyncWords pointer to the SYNC_WORD 32bit variable where the SYNC word that is set should be stored.
* @param  cSyncLength pointer to the uint8_t variable where sync length should be stored.
* @retval None.
*/
void S2LPGetSyncWords(uint32_t* lSyncWords, uint8_t* cSyncLength)
{
  uint8_t tmpBuffer[4], tmp;

  /* Reads the PCKTCTRL2 register value */
  S2LPSpiReadRegisters(PCKTCTRL6_ADDR, 1, &tmp);

  tmp &= SYNC_LEN_REGMASK;
  tmp >>= 2;
  *cSyncLength = tmp;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(SYNC3_ADDR, 4, tmpBuffer);
  *lSyncWords = ((uint32_t)tmpBuffer[0]<<24) | ((uint32_t)tmpBuffer[1]<<16) | ((uint32_t)tmpBuffer[2]<<8) | ((uint32_t)tmpBuffer[3]);

}


/**
 * @brief  Enable or Disable the filtering on CRC.
 * @param  xNewState new state for CRC_CHECK.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LPPktCommonFilterOnCrc(SFunctionalState xNewState)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= CRC_FLT_REGMASK;
  }
  else {
    tmp &= ~CRC_FLT_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);

}



/**
* @brief  Get the received destination address.
* @param  None.
* @retval uint8_t the last received destination address.
*/
uint8_t S2LPGetReceivedDestinationAddress(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(RX_ADDRE_FIELD0_ADDR, 1, &tmp);
  return tmp;
}


/**
* @brief  Get the received source address.
* @param  None.
* @retval uint8_t the last received source address.
*/
uint8_t S2LPGetReceivedSourceAddress(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(RX_ADDRE_FIELD1_ADDR, 1, &tmp);
  return tmp;
}


/**
* @brief  Get the MY_ADDRESS set on the chip.
* @param  None.
* @retval uint8_t MY_ADDRESS set.
*/
uint8_t S2LPGetMyAddress(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKT_FLT_GOALS0_ADDR, 1, &tmp);
  return tmp;
}


/**
* @brief  Get the BROADCAST_ADDRESS set on the chip.
* @param  None.
* @retval uint8_t BROADCAST_ADDRESS set.
*/
uint8_t S2LPGetBroadcastAddress(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKT_FLT_GOALS2_ADDR, 1, &tmp);
  return tmp;
}


/**
* @brief  Get the MULTICAST_ADDRESS set on the chip.
* @param  None.
* @retval uint8_t MULTICAST_ADDRESS set.
*/
uint8_t S2LPGetMulticastAddress(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKT_FLT_GOALS1_ADDR, 1, &tmp);
  return tmp;
}


/**
* @brief  Get the SOURCE_MASK set on the chip.
* @param  None.
* @retval uint8_t SOURCE_MASK set.
*/
uint8_t S2LPGetRxSourceMask(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKT_FLT_GOALS4_ADDR, 1, &tmp);
  return tmp;
}


/**
* @brief  Get the SOURCE_REFERENCE set on the chip.
* @param  None.
* @retval uint8_t SOURCE_REFERENCE set.
*/
uint8_t S2LPGetRxSourceReferenceAddress(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKT_FLT_GOALS3_ADDR, 1, &tmp);
  return tmp;
}



/**
* @brief  Set the SOURCE_MASK .
* @param  mask Source mask to be set .
* @retval None.
*/
void S2LPSetRxSourceMask(uint8_t mask)
{
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS4_ADDR, 1, &mask);
}


/**
* @brief  Set the SOURCE_REFERENCEK.
* @param  address Source address to be used as a reference .
* @retval None.
*/
void S2LPSetRxSourceReferenceAddress(uint8_t address)
{
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS3_ADDR, 1, &address);
}


/**
* @brief  Set the BROADCAST_ADDRESS set on the chip.
* @param  address BROADCAST_ADDRESS to be set.
* @retval None.
*/
void S2LPSetBroadcastAddress(uint8_t address)
{
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS2_ADDR, 1, &address);
}


/**
* @brief  Set the MULTICAST_ADDRESS set on the chip.
* @param  address MULTICAST_ADDRESS to be set.
* @retval None.
*/
void S2LPSetMulticastAddress(uint8_t address)
{
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS1_ADDR, 1, &address);
}



/**
* @brief  Set the MY_ADDRESS (source address that will be transmitted).
* @param  address MY_ADDRESS to be set.
* @retval None.
*/
void S2LPSetMyAddress(uint8_t address)
{
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS0_ADDR, 1, &address);
}




/**
 * @brief  Set the DirectRF RX mode of S2LP.
 * @param  xDirectRx code of the desired mode.
 *         This parameter can be any value of @ref DirectRx.
 * @retval None.
 */
void S2LPPacketHandlerSetRxMode(DirectRx xDirectRx)
{
  uint8_t tmp;
  s_assert_param(IS_DIRECT_RX(xDirectRx));

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmp);
  tmp &= ~RX_MODE_REGMASK;
  tmp |= (uint8_t)xDirectRx;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL3_ADDR, 1, &tmp);

}


/**
 * @brief  Return the DirectRF RX mode of S2LP.
 * @param  None.
 * @retval DirectRx Direct Rx mode.
 */
DirectRx S2LPPacketHandlerGetRxMode(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmp);
  return (DirectRx)(tmp & RX_MODE_REGMASK);

}


/**
 * @brief  Set the TX mode of S2LP.
 * @param  xDirectTx code of the desired source.
 *         This parameter can be any value of @ref DirectTx.
 * @retval None.
 */
void S2LPPacketHandlerSetTxMode(DirectTx xDirectTx)
{
  uint8_t tmp;
  s_assert_param(IS_DIRECT_TX(xDirectTx));

  S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);
  tmp &= ~TXSOURCE_REGMASK;
  tmp |= (uint8_t)xDirectTx;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL1_ADDR, 1, &tmp);
}


/**
 * @brief  Return the DirectRF TX mode of S2LP.
 * @param  None.
 * @retval DirectTx Direct Tx mode.
 */
DirectTx S2LPPacketHandlerGetTxMode(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);
  return (DirectTx)(tmp & TXSOURCE_REGMASK);
}

/**
 * @brief  Returns the sequence number of the transmitted packet.
 * @param  None.
 * @retval uint8_t Sequence number of the transmitted packet.
 */
uint8_t S2LPPacketHandlerGetTransmittedSeqNumber(void)
{
  uint8_t tempRegValue;

  /* Reads the TX_PCKT_INFO register value */
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(TX_PCKT_INFO_ADDR, 1, &tempRegValue);

  /* Obtains and returns the TX sequence number */
  return (tempRegValue >> 4) & 0x07;

}

/**
 * @brief  Set the extended length field in case of variable length.
 * @param  xExtendedLenField enable (S_ENABLE) or disable (S_DISABLE).
 * @retval None.
 */
void S2LPPacketHandlerSetExtendedLenField(SFunctionalState xExtendedLenField)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xExtendedLenField));

  S2LPSpiReadRegisters(PCKTCTRL4_ADDR, 1, &tmp);
  if(xExtendedLenField == S_ENABLE) {
    tmp |= LEN_WID_REGMASK;
  }
  else {
    tmp &= ~LEN_WID_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL4_ADDR, 1, &tmp);
}


/**
 * @brief  Swap the 4FSK symbol mapping.
 * @param  xSwapSymbol enable (S_ENABLE) or disable (S_DISABLE).
 * @retval None.
 */
void S2LPPacketHandlerSwap4FSKSymbol(SFunctionalState xSwapSymbol)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xSwapSymbol));

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmp);
  if(xSwapSymbol == S_ENABLE) {
    tmp |= FSK4_SYM_SWAP_REGMASK;
  }
  else {
    tmp &= ~FSK4_SYM_SWAP_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL3_ADDR, 1, &tmp);
}

/**
 * @brief  Change the FIFO endianness .
 * @param  xEnableSwap if it is S_DISABLE the FIFO will be accessed in big endian mode.
 *         if it is S_ENABLE the FIFO will be accessed in little endian mode.
 * @retval None.
 */
void S2LPPacketHandlerSwapFifoEndianess(SFunctionalState xEnableSwap)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xEnableSwap));

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmp);
  if(xEnableSwap == S_ENABLE) {
    tmp |= BYTE_SWAP_REGMASK;
  }
  else {
    tmp &= ~BYTE_SWAP_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL3_ADDR, 1, &tmp);
}

/**
 * @brief  Swap preamble pattern.
 * @param  xEnableSwap if it is S_DISABLE the preamble pattern will be 10101010.
 *         if it is S_ENABLE the preamble pattern will be 01010101.
 * @retval None.
 */
void S2LPPacketHandlerSwapPreamblePattern(SFunctionalState xEnableSwap)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xEnableSwap));

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmp);
  tmp&=0xFC;

  if(xEnableSwap == S_DISABLE) {
    tmp |= 0x01;
  }

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL3_ADDR, 1, &tmp);
}



/**
* @brief  Set the variable length mode.
* @param  xVarLen if enabled (S_ENABLE) the length field will be set into the packet.
* @retval None.
*/
void S2LPPacketHandlerSetVariableLength(SFunctionalState xVarLen)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xVarLen));

  S2LPSpiReadRegisters(PCKTCTRL2_ADDR, 1, &tmp);
  if(xVarLen == S_ENABLE) {
    tmp |= FIX_VAR_LEN_REGMASK;
  }
  else {
    tmp &= ~FIX_VAR_LEN_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL2_ADDR, 1, &tmp);
}


/**
* @brief  Set the CRC mode.
* @param  xPktCrcMode This parameter can be one among the @ref PktCrcMode .
* @retval None.
*/
void S2LPPacketHandlerSetCrcMode(PktCrcMode xPktCrcMode)
{
  uint8_t tmp;
  s_assert_param(IS_PKT_CRC_MODE(xPktCrcMode));

  S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);
  tmp &= ~CRC_MODE_REGMASK;
  tmp |= (uint8_t)xPktCrcMode;
  S2LPSpiWriteRegisters(PCKTCTRL1_ADDR, 1, &tmp);

}


/**
* @brief  Get the CRC mode.
* @param  None.
* @retval PktCrcMode CRC mode in the @ref PktCrcMode enum.
*/
PktCrcMode S2LPPacketHandlerGetCrcMode(void)
{
  uint8_t tmp;
  S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);
  tmp &= ~CRC_MODE_REGMASK;
  return (PktCrcMode)tmp;
}



/**
* @brief  Select the secondary sync mode.
*        In TX if enabled: it will send the secondary sync word (from the PCKT_FLT_GOALSx),
*         otherwise it will send the primary sync (from the SYNCx registers).
*        In RX if enabled: it will enable the double sync mode seeking for the 2 sync words in SYNCx or PCKT_FLT_GOALSx in parallel.
* @param  xSecondarySync can be S_ENABLE or S_DISABLE.
* @retval None.
*/
void S2LPPacketHandlerSelectSecondarySync(SFunctionalState xSecondarySync)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xSecondarySync));

  S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);
  if(xSecondarySync == S_ENABLE) {
    tmp |= SECOND_SYNC_SEL_REGMASK;
  }
  else {
    tmp &= ~SECOND_SYNC_SEL_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL1_ADDR, 1, &tmp);

}


/**
* @brief  Enable or Disable the auto packet filter mechanisms.
* @note         This must be enabled for any filtering.
* @param  xNewState enable(S_ENABLE) or disable (S_DISABLE).
* @retval None.
*/
void S2LPPacketHandlerSetAutoPcktFilter(SFunctionalState xNewState)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PROTOCOL1_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= AUTO_PCKT_FLT_REGMASK;
  }
  else {
    tmp &= ~AUTO_PCKT_FLT_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PROTOCOL1_ADDR, 1, &tmp);
}



/**
* @brief  Set the RX persistent mode. The device will be ever in RX unles an abort command comes.
* @param  xNewState enable(S_ENABLE) or disable (S_DISABLE).
* @retval None.
*/
void S2LPPacketHandlerSetRxPersistentMode(SFunctionalState xNewState)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PROTOCOL0_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= PERS_RX_REGMASK;
  }
  else {
    tmp &= ~PERS_RX_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PROTOCOL0_ADDR, 1, &tmp);
}


/**
* @brief  Set the source address filtering.
* @param  xNewState enable(S_ENABLE) or disable (S_DISABLE).
* @retval None.
*/
void S2LPPacketHandlerSetSrcAddrFlt(SFunctionalState xNewState)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= SOURCE_ADDR_FLT_REGMASK;
  }
  else {
    tmp &= ~SOURCE_ADDR_FLT_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);
}



/**
* @brief  Set the secondary sync word.
* @param  lSyncWords secondary sync word.
* @retval None.
*/
void S2LPSetDualSyncWords(uint32_t lSyncWords)
{
  uint8_t tmpBuffer[4];

  /* Split the 32-bit value in 4 8-bit values */
  for(uint8_t i=0 ; i<4 ; i++) {
    tmpBuffer[i]=(uint8_t)(lSyncWords>>(8*i));
  }

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS3_ADDR, 4, tmpBuffer);
}


/**
* @brief  Get the secondary sync word.
* @param  lSyncWords pointer to the secondary sync word variable.
* @retval None.
*/
void S2LPGetDualSyncWords(uint32_t* lSyncWords)
{
  uint8_t tmpBuffer[4];

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKT_FLT_GOALS3_ADDR, 4, tmpBuffer);
  *lSyncWords = ((uint32_t)tmpBuffer[0]<<24) | ((uint32_t)tmpBuffer[1]<<16) | ((uint32_t)tmpBuffer[2]<<8) | ((uint32_t)tmpBuffer[3]);
}


/**
 *@}
 */

/**
 *@}
 */


/**
 *@}
 */


/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
