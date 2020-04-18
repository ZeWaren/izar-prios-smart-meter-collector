/**
 * @file    S2LP_PktBasic.c
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP Basic packets.
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
#include "S2LP_PktBasic.h"
#include "S2LP_PktWMbus.h"


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_PktBasic
 * @{
 */



/**
 * @defgroup PktBasic_Private_Defines           Pkt Basic Private Defines
 * @{
 */

#define PKT_FORMAT_BASIC_CODE                   0x00

/**
 *@}
 */


/**
 * @defgroup PktBasic_Private_Macros            Pkt Basic Private Macros
 * @{
 */

#define IS_BASIC_PREAMBLE_LENGTH           IS_PREAMBLE_LEN
#define IS_BASIC_SYNC_LENGTH               IS_SYNC_LEN
#define IS_BASIC_PKT_LEN_FIELD_WID         IS_PKT_LEN_FIELD_WID
#define IS_BASIC_CRC_MODE                  IS_PKT_CRC_MODE

/**
 *@}
 */



/**
 * @defgroup PktBasic_Private_Functions                 Pkt Basic Private Functions
 * @{
 */

/**
 * @brief  Initialize the S2LP Basic packet according to the specified parameters in the PktBasicInit struct.
 *         Notice that this function sets the autofiltering option on CRC if it is set to any value different from BASIC_NO_CRC.
 * @param  pxPktBasicInit Basic packet init structure.
 *         This parameter is a pointer to @ref PktBasicInit.
 * @retval None.
 */
void S2LPPktBasicInit(PktBasicInit* pxPktBasicInit)
{
  uint8_t tmpBuffer[6];

  /* Check the parameters */
  s_assert_param(IS_BASIC_PREAMBLE_LENGTH(pxPktBasicInit->xPreambleLength));
  s_assert_param(IS_BASIC_SYNC_LENGTH(pxPktBasicInit->xSyncLength));
  s_assert_param(IS_BASIC_CRC_MODE(pxPktBasicInit->xCrcMode));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->cExtendedPktLenField));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->xFixVarLength));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->xAddressField));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->xFec));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->xDataWhitening));

  S2LPPktWMbusSetSubmode(WMBUS_SUBMODE_NOT_CONFIGURED);

  /* Always set the automatic packet filtering */
  S2LPSpiReadRegisters(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);
  // tmpBuffer[0] |= AUTO_PCKT_FLT_REGMASK;
  tmpBuffer[0] &= ~(AUTO_PCKT_FLT_REGMASK);
  S2LPSpiWriteRegisters(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);

  tmpBuffer[0] = ((pxPktBasicInit->xSyncLength)<<2) | (uint8_t)((pxPktBasicInit->xPreambleLength)>>8);
  tmpBuffer[1] = (uint8_t)(pxPktBasicInit->xPreambleLength);
  tmpBuffer[2] = (((uint8_t)pxPktBasicInit->xAddressField)<<3);

  if((pxPktBasicInit->cExtendedPktLenField)==S_ENABLE)
  {
    tmpBuffer[2]|=0x80;
  }

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmpBuffer[3]);
  tmpBuffer[3] &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK);
  tmpBuffer[3] |= PKT_FORMAT_BASIC_CODE;

  S2LPSpiReadRegisters(PCKTCTRL2_ADDR, 2, &tmpBuffer[4]);

  if(pxPktBasicInit->xFixVarLength == S_ENABLE) {
     tmpBuffer[4] |= FIX_VAR_LEN_REGMASK;
  }
  else {
     tmpBuffer[4] &= ~FIX_VAR_LEN_REGMASK;
  }
  tmpBuffer[4] &= ~(MANCHESTER_EN_REGMASK | MBUS_3OF6_EN_REGMASK);
  //tmpBuffer[4] |= MANCHESTER_EN_REGMASK;
  tmpBuffer[4] |= MBUS_3OF6_EN_REGMASK;

  tmpBuffer[5] &= ~(CRC_MODE_REGMASK | TXSOURCE_REGMASK);
  tmpBuffer[5] |= (uint8_t)pxPktBasicInit->xCrcMode;

  if(pxPktBasicInit->xDataWhitening == S_ENABLE) {
     tmpBuffer[5] |= WHIT_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~WHIT_EN_REGMASK;
  }

  if(pxPktBasicInit->xFec == S_ENABLE)
  {
     tmpBuffer[5] |= FEC_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~FEC_EN_REGMASK;
  }

  S2LPSpiWriteRegisters(PCKTCTRL6_ADDR, 6, tmpBuffer);

  /* SYNC word */
  for(uint8_t i=0 ; i<4 ; i++) {
    tmpBuffer[i] = (uint8_t)(pxPktBasicInit->lSyncWords>>(8*i));
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(SYNC3_ADDR, 4, tmpBuffer);

  /* Sets CRC check bit */
  if(pxPktBasicInit->xCrcMode == PKT_NO_CRC) {
    S2LPPktBasicFilterOnCrc(S_DISABLE);
  }
  else {
    S2LPPktBasicFilterOnCrc(S_ENABLE);
  }

    /* Constellation map setting */
  S2LPSpiReadRegisters(MOD1_ADDR, 1, tmpBuffer);
  tmpBuffer[0] &= ~G4FSK_CONST_MAP_REGMASK;
  S2LPSpiWriteRegisters(MOD1_ADDR, 1, tmpBuffer);
}


