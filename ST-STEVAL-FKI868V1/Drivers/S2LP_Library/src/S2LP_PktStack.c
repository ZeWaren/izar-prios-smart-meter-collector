/**
 * @file    S2LP_PktStack.c
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP STack packets.
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
#include "S2LP_PktStack.h"
#include "S2LP_PktWMbus.h"


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_PktStack
 * @{
 */

/**
 * @defgroup PktStack_Private_Defines                   Pkt STack Private Defines
 * @{
 */
#define PKT_FORMAT_STACK_CODE           (uint8_t)3

/**
 *@}
 */


/**
 * @defgroup PktStack_Private_Macros                    Pkt STack Private Macros
 * @{
 */


#define IS_STACK_PREAMBLE_LENGTH           IS_PREAMBLE_LEN
#define IS_STACK_SYNC_LENGTH               IS_SYNC_LEN
#define IS_STACK_PKT_LEN_FIELD_WID         IS_PKT_LEN_FIELD_WID
#define IS_STACK_CRC_MODE                  IS_PKT_CRC_MODE
#define IS_STACK_NMAX_RETX(NRETX)          (NRETX<=15)
#define IS_STACK_SEQNUM_RELOAD_VAL(VAL)    (VAL<=3)

/**
 *@}
 */


/**
 * @defgroup PktStack_Private_Functions                 Pkt STack Private Functions
 * @{
 */


/**
 * @brief  Initialize the S2LP STack packet according to the specified
 *         parameters in the PktStackInit.
 * @param  pxPktStackInit STack packet init structure.
 *         This parameter is a pointer to @ref PktStackInit.
 * @retval None.
 */
void S2LPPktStackInit(PktStackInit* pxPktStackInit)
{
  uint8_t tmpBuffer[6];

  /* Check the parameters */
  s_assert_param(IS_STACK_PREAMBLE_LENGTH(pxPktStackInit->xPreambleLength));
  s_assert_param(IS_STACK_SYNC_LENGTH(pxPktStackInit->xSyncLength));
  s_assert_param(IS_STACK_CRC_MODE(pxPktStackInit->xCrcMode));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktStackInit->cExtendedPktLenField));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktStackInit->xFixVarLength));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktStackInit->xFec));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktStackInit->xDataWhitening));

  S2LPPktWMbusSetSubmode(WMBUS_SUBMODE_NOT_CONFIGURED);

  /* Always set the automatic packet filtering */
  S2LPSpiReadRegisters(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);
  tmpBuffer[0] |= AUTO_PCKT_FLT_REGMASK;
  S2LPSpiWriteRegisters(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);

  tmpBuffer[0] = ((pxPktStackInit->xSyncLength)<<2) | (uint8_t)((pxPktStackInit->xPreambleLength)>>8);
  tmpBuffer[1] = (uint8_t)(pxPktStackInit->xPreambleLength);
  tmpBuffer[2] = ((uint8_t)pxPktStackInit->cExtendedPktLenField)<<7 | ADDRESS_LEN_REGMASK;

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmpBuffer[3]);
  tmpBuffer[3] &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK);
  tmpBuffer[3] |= PKT_FORMAT_STACK_CODE<<6;

  S2LPSpiReadRegisters(PCKTCTRL2_ADDR, 2, &tmpBuffer[4]);

  if(pxPktStackInit->xFixVarLength == S_ENABLE) {
     tmpBuffer[4] |= FIX_VAR_LEN_REGMASK;
  }
  else {
     tmpBuffer[4] &= ~FIX_VAR_LEN_REGMASK;
  }
  tmpBuffer[4] &= ~(MANCHESTER_EN_REGMASK | MBUS_3OF6_EN_REGMASK);

  tmpBuffer[5] &= ~(CRC_MODE_REGMASK | TXSOURCE_REGMASK);
  tmpBuffer[5] |= (uint8_t)pxPktStackInit->xCrcMode;

  if(pxPktStackInit->xDataWhitening == S_ENABLE) {
     tmpBuffer[5] |= WHIT_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~WHIT_EN_REGMASK;
  }

  if(pxPktStackInit->xFec == S_ENABLE) {
     tmpBuffer[5] |= FEC_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~FEC_EN_REGMASK;
  }

  S2LPSpiWriteRegisters(PCKTCTRL6_ADDR, 6, tmpBuffer);

  /* SYNC word */
  for(uint8_t i=0 ; i<4 ; i++) {
    tmpBuffer[i] = (uint8_t)(pxPktStackInit->lSyncWords>>(8*i));
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(SYNC3_ADDR, 4, tmpBuffer);

  /* Sets CRC check bit */
  if(pxPktStackInit->xCrcMode == PKT_NO_CRC) {
    S2LPPktStackFilterOnCrc(S_DISABLE);
  }
  else {
    S2LPPktStackFilterOnCrc(S_ENABLE);
  }
    /* Constellation map setting */
  S2LPSpiReadRegisters(MOD1_ADDR, 1, tmpBuffer);
  tmpBuffer[0] &= ~G4FSK_CONST_MAP_REGMASK;
  S2LPSpiWriteRegisters(MOD1_ADDR, 1, tmpBuffer);
}


