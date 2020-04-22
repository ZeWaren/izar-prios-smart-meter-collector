/**
  ******************************************************************************
  * @file           : WMBus.c
  * @brief          : Logic to validate and parse a WMBus frame
  * @author         : Erwan Martin <public@fzwte.net>
  ******************************************************************************
  */

/* C Includes */
#include <stdint.h>

/* Application includes */
#include "WMBus.h"

#define CRC_POLYNOM			0x3D65
   
/**
* @brief  Calculates the 16-bit CRC. The function requires that the CRC_POLYNOM is defined,
*           which gives the wanted CRC polynom.
* @param  Input Param:uint8  crcData  - Data to perform the CRC-16 operation on.
*                     uint16 crcReg   - Current or initial value of the CRC calculation
* @retval LINK_STATUS : LINK_STATUS_SUCCESS
*       LINK_STATUS_INVALID_SIZE   - The data-field buffer is too large
*
*/
uint16_t crcCalc(uint16_t crcReg, uint8_t crcData) {
  uint8_t i;

  for (i = 0; i < 8; i++) {
    /* If upper most bit is 1 */
    if (((crcReg & 0x8000) >> 8) ^ (crcData & 0x80))
      crcReg = (crcReg << 1)  ^ CRC_POLYNOM;
    else
      crcReg = (crcReg << 1);

    crcData <<= 1;
  }

  return crcReg;
}


/**
* @brief  Calculates the 16-bit CRC.
* @param  Input Param:uint8  crcData  - Data to perform the CRC-16 operation on.
*                     uint16 crcReg   - Current or initial value of the CRC calculation
* @retval LINK_STATUS : LINK_STATUS_SUCCESS
*       LINK_STATUS_INVALID_SIZE   - The data-field buffer is too large
*
*/
uint8_t CRCCheck(const uint8_t * pStart, const uint8_t * const pStop) {
  uint16_t  seed=0x0000;

  while (pStart != pStop) {
      seed = crcCalc(seed, *pStart);
      pStart++;
  }
  seed =~seed;
  if((pStop[0]  == (uint8_t)(seed>>8))&&(pStop[1]  == (uint8_t)(seed))) {
      return 1;
  }
  return 0;
}

/**
  * @brief Check if data correspond to a proper WMBus frame, and return the
  *        protocol header values from it.
  * @param uint8_t *frame The location of the frame
  * @param uint8_t len The length of the frame
  * @param uint8_t *LField Where to store the L-field
  * @param uint8_t *CField Where to store the C-field
  * @param uint16_t *MField Where to store the M-field
  * @param uint32_t *A_Id Where to store the id part of the A-field
  * @param uint8_t *A_Ver Where to store the version part of the A-field
  * @param uint8_t *A_Type Where to store the type part of the A-field
  * @retval uint8_t 1 if the frame is valid, 0 otherwise
  */
uint8_t CheckWMBusFrame(const uint8_t *frame, const uint8_t len, uint8_t * const LField, uint8_t * const CField, uint16_t * const MField, uint32_t * const A_Id, uint8_t * const A_Ver, uint8_t * const A_Type) {
    if (len < 13) {
        /* The frame is too short to be what we want */
        return 0;
    }
    
    /* Check the CRC of the frame */
    uint8_t crc = CRCCheck(&frame[0], &frame[10]);
      
    if (!crc) {
        return 0;
    }
    
    for(uint16_t i=0 ; i<(len-12)/18 ; i++) {
        crc &= CRCCheck(&frame[12+18*i],&frame[12+18*(i+1)-2]);
        if (!crc) {
            return 0;
        }
    }

    if((len-12)%18!=0) {
        crc &= CRCCheck(&frame[len-((len-12)%18)], &frame[len-2]);
        if (!crc) {
            return 0;
        }
    }
    
    if (!crc) {
        return 0;
    }
    
    /* The CRC is correct. It's a proper WMBus frame */
    /* Let's extract the header fields from the payload and return them */
    *LField = frame[0];
    *CField = frame[1];
    *MField = frame[3] << 8 | frame[2];
    *A_Id = frame[4] | frame[5] << 8 | frame[6] << 16 | frame[7] << 24;
    *A_Ver = frame[8];
    *A_Type = frame[9];
    
    return 1;
}