/**
 * @brief  Return the S2LP Basic packet structure according to the specified parameters in the registers.
 * @param  pxPktBasicInit Basic packet init structure.
 *         This parameter is a pointer to @ref PktBasicInit.
 * @retval None.
 */
void S2LPPktBasicGetInfo(PktBasicInit* pxPktBasicInit)
{
  uint8_t tmpBuffer[6];

  S2LPSpiReadRegisters(PCKTCTRL6_ADDR, 6, tmpBuffer);

  /* Sync length */
  pxPktBasicInit->xSyncLength = ((tmpBuffer[0] & SYNC_LEN_REGMASK)>>2);

  /* Preamble length */
  pxPktBasicInit->xPreambleLength = (((uint16_t)(tmpBuffer[0] & PREAMBLE_LEN_9_8_REGMASK))<<8) | ((uint16_t)tmpBuffer[1]);

  /* Length width */
  pxPktBasicInit->cExtendedPktLenField = (SFunctionalState)((tmpBuffer[2] & LEN_WID_REGMASK)>>7);

  /* Address field */
  pxPktBasicInit->xAddressField = (SFunctionalState)((tmpBuffer[2] & ADDRESS_LEN_REGMASK)>>3);

  /* FIX or VAR bit */
  pxPktBasicInit->xFixVarLength = (SFunctionalState)(tmpBuffer[4] & FIX_VAR_LEN_REGMASK);

  /* CRC mode */
  pxPktBasicInit->xCrcMode = (BasicCrcMode)(tmpBuffer[5] & CRC_MODE_REGMASK);

  /* Whitening */
  pxPktBasicInit->xDataWhitening = (SFunctionalState)((tmpBuffer[5] & WHIT_EN_REGMASK)>> 4);

  /* FEC */
  pxPktBasicInit->xFec = (SFunctionalState)(tmpBuffer[5] & FEC_EN_REGMASK);

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(SYNC3_ADDR, 4, tmpBuffer);

  /* SYNC word */
  pxPktBasicInit->lSyncWords = 0;
  for(uint8_t i=0 ; i<4 ; i++) {
    pxPktBasicInit->lSyncWords |= ((uint32_t)tmpBuffer[i])<<(8*i);
  }
}


/**
 * @brief  Initialize the S2LP Basic packet addresses according to the specified
 *         parameters in the PktBasicAddressesInit struct.
 * @param  pxPktBasicAddresses Basic packet addresses init structure.
 *         This parameter is a pointer to @ref PktBasicAddresses.
 * @retval None.
 */
void S2LPPktBasicAddressesInit(PktBasicAddressesInit* pxPktBasicAddresses)
{
  uint8_t tmpBuffer[3];
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicAddresses->xFilterOnMyAddress));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicAddresses->xFilterOnMulticastAddress));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicAddresses->xFilterOnBroadcastAddress));

  /* Reads the PCKT_FLT_OPTIONS ragister */
  S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);

  /* Enables or disables filtering on my address */
  if(pxPktBasicAddresses->xFilterOnMyAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_SOURCE_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_SOURCE_ADDR_REGMASK;
  }

  /* Enables or disables filtering on multicast address */
  if(pxPktBasicAddresses->xFilterOnMulticastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_MULTICAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_MULTICAST_ADDR_REGMASK;
  }

  /* Enables or disables filtering on broadcast address */
  if(pxPktBasicAddresses->xFilterOnBroadcastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_BROADCAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_BROADCAST_ADDR_REGMASK;
  }

  S2LPSpiWriteRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);

  /* Fills the array with the addresses passed in the structure */
  tmpBuffer[2] = pxPktBasicAddresses->cMyAddress;
  tmpBuffer[0] = pxPktBasicAddresses->cBroadcastAddress;
  tmpBuffer[1] = pxPktBasicAddresses->cMulticastAddress;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS2_ADDR, 3, tmpBuffer);
}