/**
 * @brief  Return the S2LP STack packet structure according to the specified parameters in the registers.
 * @param  pxPktStackInit STack packet init structure.
 *         This parameter is a pointer to @ref PktStackInit.
 * @retval None.
 */
void S2LPPktStackGetInfo(PktStackInit* pxPktStackInit)
{
  uint8_t tmpBuffer[6];

  S2LPSpiReadRegisters(PCKTCTRL6_ADDR, 6, tmpBuffer);

  /* Sync length */
  pxPktStackInit->xSyncLength = ((tmpBuffer[0] & SYNC_LEN_REGMASK)>>2);

  /* Preamble length */
  pxPktStackInit->xPreambleLength = (((uint16_t)(tmpBuffer[0] & PREAMBLE_LEN_9_8_REGMASK))<<8) | ((uint16_t)tmpBuffer[1]);

  /* Length width */
  pxPktStackInit->cExtendedPktLenField = (SFunctionalState)((tmpBuffer[2] & LEN_WID_REGMASK)>>7);

  /* FIX or VAR bit */
  pxPktStackInit->xFixVarLength = (SFunctionalState)(tmpBuffer[4] & FIX_VAR_LEN_REGMASK);

  /* CRC mode */
  pxPktStackInit->xCrcMode = (StackCrcMode)(tmpBuffer[5] & CRC_MODE_REGMASK);

  /* Whitening */
  pxPktStackInit->xDataWhitening = (SFunctionalState)((tmpBuffer[5] & WHIT_EN_REGMASK)>> 4);

  /* FEC */
  pxPktStackInit->xFec = (SFunctionalState)(tmpBuffer[5] & FEC_EN_REGMASK);

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(SYNC3_ADDR, 4, tmpBuffer);

  /* SYNC word */
  pxPktStackInit->lSyncWords = 0;
  for(uint8_t i=0 ; i<4 ; i++) {
    pxPktStackInit->lSyncWords |= ((uint32_t)tmpBuffer[i])<<(8*i);
  }
}


/**
 * @brief  Initialize the S2LP STack packet addresses according to the specified
 *         parameters in the PktStackAddresses struct.
 * @param  pxPktStackAddresses STack packet addresses init structure.
 *         This parameter is a pointer to @ref PktStackAddressesInit .
 * @retval None.
 */