/**
 * @brief  Return the S2LP Basic packet addresses structure according to the specified
 *         parameters in the registers.
 * @param  pxPktBasicAddresses Basic packet addresses init structure.
 *         This parameter is a pointer to @ref PktBasicAddresses.
 * @retval None.
 */
void S2LPPktBasicGetAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses)
{
  uint8_t tmpBuffer[3];

  S2LPSpiReadRegisters(PCKT_FLT_GOALS3_ADDR, 3, tmpBuffer);
  pxPktBasicAddresses->cMyAddress = tmpBuffer[0];
  pxPktBasicAddresses->cBroadcastAddress = tmpBuffer[1];
  pxPktBasicAddresses->cMulticastAddress = tmpBuffer[2];

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);
  pxPktBasicAddresses->xFilterOnBroadcastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_BROADCAST_ADDR_REGMASK) >> 3);
  pxPktBasicAddresses->xFilterOnMulticastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_MULTICAST_ADDR_REGMASK) >> 2);
  pxPktBasicAddresses->xFilterOnMyAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_SOURCE_ADDR_REGMASK) >> 1);
}


/**
 * @brief  Configure the Basic packet format as packet used by S2LP.
 * @param  None.
 * @retval None.
 */
void S2LPPktBasicSetFormat(void)
{
  uint8_t tmp;

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmp);

  /* Build the new value. Also set to 0 the direct RX mode bits */
  tmp &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK | PREAMBLE_SEL_REGMASK | BYTE_SWAP_REGMASK);
  tmp |= PKT_FORMAT_BASIC_CODE;
  S2LPSpiWriteRegisters(PCKTCTRL3_ADDR, 1, &tmp);

  S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);

  /* Set to 0 the direct TX mode bits */
  tmp &= ~TXSOURCE_REGMASK;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL1_ADDR, 1, &tmp);

  S2LPPktWMbusSetSubmode(WMBUS_SUBMODE_NOT_CONFIGURED);
}


/**
 * @brief  Set the address length for S2LP Basic packets.
 * @param  xAddressField length of ADDRESS in bytes.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LPPktBasicAddressField(SFunctionalState xAddressField)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xAddressField));

  S2LPSpiReadRegisters(PCKTCTRL4_ADDR, 1, &tmp);
  if(xAddressField==S_ENABLE) {
    tmp |= ADDRESS_LEN_REGMASK;
  }
  else {
    tmp &= ADDRESS_LEN_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL4_ADDR, 1, &tmp);

}


/**
 * @brief  Specify if the Address field for S2LP Basic packets is enabled or disabled.
 * @param  None.
 * @retval SFunctionalState Notifies if the address field is enabled or disabled.
 */
SFunctionalState S2LPPktBasicGetAddressField(void)
{
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKTCTRL4_ADDR, 1, &tmp);
  if(tmp & ADDRESS_LEN_REGMASK) {
    return S_ENABLE;
  }
  else {
    return S_DISABLE;
  }

}


/**
 * @brief  Set the payload length for S2LP Basic packets. Since the packet length
 *         depends from the address and the control field size, this
 *         function reads the correspondent registers in order to determine
 *         the correct packet length to be written.
 * @param  nPayloadLength payload length in bytes.
 *         This parameter is an uint16_t.
 * @retval None.
 */
void S2LPPktBasicSetPayloadLength(uint16_t nPayloadLength)
{
  uint8_t tmpBuffer[2];

  if(S2LPPktBasicGetAddressField()) {
    nPayloadLength++;
  }
  tmpBuffer[0] = (uint8_t)(nPayloadLength>>8);
  tmpBuffer[1] = (uint8_t)nPayloadLength;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PCKTLEN1_ADDR, 2, tmpBuffer);
}


/**
 * @brief  Return the payload length for S2LP Basic packets. Since the
 *         packet length depends from the address and the control
 *         field size, this function reads the correspondent
 *         registers in order to determine the correct payload length
 *         to be returned.
 * @param  None.
 * @retval uint16_t Payload length in bytes.
 */
uint16_t S2LPPktBasicGetPayloadLength(void)
{
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PCKTLEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);

  if(S2LPPktBasicGetAddressField()) {
    nPayloadLength--;
  }
  return nPayloadLength;
}

/**
 * @brief  Return the packet length field of the received packet.
 * @param  None.
 * @retval uint16_t Packet length.
 */
uint16_t S2LPPktBasicGetReceivedPktLength(void)
{
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(RX_PCKT_LEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);

  if(S2LPPktBasicGetAddressField()) {
    nPayloadLength--;
  }
  return nPayloadLength;
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