void S2LPPktStackAddressesInit(PktStackAddressesInit* pxPktStackAddresses)
{
  uint8_t tmpBuffer[3];
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktStackAddresses->xFilterOnMyAddress));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktStackAddresses->xFilterOnMulticastAddress));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktStackAddresses->xFilterOnBroadcastAddress));

  /* Reads the PCKT_FLT_OPTIONS ragister */
  S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);

  /* Enables or disables filtering on my address */
  if(pxPktStackAddresses->xFilterOnMyAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_SOURCE_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_SOURCE_ADDR_REGMASK;
  }

  /* Enables or disables filtering on multicast address */
  if(pxPktStackAddresses->xFilterOnMulticastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_MULTICAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_MULTICAST_ADDR_REGMASK;
  }

  /* Enables or disables filtering on broadcast address */
  if(pxPktStackAddresses->xFilterOnBroadcastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_BROADCAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_BROADCAST_ADDR_REGMASK;
  }

  S2LPSpiWriteRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);

  /* Fills the array with the addresses passed in the structure */
  tmpBuffer[2] = pxPktStackAddresses->cMyAddress;
  tmpBuffer[0] = pxPktStackAddresses->cBroadcastAddress;
  tmpBuffer[1] = pxPktStackAddresses->cMulticastAddress;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS2_ADDR, 3, tmpBuffer);
}


/**
* @brief  Return the S2LP STack packet addresses structure according to the specified
*         parameters in the registers.
* @param  pxPktStackAddresses STack packet addresses init structure.
*         This parameter is a pointer to @ref PktStackAddresses.
* @retval None.
*/
void S2LPPktStackGetAddressesInfo(PktStackAddressesInit* pxPktStackAddresses)
{
  uint8_t tmpBuffer[3];

  S2LPSpiReadRegisters(PCKT_FLT_GOALS3_ADDR, 3, tmpBuffer);
  pxPktStackAddresses->cMyAddress = tmpBuffer[0];
  pxPktStackAddresses->cBroadcastAddress = tmpBuffer[1];
  pxPktStackAddresses->cMulticastAddress = tmpBuffer[2];

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);
  pxPktStackAddresses->xFilterOnBroadcastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_BROADCAST_ADDR_REGMASK) >> 3);
  pxPktStackAddresses->xFilterOnMulticastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_MULTICAST_ADDR_REGMASK) >> 2);
  pxPktStackAddresses->xFilterOnMyAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_SOURCE_ADDR_REGMASK) >> 1);
}


/**
 * @brief  Configure the STack packet format for S2LP.
 * @param  None.
 * @retval None.
 */
void S2LPPktStackSetFormat(void)
{
  uint8_t tmp;

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmp);

  /* Build the new value. Also set to 0 the direct RX mode bits */
  tmp &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK);
  tmp |= PKT_FORMAT_STACK_CODE;
  S2LPSpiWriteRegisters(PCKTCTRL3_ADDR, 1, &tmp);

  S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);

  /* Set to 0 the direct TX mode bits */
  tmp &= ~TXSOURCE_REGMASK;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL1_ADDR, 1, &tmp);

  S2LPPktWMbusSetSubmode(WMBUS_SUBMODE_NOT_CONFIGURED);
}



/**
 * @brief  Set the payload length for S2LP STack packets. Since the packet length
 *         depends from the address (always 2 for this packet format)
 *         and the control field size, this function reads the control length register
 *         content in order to determine the correct packet length to be written.
 * @param  nPayloadLength payload length in bytes.
 *         This parameter can be any value of uint16_t.
 * @retval None.
 */
void S2LPPktStackSetPayloadLength(uint16_t nPayloadLength)
{
  uint8_t tmpBuffer[2];

  nPayloadLength+=2;
  tmpBuffer[0] = (uint8_t)(nPayloadLength>>8);
  tmpBuffer[1] = (uint8_t)nPayloadLength;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTLEN1_ADDR, 2, tmpBuffer);
}


/**
 * @brief  Return the payload length for S2LP STack packets. Since the
 *         packet length depends from the address and the control
 *         field size, this function reads the correspondent
 *         registers in order to determine the correct payload length
 *         to be returned.
 * @param  None.
 * @retval uint16_t Payload length.
 */
uint16_t S2LPPktStackGetPayloadLength(void)
{
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKTLEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);
  nPayloadLength-=2;

  return nPayloadLength;
}


/**
 * @brief  Return the packet length field of the received packet.
 * @param  None.
 * @retval uint16_t Packet length.
 */
uint16_t S2LPPktStackGetReceivedPktLength(void)
{
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(RX_PCKT_LEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);
    nPayloadLength--;

  return nPayloadLength;
}


/**
* @brief  Se the AUTO_ACK bit on the receiver .
* @param  xNewState if S_ENABLE, the receiver will check the NO_ACK bit to see if the ack should be sent (NO_ACK=0) or not (NO_ACK=1).
* @retval None.
*/
void S2LPPktStackAutoAck(SFunctionalState xNewState)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PROTOCOL0_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= AUTO_ACK_REGMASK;
  }
  else {
    tmp &= ~AUTO_ACK_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PROTOCOL0_ADDR, 1, &tmp);

}


/**
* @brief  Set the number of retransmissions to be done in case of ACK loss.
* @param  nRetx number of retransmissions.
* @retval None.
*/
void S2LPPktStackNRetx(uint8_t nRetx)
{
  uint8_t tmp;
  s_assert_param(IS_STACK_NMAX_RETX(nRetx));

  S2LPSpiReadRegisters(PROTOCOL0_ADDR, 1, &tmp);
  tmp &= ~NMAX_RETX_REGMASK;
  tmp |= (nRetx<<4);
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PROTOCOL0_ADDR, 1, &tmp);

}


/**
* @brief  Get the NO_ACK bit.
* @param  None.
* @retval SFlagStatus if it is S_SET, the ack will be not requested, otherwise it will be.
*/
SFlagStatus S2LPPktStackGetTXAckRequest(void)
{
  uint8_t tmp;

  S2LPSpiReadRegisters(RX_PCKT_INFO_ADDR, 1, &tmp);
  tmp &= NACK_RX_REGMASK;
  tmp >>= 2;
  return (SFlagStatus)tmp;
}

/**
* @brief  This function will set the NO_ACK bit or reset it.
* @param  xNewState if it is S_DISABLE, the ack will be not request and thus the NO_ACK bit will be set to 1.
*         if this parameter is S_ENABLE, the ack will be request and thus the NO_ACK bit will be set to 0.
* @retval None.
*/
void S2LPPktStackAckRequest(SFunctionalState xNewState)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PROTOCOL0_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp &= ~NACK_TX_REGMASK;
  }
  else {
    tmp |= NACK_TX_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PROTOCOL0_ADDR, 1, &tmp);

}


/**
* @brief  Enable or Disable the piggybacking.
* @param  xNewState enable or disable.
* @retval None.
*/
void S2LPPktStackPiggybacking(SFunctionalState xNewState)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PROTOCOL1_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= PIGGYBACKING_REGMASK;
  }
  else {
    tmp &= ~PIGGYBACKING_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PROTOCOL1_ADDR, 1, &tmp);

}


/**
* @brief  Set the reload value of the sequence number.
* @note  A SEQ_NUM_RELOAD command must be strobed to make this value available for the next packet.
* @param  cReloadValue reload value.
* @retval None.
*/
void S2LPPktStackSeqNumForReload(uint8_t cReloadValue)
{
  uint8_t tmp;
  s_assert_param(IS_STACK_SEQNUM_RELOAD_VAL(cReloadValue));

  S2LPSpiReadRegisters(PROTOCOL2_ADDR, 1, &tmp);
  tmp &= ~TX_SEQ_NUM_RELOAD_REGMASK;
  tmp |= (cReloadValue<<3);
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PROTOCOL2_ADDR, 1, &tmp);
}

/**
 * @brief  Returns the number of retransmission done on the transmitted packet.
 * @param  None.
 * @retval uint8_t Number of retransmissions done until now.
 */
uint8_t S2LPPktStackGetNReTx(void)
{
  uint8_t tempRetValue;

  /* Reads the TX_PCKT_INFO register value */
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(TX_PCKT_INFO_ADDR, 1, &tempRetValue);

  /* Obtains and returns the number of retransmission done */
  return (tempRetValue & N_RETX_REGMASK);

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
