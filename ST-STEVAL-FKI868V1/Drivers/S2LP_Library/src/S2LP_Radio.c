/**
 * @file    S2LP_Radio.c
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   This file provides all the low level API to manage Analog and Digital
 *          radio part of S2-LP.
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
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "S2LP_Radio.h"
#include "S2LP_Config.h"


/** @addtogroup S2LP_Libraries
* @{
*/


/** @addtogroup S2LP_Radio
* @{
*/


/** @defgroup Radio_Private_Defines                     Radio Private Defines
* @{
*/

#define MAX_PA_VALUE                    14
#define MIN_PA_VALUE                    -31

#define VCO_CENTER_FREQ                 3600000000           /*!< VCO center frequency in Hz */

#define HIGH_BAND_FACTOR                4     /*!< Band select factor for high band. Factor B in the equation 2 */
#define MIDDLE_BAND_FACTOR              8      /*!< Band select factor for middle band. Factor B in the equation 2 */

#define HIGH_BAND_LOWER_LIMIT           825900000   /*!< Lower limit of the high band: 860 MHz   (S2-LPQTR)*/
#define HIGH_BAND_UPPER_LIMIT           1056000000   /*!< Upper limit of the high band: 940 MHz  (S2-LPCBQTR)*/
#define MIDDLE_BAND_LOWER_LIMIT         412900000   /*!< Lower limit of the middle band: 430 MHz (S2-LPQTR)*/
#define MIDDLE_BAND_UPPER_LIMIT         527100000   /*!< Upper limit of the middle band: 470 MHz (S2-LPCBQTR)*/

#define MINIMUM_DATARATE                 100  /*!< Minimum datarate supported by S2LP 100 bps */
#define MAXIMUM_DATARATE                 250000  /*!< Maximum datarate supported by S2LP 250 ksps */


/**
* @}
*/


/** @defgroup Radio_Private_Macros                      Radio Private Macros
* @{
*/

#define IS_PA_MAX_INDEX(INDEX)       ((INDEX)<=7)
#define IS_PAPOWER_DBM(PATABLE)      ((PATABLE)>= (MIN_PA_VALUE) && (PATABLE)<=(MAX_PA_VALUE))
#define IS_PAPOWER(PATABLE)          ((PATABLE)<=90)
#define IS_PA_STEP_WIDTH(WIDTH)      ((WIDTH)>=1 && (WIDTH)<=4)

#define IS_MODULATION(MOD) (((MOD) == MOD_NO_MOD) || \
                             ((MOD) == MOD_2FSK) || \
                             ((MOD) == MOD_4FSK) || \
                             ((MOD) == MOD_2GFSK_BT05) || \
                             ((MOD) == MOD_2GFSK_BT1) || \
                             ((MOD) == MOD_4GFSK_BT05) || \
                             ((MOD) == MOD_4GFSK_BT1) || \
                             ((MOD) == MOD_ASK_OOK) || \
                             ((MOD) == MOD_POLAR))

#define IS_AFC_MODE(MODE)       (MODE<=1)
#define IS_AFC_GAIN(GAIN)       (GAIN<=15)
#define IS_ISI_EQU(MODE)        (MODE<=2)
#define IS_CLKREC_MODE(MODE)    (MODE<=1)
#define IS_CLKREC_P_GAIN(GAIN)  (GAIN<=7)
#define IS_CLKREC_I_GAIN(GAIN)  (GAIN<=15)


#define IS_FREQUENCY_BAND_HIGH(FREQUENCY) ((FREQUENCY)>=HIGH_BAND_LOWER_LIMIT && \
                                           (FREQUENCY)<=HIGH_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND_MIDDLE(FREQUENCY) ((FREQUENCY)>=MIDDLE_BAND_LOWER_LIMIT && \
                                             (FREQUENCY)<=MIDDLE_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND(FREQUENCY) (IS_FREQUENCY_BAND_HIGH(FREQUENCY) || \
                                      IS_FREQUENCY_BAND_MIDDLE(FREQUENCY))

#define IS_CHANNEL_SPACE(CHANNELSPACE, F_Xo)    (CHANNELSPACE<=(F_Xo/32768*255))


#define IS_DATARATE(DATARATE, F_CLK)      (DATARATE>=MINIMUM_DATARATE && DATARATE<=((uint64_t)MAXIMUM_DATARATE*F_CLK/1000000)/26)


#define F_DEV_LOWER_LIMIT(F_Xo)		(F_Xo>>22)
#define F_DEV_UPPER_LIMIT(F_Xo)		(((uint64_t)787109*F_Xo/1000000)/26)
#define IS_F_DEV(FDEV,F_Xo)        (FDEV>=F_DEV_LOWER_LIMIT(F_Xo) && FDEV<=F_DEV_UPPER_LIMIT(F_Xo))

#define CH_BW_LOWER_LIMIT(F_CLK)      (((uint64_t)1100*F_CLK/1000000)/26)  /*!< Minimum value of the channel filter bandwidth */
#define CH_BW_UPPER_LIMIT(F_CLK)    (((uint64_t)800100*F_CLK/1000000)/26)  /*!< Maximum value of the channel filter bandwidth */

#define IS_CH_BW(BW,F_Xo)         ((BW)>=CH_BW_LOWER_LIMIT(F_Xo) && (BW)<=CH_BW_UPPER_LIMIT(F_Xo))

/**
* @}
*/


/** @defgroup Radio_Private_Variables                   Radio Private Variables
* @{
*/

/**
* @brief  The Xtal frequency. To be set by the user (see SetXtalFreq() function)
*/
static uint32_t s_lXtalFrequency=50000000;


/**
* @brief  It represents the available channel bandwidth times 10 for 26 Mhz xtal.
* @note   The channel bandwidth for others xtal frequencies can be computed since this table
*         multiplying the current table by a factor xtal_frequency/26e6.
*/
static const uint16_t s_vectnBandwidth26M[90]=
{
  8001, 7951, 7684, 7368, 7051, 6709, 6423, 5867, 5414, \
    4509, 4259, 4032, 3808, 3621, 3417, 3254, 2945, 2703, \
      2247, 2124, 2015, 1900, 1807, 1706, 1624, 1471, 1350, \
        1123, 1062, 1005,  950,  903,  853,  812,  735,  675, \
          561,  530,  502,  474,  451,  426,  406,  367,  337, \
            280,  265,  251,  237,  226,  213,  203,  184,  169, \
              140,  133,  126,  119,  113,  106,  101,   92,   84, \
                70,   66,   63,   59,   56,   53,   51,   46,   42, \
                  35,   33,   31,   30,   28,   27,   25,   23,   21, \
                    18,   17,   16,   15,   14,   13,   13,   12,   11
};


/**
* @}
*/


/** @defgroup Radio_Private_FunctionPrototypes          Radio Private Function Prototypes
* @{
*/
uint32_t S2LPRadioComputeDatarate(uint16_t cM, uint8_t cE);
void S2LPRadioSearchDatarateME(uint32_t lDatarate, uint16_t* pcM, uint8_t* pcE);
void S2LPRadioSearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE);
void S2LPRadioSearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE);
uint32_t S2LPRadioComputeDatarate(uint16_t cM, uint8_t cE);
uint32_t S2LPRadioComputeFreqDeviation(uint8_t cM, uint8_t cE, uint8_t bs, uint8_t refdiv);
uint32_t S2LPRadioComputeChannelFilterBw(uint8_t cM, uint8_t cE);
uint32_t S2LPRadioComputeFrequencyBase(uint32_t lSynthWord, uint8_t bs, uint8_t refdiv);
uint32_t S2LPRadioComputeSynthWord(uint32_t frequency, uint8_t refdiv);
uint8_t S2LPRadioComputeChannelSpacingRegValue(uint32_t lChannelSpace);
uint32_t S2LPRadioComputeChannelSpacing(uint8_t cChSpaceRegVal);
void S2LPRadioSearchWCP(uint8_t* cp_isel, uint8_t* pfd_split, uint32_t lFc, uint8_t refdiv);
void S2LPRadioComputeIF(uint32_t nIF, uint8_t* pcAnaIf, uint8_t* pcPcDigIf);

/**
* @}
*/


/** @defgroup Radio_Private_Functions                    Radio Private Functions
* @{
*/

/**
* @brief  Returns the mantissa and exponent, whose value used in the datarate formula
*         will give the datarate value closer to the given datarate.
* @param  fDatarate datarate expressed in bps. This parameter ranging between 100 and 500000.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
void S2LPRadioSearchDatarateME(uint32_t lDatarate, uint16_t* pcM, uint8_t* pcE)
{
  uint32_t lDatarateTmp, f_dig=s_lXtalFrequency;
  uint8_t uDrE;
  uint64_t tgt1,tgt2,tgt;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  /* Search the exponent value */
  for(uDrE = 0; uDrE != 12; uDrE++) {
    lDatarateTmp = S2LPRadioComputeDatarate(0xFFFF, uDrE);
    if(lDatarate<=lDatarateTmp)
      break;
  }
  (*pcE) = (uint8_t)uDrE;

  if(uDrE==0) {
    tgt=((uint64_t)lDatarate)<<32;
    (*pcM) = (uint16_t)(tgt/f_dig);
    tgt1=(uint64_t)f_dig*(*pcM);
    tgt2=(uint64_t)f_dig*((*pcM)+1);
  }
  else {
    tgt=((uint64_t)lDatarate)<<(33-uDrE);
    (*pcM) = (uint16_t)((tgt/f_dig)-65536);
    tgt1=(uint64_t)f_dig*((*pcM)+65536);
    tgt2=(uint64_t)f_dig*((*pcM)+1+65536);
  }


  (*pcM)=((tgt2-tgt)<(tgt-tgt1))?((*pcM)+1):(*pcM);

}

/**
* @brief  Returns the mantissa and exponent, whose value used in the frequency deviation formula
*         will give a frequency deviation value most closer to the given frequency deviation.
* @param  fFDev frequency deviation expressed in Hz. This parameter can be a value in the range [F_Xo*8/2^18, F_Xo*7680/2^18].
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
void S2LPRadioSearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE)
{
  uint8_t uFDevE, tmp, bs = MIDDLE_BAND_FACTOR, refdiv = 1;
  uint32_t lFDevTmp;
  uint64_t tgt1,tgt2,tgt;

  s_assert_param(IS_F_DEV(lFDev, s_lXtalFrequency));

  S2LPSpiReadRegisters(SYNT3_ADDR, 1, &tmp);
  if((tmp&BS_REGMASK) == 0) {
    bs = HIGH_BAND_FACTOR;
  }

  if(S2LPRadioGetRefDiv()) {
    refdiv = 2;
  }

  /* Search the exponent of the frequency deviation value */
  for(uFDevE = 0; uFDevE != 12; uFDevE++) {
    lFDevTmp = S2LPRadioComputeFreqDeviation(255, uFDevE, bs, refdiv);
    if(lFDev<lFDevTmp)
      break;
  }
  (*pcE) = (uint8_t)uFDevE;

  if(uFDevE==0)
  {
    tgt=((uint64_t)lFDev)<<22;
    (*pcM)=(uint32_t)(tgt/s_lXtalFrequency);
    tgt1=(uint64_t)s_lXtalFrequency*(*pcM);
    tgt2=(uint64_t)s_lXtalFrequency*((*pcM)+1);
  }
  else
  {
    tgt=((uint64_t)lFDev)<<(23-uFDevE);
    (*pcM)=(uint32_t)(tgt/s_lXtalFrequency)-256;
    tgt1=(uint64_t)s_lXtalFrequency*((*pcM)+256);
    tgt2=(uint64_t)s_lXtalFrequency*((*pcM)+1+256);
  }

  (*pcM)=((tgt2-tgt)<(tgt-tgt1))?((*pcM)+1):(*pcM);
}


/**
* @brief  Returns the mantissa and exponent for a given bandwidth.
*         Even if it is possible to pass as parameter any value in the below mentioned range,
*         the API will search the closer value according to a fixed table of channel
*         bandwidth values (@ref s_vectnBandwidth), as defined in the datasheet, returning the corresponding mantissa
*         and exponent value.
* @param  lBandwidth bandwidth expressed in Hz. This parameter ranging between 1100 and 800100.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
void S2LPRadioSearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE)
{
  int8_t i, i_tmp;
  uint32_t f_dig=s_lXtalFrequency;
  int32_t chfltCalculation[3];


  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  s_assert_param(IS_CH_BW(lBandwidth,f_dig));

  /* Search the channel filter bandwidth table index */
  for(i=0;i<90 && (lBandwidth<(uint32_t)(((uint64_t)s_vectnBandwidth26M[i]*f_dig)/260000));i++);

  if(i!=0) {
    /* Finds the index value with best approximation in i-1, i and i+1 elements */
    i_tmp = i;

    for(uint8_t j=0;j<3;j++) {
      if(((i_tmp+j-1)>=0) && ((i_tmp+j-1)<=89)) {
        chfltCalculation[j] = (int32_t)lBandwidth - (int32_t)(((uint64_t)s_vectnBandwidth26M[i_tmp+j-1]*f_dig)/260000);
      }
      else {
        chfltCalculation[j] = 0x7FFFFFFF;
      }
    }
    uint32_t chfltDelta = 0xFFFFFFFF;

    for(uint8_t j=0;j<3;j++) {
      if(S_ABS(chfltCalculation[j])<chfltDelta) {
        chfltDelta = S_ABS(chfltCalculation[j]);
        i=i_tmp+j-1;
      }
    }
  }
  (*pcE) = (uint8_t)(i/9);
  (*pcM) = (uint8_t)(i%9);

}

/**
* @brief  Returns the mantissa and exponent, whose value used in the datarate formula
*         will give the datarate value closer to the given datarate.
* @param  fDatarate datarate expressed in bps. This parameter ranging between 100 and 500000.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
uint32_t S2LPRadioComputeDatarate(uint16_t cM, uint8_t cE)
{
  uint32_t f_dig=s_lXtalFrequency;
  uint64_t dr;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  if(cE==0) {
    dr=((uint64_t)f_dig*cM);
    return (uint32_t)(dr>>32);
  }

  dr=((uint64_t)f_dig)*((uint64_t)cM+65536);

  return (uint32_t)(dr>>(33-cE));
}

/**
* @brief  Returns the mantissa and exponent, whose value used in the datarate formula
*         will give the datarate value closer to the given datarate.
* @param  fDatarate datarate expressed in bps. This parameter ranging between 100 and 500000.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
uint32_t S2LPRadioComputeFreqDeviation(uint8_t cM, uint8_t cE, uint8_t bs, uint8_t refdiv)
{
  uint32_t f_xo=s_lXtalFrequency;

  if(cE==0) {
    return (uint32_t)(((uint64_t)f_xo*cM)>>22);
  }

  return (uint32_t)(((uint64_t)f_xo*(256+cM))>>(23-cE));
}


/**
* @brief  Computes the channel filter value starting from mantissa and exponent.
* @param  cM mantissa value.
* @param  cE exponent value.
* @retval uint32_t the channel filter value in Hz.
*/
uint32_t S2LPRadioComputeChannelFilterBw(uint8_t cM, uint8_t cE)
{
  uint32_t f_dig=s_lXtalFrequency;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  return (uint32_t)((uint64_t)100*s_vectnBandwidth26M[cM+(cE*9)]*f_dig/26000000);

}


/**
* @brief  Computes a frequency from a given SYNTH word.
* @param  lSynthWord the given SYNTH word.
* @param  bs value of the PLL divider.
* @param  refdiv reference divider value value (it can be only 1 or 2).
* @retval uint32_t the frequency base filter value in Hz.
*/
uint32_t S2LPRadioComputeFrequencyBase(uint32_t lSynthWord, uint8_t bs, uint8_t refdiv)
{
  return (uint32_t)((((uint64_t)s_lXtalFrequency*lSynthWord)>>19)/bs/refdiv);
}


/**
* @brief  Computes the synth word from a given frequency.
* @param  frequency Target frequency value expressed in Hz.
* @param  refdiv reference divider value value (it can be only 1 or 2).
* @retval uint32_t SYNTH_WORD.
*/
uint32_t S2LPRadioComputeSynthWord(uint32_t frequency, uint8_t refdiv)
{
  uint8_t band;

  if(IS_FREQUENCY_BAND_HIGH(frequency)) {
    band = HIGH_BAND_FACTOR;
  }
  else {
    band = MIDDLE_BAND_FACTOR;
  }

  uint64_t tgt1,tgt2,tgt;
  uint32_t synth;

  tgt = (((uint64_t)frequency)<<19)*(band*refdiv);
  synth=(uint32_t)(tgt/s_lXtalFrequency);
  tgt1 = (uint64_t)s_lXtalFrequency*(synth);
  tgt2 = (uint64_t)s_lXtalFrequency*(synth+1);

  synth=((tgt2-tgt)<(tgt-tgt1))?(synth+1):(synth);  

  return synth;
}


/**
* @brief  Computes the channel space register staring from the channel space value in Hz.
*         The channel spacing step is F_Xo/32768.
* @param  lChannelSpace the channel spacing expressed in Hz.
* @retval uint8_t The channel specing register value .
*/
uint8_t S2LPRadioComputeChannelSpacingRegValue(uint32_t lChannelSpace)
{
  return (uint32_t)(((uint64_t)lChannelSpace)<<15)/s_lXtalFrequency;
}


/**
* @brief  Compute the channel spacing register from the channel spacing given in Hz.
*         The channel spacing step is F_Xo/32768.
* @param  cChSpaceRegVal the channel spacing register
* @retval uint32_t The channel specing value in Hz.
*/
uint32_t S2LPRadioComputeChannelSpacing(uint8_t cChSpaceRegVal)
{
  return (uint32_t)(((uint64_t)s_lXtalFrequency*cChSpaceRegVal)>>15);
}


/**
* @brief  Computes the ANALOG_IF and DIGITAL_IF register values staring from a image frequency
*          value in Hz.
* @param  nIF datarate expressed in bps. This parameter ranging between 100 and 500000.
* @param  pcAnaIf pointer to the register of analog IF.
* @param  pcDigIf pointer to the returned of digital IF.
* @retval None.
*/
void S2LPRadioComputeIF(uint32_t nIF, uint8_t* pcAnaIf, uint8_t* pcDigIf)
{
  uint32_t f_dig=s_lXtalFrequency;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  (*pcAnaIf)=(uint8_t)((((uint64_t)nIF)<<13)*3/s_lXtalFrequency-100);
  (*pcDigIf)=(uint8_t)((((uint64_t)nIF)<<13)*3/f_dig-100);
}


/**
* @brief  Returns the charge pump word for a given VCO frequency.
* @param  cp_isel pointer to the charge pump register value.
* @param  pfd_split pointer to the pfd register value.
* @param  lFc channel center frequency expressed in Hz.
*         This parameter can be a value in one of the following ranges:<ul>
*         <li> High_Band: from 779 MHz to 915 MHz </li>
*         <li> Middle Band: from 387 MHz to 470 MHz </li>
* @param  refdiv reference divider value value (it can be only 1 or 2).
* @retval uint8_t Charge pump word.
*/
void S2LPRadioSearchWCP(uint8_t* cp_isel, uint8_t* pfd_split, uint32_t lFc, uint8_t refdiv)
{
  uint32_t vcofreq, lFRef;
  uint8_t BFactor = MIDDLE_BAND_FACTOR;

  s_assert_param(IS_FREQUENCY_BAND(lFc));

  /* Search the operating band */
  if(IS_FREQUENCY_BAND_HIGH(lFc)) {
    BFactor = HIGH_BAND_FACTOR;
  }

  /* Calculates the VCO frequency VCOFreq = lFc*B */
  vcofreq = lFc*BFactor;

  /* Calculated the reference frequency clock */
  lFRef = s_lXtalFrequency/refdiv;

  /* Set the correct charge pump word */
  if(vcofreq>=VCO_CENTER_FREQ) {
    if(lFRef>DIG_DOMAIN_XTAL_THRESH) {
      *cp_isel = 0x02;
      *pfd_split = 0;
    }
    else {
      *cp_isel = 0x01;
      *pfd_split = 1;
    }
  }
  else {
    if(lFRef>DIG_DOMAIN_XTAL_THRESH) {
      *cp_isel = 0x03;
      *pfd_split = 0;
    }
    else {
      *cp_isel = 0x02;
      *pfd_split = 1;
    }
  }

}


/**
* @}
*/


/** @defgroup Radio_Public_Functions                    Radio Public Functions
* @{
*/


/**
* @brief  Initializes the S2LP analog and digital radio part according to the specified
*         parameters in the pxSRadioInitStruct.
* @param  pxSRadioInitStruct pointer to a SRadioInit structure that
*         contains the configuration information for the analog radio part of S2LP.
* @retval Error code: 0=no error, 1=error during calibration of VCO.
*/
uint8_t S2LPRadioInit(SRadioInit* pxSRadioInitStruct)
{
  uint8_t tmpBuffer[6], tmp8, dr_e, fdev_m, fdev_e, bw_m, bw_e;
  uint16_t dr_m;
  SFunctionalState xState;

  s_assert_param(IS_FREQUENCY_BAND(pxSRadioInitStruct->lFrequencyBase));
  s_assert_param(IS_MODULATION(pxSRadioInitStruct->xModulationSelect));
  s_assert_param(IS_DATARATE(pxSRadioInitStruct->lDatarate,s_lXtalFrequency));
  s_assert_param(IS_F_DEV(pxSRadioInitStruct->lFreqDev,s_lXtalFrequency));

  /* Configure the digital, ADC, SMPS reference clock divider */
  xState = S2LPRadioGetDigDiv();

  if(((s_lXtalFrequency<DIG_DOMAIN_XTAL_THRESH) && (xState==S_ENABLE)) || ((s_lXtalFrequency>DIG_DOMAIN_XTAL_THRESH) && (xState==S_DISABLE))) 
  {
    S2LPSpiCommandStrobes(CMD_STANDBY);
    do{
      for(volatile uint8_t i=0; i!=0xFF; i++);
      S2LPRefreshStatus();      // add a timer expiration callback
    }while(g_xStatus.MC_STATE!=MC_STATE_STANDBY);

    xState = (SFunctionalState)!xState;
    S2LPRadioSetDigDiv(xState);

    S2LPSpiCommandStrobes(CMD_READY);

    do{
      for(volatile uint8_t i=0; i!=0xFF; i++);
      S2LPRefreshStatus();      // add a timer expiration callback
    }while(g_xStatus.MC_STATE!=MC_STATE_READY);
  }

  if(xState==S_ENABLE) {
    s_assert_param(IS_CH_BW(pxSRadioInitStruct->lBandwidth,(s_lXtalFrequency>>1)));
  }
  else {
    s_assert_param(IS_CH_BW(pxSRadioInitStruct->lBandwidth,s_lXtalFrequency));
  }

  /* Intermediate Frequency setting */
  S2LPRadioComputeIF(300000, &tmpBuffer[0], &tmpBuffer[1]);
  S2LPSpiWriteRegisters(IF_OFFSET_ANA_ADDR, 2, tmpBuffer);

  /* Calculates the datarate register values */
  S2LPRadioSearchDatarateME(pxSRadioInitStruct->lDatarate, &dr_m, &dr_e);
  tmpBuffer[0] = (uint8_t)(dr_m>>8);
  tmpBuffer[1] = (uint8_t)dr_m;
  tmpBuffer[2] = (uint8_t)(pxSRadioInitStruct->xModulationSelect | dr_e);

  /* Calculates the frequency deviation register values */
  S2LPRadioSearchFreqDevME(pxSRadioInitStruct->lFreqDev, &fdev_m, &fdev_e);
  S2LPSpiReadRegisters(MOD1_ADDR, 1, &tmpBuffer[3]);
  tmpBuffer[3] &= ~FDEV_E_REGMASK;
  tmpBuffer[3] |= fdev_e;
  tmpBuffer[4] = fdev_m;

  /* Calculates the channel filter register values */
  S2LPRadioSearchChannelBwME(pxSRadioInitStruct->lBandwidth, &bw_m, &bw_e);
  tmpBuffer[5] = (bw_m<<4) | bw_e;

  /* Configures the radio registers */
  S2LPSpiWriteRegisters(MOD4_ADDR, 6, tmpBuffer);

  S2LPSpiReadRegisters(PA_POWER0_ADDR, 3, &tmpBuffer[0]);

  /* if OOK is selected enable the PA_FC else enable it */
  if((pxSRadioInitStruct->xModulationSelect)!=MOD_ASK_OOK)
  {
    tmpBuffer[0] &= 0x7F;
    tmpBuffer[1] &= 0xFD;
  }
  else
  {
    tmpBuffer[0] |= 0x80;
    tmpBuffer[1] |= 0x02;
  }


  tmpBuffer[2]&=0xFC;

  /* Bessel filter config */
  if(pxSRadioInitStruct->lDatarate<16000)
  {
    tmpBuffer[2]|=0x00;
  }
  else if(pxSRadioInitStruct->lDatarate<32000)
  {
    tmpBuffer[2]|=0x01;
  }
  else if(pxSRadioInitStruct->lDatarate<62500)
  {
    tmpBuffer[2]|=0x02;
  }
  else
  {
    tmpBuffer[2]|=0x03;
  }

  S2LPSpiWriteRegisters(PA_POWER0_ADDR, 3, &tmpBuffer[0]);

  /* Enable the freeze option of the AFC on the SYNC word */
  S2LPSpiReadRegisters(AFC2_ADDR, 1, &tmp8);
  tmp8 |= AFC_FREEZE_ON_SYNC_REGMASK; S2LPSpiWriteRegisters(AFC2_ADDR, 1, &tmp8);

  return S2LPRadioSetFrequencyBase(pxSRadioInitStruct->lFrequencyBase);

}


/**
* @brief  Returns the S2LP analog and digital radio structure according to the registers value.
* @param  pxSRadioInitStruct pointer to a SRadioInit structure that
*         contains the configuration information for the analog radio part of S2LP.
* @retval None.
*/
void S2LPRadioGetInfo(SRadioInit* pxSRadioInitStruct)
{
  uint8_t tmpBuffer[6];
  uint8_t band, cRefDiv, dr_e, fdev_m, fdev_e, bw_e, bw_m;
  uint16_t dr_m;
  uint32_t tmp32;

  S2LPSpiReadRegisters(SYNT3_ADDR, 4, tmpBuffer);

  /* Reads the operating band masking the Band selected field */
  if(tmpBuffer[0] & BS_REGMASK) {
    band = MIDDLE_BAND_FACTOR;
  }
  else {
    band = HIGH_BAND_FACTOR;
  }

  /* Computes the synth word */
  tmp32 = (((uint32_t)(tmpBuffer[0] & SYNT_27_24_REGMASK))<<24) | (((uint32_t)tmpBuffer[1])<<16) | (((uint32_t)tmpBuffer[2])<<8) | ((uint32_t)tmpBuffer[3]);

  /* Calculates the frequency base */
  cRefDiv = (uint8_t)S2LPRadioGetRefDiv() + 1;

  pxSRadioInitStruct->lFrequencyBase = S2LPRadioComputeFrequencyBase(tmp32, band, cRefDiv);

  /* Reads the radio registers */
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(MOD4_ADDR, 6, tmpBuffer);

  /* Modulation select */
  pxSRadioInitStruct->xModulationSelect = (ModulationSelect)(tmpBuffer[2] & MOD_TYPE_REGMASK);

  /* Reads the frequency deviation for mantissa and exponent */
  fdev_m = tmpBuffer[4];
  fdev_e = tmpBuffer[3] & FDEV_E_REGMASK;

  /* Reads the channel filter register for mantissa and exponent */
  bw_m = (tmpBuffer[5] & CHFLT_M_REGMASK)>>4;
  bw_e = tmpBuffer[5] & CHFLT_E_REGMASK;

  /* Reads the data rate register for mantissa and exponent */
  dr_m = ((uint16_t)tmpBuffer[0]<<8) | ((uint16_t)tmpBuffer[1]);
  dr_e = tmpBuffer[2] & DATARATE_E_REGMASK;

  /* Calculates the datarate */
  pxSRadioInitStruct->lDatarate = S2LPRadioComputeDatarate(dr_m, dr_e);

  /* Calculates the frequency deviation */
  pxSRadioInitStruct->lFreqDev = S2LPRadioComputeFreqDeviation(fdev_m, fdev_e, band, cRefDiv);

  /* Reads the channel filter bandwidth from the look-up table and return it */
  pxSRadioInitStruct->lBandwidth = S2LPRadioComputeChannelFilterBw(bw_m, bw_e);

}


/**
* @brief  Sets the SYNTH registers.
* @param  lSynthWord the synth word to write in the SYNTH[3:0] registers.
* @retval None.
*/
void S2LPRadioSetSynthWord(uint32_t lSynthWord)
{
  uint8_t tmpBuffer[4];
  uint8_t tmp;

  S2LPSpiReadRegisters(SYNT3_ADDR, 1, &tmp);
  tmp &= ~SYNT_27_24_REGMASK;

  /* Build the array for SYNTH registers */
  tmpBuffer[0] = (((uint8_t)(lSynthWord>>24)) & SYNT_27_24_REGMASK) | tmp;
  tmpBuffer[1] = (uint8_t)(lSynthWord>>16);
  tmpBuffer[2] = (uint8_t)(lSynthWord>>8);
  tmpBuffer[3] = (uint8_t)lSynthWord;

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(SYNT3_ADDR, 4, tmpBuffer);
}


/**
* @brief  Returns the synth word.
* @param  None.
* @retval uint32_t Synth word.
*/
uint32_t S2LPRadioGetSynthWord(void)
{
  uint8_t tmpBuffer[4];
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(SYNT3_ADDR, 4, tmpBuffer);
  return ((((uint32_t)(tmpBuffer[0] & SYNT_27_24_REGMASK))<<24) | (((uint32_t)tmpBuffer[1])<<16) | (((uint32_t)tmpBuffer[2])<<8) | ((uint32_t)tmpBuffer[3]));
}


/**
* @brief  Sets the channel number.
* @param  cChannel the channel number.
* @retval None.
*/
void S2LPRadioSetChannel(uint8_t cChannel)
{
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(CHNUM_ADDR, 1, &cChannel);
}


/**
* @brief  Returns the actual channel number.
* @param  None.
* @retval uint8_t Actual channel number.
*/
uint8_t S2LPRadioGetChannel(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(CHNUM_ADDR, 1, &tmp);
  return tmp;
}


/**
* @brief  Set the reference divider value.
* @param  xNewState : S_DISABLE to disable the reference divider and S_ENABLE to enable it.
* @retval None.
*/
void S2LPRadioSetRefDiv(SFunctionalState xNewState)
{
  uint8_t tmp;

  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(XO_RCO_CONF0_ADDR, 1, &tmp);

  if(xNewState == S_ENABLE) {
    tmp |= REFDIV_REGMASK;
  } else {
    tmp &= ~REFDIV_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(XO_RCO_CONF0_ADDR, 1, &tmp);
}


/**
* @brief  To know if the reference deivider is enabled or disabled.
* @param  None.
* @retval SFunctionalState SET (enabled) or RESET (disabled).
*/
SFunctionalState S2LPRadioGetRefDiv(void)
{
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(XO_RCO_CONF0_ADDR, 1, &tmp);

  if(tmp & REFDIV_REGMASK) {
    return S_ENABLE;
  } else {
    return S_DISABLE;
  }
}


/**
* @brief  Set the digital divider .
* @param  xNewState S_DISABLE to disable the digital divider and S_ENABLE to enable it.
* @retval None.
*/
void S2LPRadioSetDigDiv(SFunctionalState xNewState)
{
  uint8_t tmp;

  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(XO_RCO_CONF1_ADDR, 1, &tmp);

  if(xNewState == S_ENABLE) {
    tmp &= ~PD_CLKDIV_REGMASK;
  } else {
    tmp |= PD_CLKDIV_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(XO_RCO_CONF1_ADDR, 1, &tmp);
}


/**
* @brief  Get the digital divider .
* @retval SFunctionalState S_DISABLE to disable the digital divider and S_ENABLE to enable it.
*/
SFunctionalState S2LPRadioGetDigDiv(void)
{
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(XO_RCO_CONF1_ADDR, 1, &tmp);

  if(tmp & PD_CLKDIV_REGMASK) {
    return S_DISABLE;
  } else {
    return S_ENABLE;
  }
}


/**
* @brief  Set the channel space factor in channel space register.
*         The channel spacing step is computed as F_Xo/32768.
* @param  fChannelSpace the channel space expressed in Hz.
* @retval None.
*/
void S2LPRadioSetChannelSpace(uint32_t lChannelSpace)
{
  uint8_t tmp;
  tmp = S2LPRadioComputeChannelSpacingRegValue(lChannelSpace);
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(CH_SPACE_ADDR, 1, &tmp);
}


/**
* @brief  Return the channel space register.
* @param  None.
* @retval uint32_t Channel space. The channel space is: CS = channel_space_factor x XtalFrequency/2^15
*         where channel_space_factor is the CHSPACE register value.
*/
uint32_t S2LPRadioGetChannelSpace(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(CH_SPACE_ADDR, 1, &tmp);
  return S2LPRadioComputeChannelSpacing(tmp);
}


/**
* @brief  Set the Synth word and the Band Select register according to desired base carrier frequency.
*         In this API the Xtal configuration is read out from
*         the corresponding register. The user shall fix it before call this API.
* @param  lFBase the base carrier frequency expressed in Hz as unsigned word.
* @retval Error code: 0=no error, 1=error during calibration of VCO.
*/
uint8_t S2LPRadioSetFrequencyBase(uint32_t lFBase)
{
  uint32_t tmp32;
  uint8_t tmpBuffer[4], cp_isel, bs = 1, pfd_split, tmp, cRefDiv;

  s_assert_param(IS_FREQUENCY_BAND(lFBase));

  tmp32 = S2LPRadioComputeSynthWord(lFBase, ((uint8_t)S2LPRadioGetRefDiv()+1));

  if(IS_FREQUENCY_BAND_HIGH(lFBase)) {
    bs = 0;
  }

  cRefDiv = (uint8_t)S2LPRadioGetRefDiv() + 1;

  /* Search the VCO charge pump word and set the corresponding register */
  S2LPRadioSearchWCP(&cp_isel, &pfd_split, lFBase, cRefDiv);

  S2LPSpiReadRegisters(SYNTH_CONFIG2_ADDR, 1, &tmp);
  tmp &= ~PLL_PFD_SPLIT_EN_REGMASK;
  tmp |= (pfd_split<<2);
  S2LPSpiWriteRegisters(SYNTH_CONFIG2_ADDR, 1, &tmp);

  /* Build the array of registers values for the analog part */
  tmpBuffer[0] = (((uint8_t)(tmp32>>24)) & SYNT_27_24_REGMASK) | cp_isel<<5 | (bs<<4) ;
  tmpBuffer[1] = (uint8_t)(tmp32>>16);
  tmpBuffer[2] = (uint8_t)(tmp32>>8);
  tmpBuffer[3] = (uint8_t)tmp32;

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(SYNT3_ADDR, 4, tmpBuffer);

  return 0;
}


/**
* @brief  Return the base carrier frequency.
* @param  None.
* @retval uint32_t Base carrier frequency expressed in Hz as unsigned word.
*/
uint32_t S2LPRadioGetFrequencyBase(void)
{
  uint8_t tmpBuffer[4];
  uint32_t tmp32;
  uint8_t cRefDiv, band;

  /* Reads the synth registers */
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(SYNT3_ADDR, 4, tmpBuffer);

  /* Reads the operating band masking the Band selected field */
  if(tmpBuffer[0] & BS_REGMASK) {
    band = MIDDLE_BAND_FACTOR;
  }
  else {
    band = HIGH_BAND_FACTOR;
  }
  cRefDiv = (uint8_t)S2LPRadioGetRefDiv() + 1;

  /* Computes the synth word */
  tmp32 = (((uint32_t)(tmpBuffer[0] & SYNT_27_24_REGMASK))<<24) | (((uint32_t)tmpBuffer[1])<<16) | (((uint32_t)tmpBuffer[2])<<8) | ((uint32_t)tmpBuffer[3]);

  /* Calculates the frequency base and return it */
  return S2LPRadioComputeFrequencyBase(tmp32, band, cRefDiv);
}


/**
* @brief  Set the datarate.
* @param  fDatarate datarate expressed in bps. This value shall be in the range
*         [100 500000].
* @retval None.
*/
void S2LPRadioSetDatarate(uint32_t lDatarate)
{
  uint8_t dr_e, tmpBuffer[3];
  uint16_t dr_m;

  if(s_lXtalFrequency>DIG_DOMAIN_XTAL_THRESH) {
    s_assert_param(IS_DATARATE(lDatarate,s_lXtalFrequency/2));
  }
  else {
    s_assert_param(IS_DATARATE(lDatarate,s_lXtalFrequency));
  }

  /* Calculates the datarate mantissa and exponent */
  S2LPRadioSearchDatarateME(lDatarate, &dr_m, &dr_e);

  /* Reads the MOD_O register*/
  S2LPSpiReadRegisters(MOD4_ADDR, 3, tmpBuffer);

  /* Mask the other fields and set the datarate exponent */
  tmpBuffer[0] = (uint8_t)(dr_m>>8);
  tmpBuffer[1] = (uint8_t)dr_m;
  tmpBuffer[2] &= ~DATARATE_E_REGMASK;
  tmpBuffer[2] |= dr_e;

  /* Writes the Datarate registers */
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(MOD4_ADDR, 3, tmpBuffer);

}


/**
* @brief  Return the datarate.
* @param  None.
* @retval uint32_t Settled datarate expressed in bps.
*/
uint32_t S2LPRadioGetDatarate(void)
{
  uint8_t tmpBuffer[3], dr_e;
  uint16_t dr_m;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(MOD4_ADDR, 3, tmpBuffer);
  dr_m = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);
  dr_e = tmpBuffer[2]&DATARATE_E_REGMASK;

  return S2LPRadioComputeDatarate(dr_m, dr_e);
}


/**
* @brief  Set the frequency deviation.
* @param  fFDev frequency deviation expressed in Hz. Be sure that this value
*         is in the correct range [F_Xo*8/2^18, F_Xo*7680/2^18] Hz.
* @retval None.
*/
void S2LPRadioSetFrequencyDev(uint32_t lFDev)
{
  uint8_t uFDevM, uFDevE, tmpBuffer[2];

  s_assert_param(IS_F_DEV(lFDev, s_lXtalFrequency));

  /* Calculates the frequency deviation mantissa and exponent */
  S2LPRadioSearchFreqDevME(lFDev, &uFDevM, &uFDevE);

  S2LPSpiReadRegisters(MOD1_ADDR, 2, tmpBuffer);
  tmpBuffer[0] &= ~FDEV_E_REGMASK;
  tmpBuffer[0] |= uFDevE;
  tmpBuffer[1] = uFDevM;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(MOD1_ADDR, 2, tmpBuffer);
}


/**
* @brief  Return the frequency deviation.
* @param  None.
* @retval uint32_t Frequency deviation value expressed in Hz.
*         This value will be in the range [F_Xo*8/2^18, F_Xo*7680/2^18] Hz.
*/
uint32_t S2LPRadioGetFrequencyDev(void)
{
  uint8_t tmpBuffer[2], uFDevE, tmp, refdiv, cBs = HIGH_BAND_FACTOR;

  refdiv = (uint8_t)S2LPRadioGetRefDiv() + 1;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(SYNT3_ADDR, 1, &tmp);
  if((tmp&BS_REGMASK) == 0) {
    cBs = MIDDLE_BAND_FACTOR;
  }

  /* Reads the frequency deviation register for mantissa and exponent */
  S2LPSpiReadRegisters(MOD1_ADDR, 2, tmpBuffer);
  uFDevE = tmpBuffer[0] & FDEV_E_REGMASK;

  /* Calculates the frequency deviation and return it */
  return S2LPRadioComputeFreqDeviation(tmpBuffer[1], uFDevE, cBs, refdiv);
}


/**
* @brief  Set the channel filter bandwidth.
* @param  lBandwidth channel filter bandwidth expressed in Hz. This parameter shall be in the range [1100 800100]
*         Even if it is possible to pass as parameter any value in the above mentioned range,
*         the API will search the most closer value according to a fixed table of channel
*         bandwidth values (@ref s_vectnBandwidth), as defined in the datasheet. To verify the settled channel bandwidth
*         it is possible to use the S2LPRadioGetChannelBW() API.
* @retval None.
*/
void S2LPRadioSetChannelBW(uint32_t lBandwidth)
{
  uint8_t uBwM, uBwE, tmpBuffer;

  /* Search in the channel filter bandwidth table the exponent value */
  if(s_lXtalFrequency>DIG_DOMAIN_XTAL_THRESH) {
    s_assert_param(IS_CH_BW(lBandwidth,(s_lXtalFrequency>>1)));
  }
  else {
    s_assert_param(IS_CH_BW(lBandwidth,(s_lXtalFrequency)));
  }

  /* Calculates the channel bandwidth mantissa and exponent */
  S2LPRadioSearchChannelBwME(lBandwidth, &uBwM, &uBwE);
  tmpBuffer = (uBwM<<4)|(uBwE);

  /* Writes the Channel filter register */
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(CHFLT_ADDR, 1, &tmpBuffer);

}

/**
* @brief  Return the channel filter bandwidth.
* @param  None.
* @retval uint32_t Channel filter bandwidth expressed in Hz.
*/
uint32_t S2LPRadioGetChannelBW(void)
{
  uint8_t tmpBuffer, uBwM, uBwE;

  /* Reads the channel filter register for mantissa and exponent */
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(CHFLT_ADDR, 1, &tmpBuffer);
  uBwM = (tmpBuffer&0xF0)>>4;
  uBwE = tmpBuffer&0x0F;

  return S2LPRadioComputeChannelFilterBw(uBwM, uBwE);

}


/**
* @brief  Set the modulation type.
* @param  xModulation modulation to set.
*         This parameter shall be of type @ref ModulationSelect .
* @retval None.
*/
void S2LPRadioSetModulation(ModulationSelect xModulation)
{
  uint8_t tmpBuffer;
  s_assert_param(IS_MODULATION(xModulation));

  S2LPSpiReadRegisters(MOD2_ADDR, 1, &tmpBuffer);
  tmpBuffer &= ~MOD_TYPE_REGMASK;
  tmpBuffer |= xModulation;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(MOD2_ADDR, 1, &tmpBuffer);
}


/**
* @brief  Return the modulation type used.
* @param  None.
* @retval ModulationSelect Settled modulation type.
*/
ModulationSelect S2LPRadioGetModulation(void)
{
  uint8_t tmpBuffer;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(MOD2_ADDR, 1, &tmpBuffer);
  return (ModulationSelect)(tmpBuffer&MOD_TYPE_REGMASK);

}


/**
* @brief  Set the XTAL frequency.
* @note this function must be called before any Radio setting.
* @param  uint32_t XTAL frequency.
* @retval void.
*/
void S2LPRadioSetXtalFrequency(uint32_t lXtalFrequency)
{
  s_lXtalFrequency = lXtalFrequency;
}


/**
* @brief  Return the XTAL frequency.
* @param  void.
* @retval uint32_t XTAL frequency.
*/
uint32_t S2LPRadioGetXtalFrequency(void)
{
  return s_lXtalFrequency;
}


/**
* @brief  Set the MAX_DBM bit. This will allow to transmit at the maximum power.
* @param  xNewState enable (S_ENABLE) to enable or disable (S_DISABLE) the max dBm.
* @retval None.
*/
void S2LPRadioSetMaxPALevel(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LPSpiReadRegisters(PA_POWER0_ADDR, 1, &tmp);

  if(xNewState == S_ENABLE) {
    tmp |= PA_MAXDBM_REGMASK;
  } else {
    tmp &= ~PA_MAXDBM_REGMASK;
  }

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PA_POWER0_ADDR, 1, &tmp);
}


/**
* @brief  Sets a specific PA_LEVEL register, with a value given in dBm.
* @param  cIndex PA_LEVEL to set. This parameter shall be in the range [0:7].
* @param  lPowerdBm PA value to write expressed in dBm . Be sure that this values is in the
*         correct range [-PA_LOWER_LIMIT: PA_UPPER_LIMIT] dBm.
* @retval None.
* @note This function makes use of the @ref S2LPRadioGetdBm2Reg fcn to interpolate the
*       power value.
*/
void S2LPRadioSetPALeveldBm(uint8_t cIndex, int32_t lPowerdBm)
{
  uint8_t address, paLevelValue;
  s_assert_param(IS_PA_MAX_INDEX(cIndex));
  s_assert_param(IS_PAPOWER_DBM(lPowerdBm));

  if(lPowerdBm> 14)
  {
    paLevelValue = 1;
  }
  else {
    paLevelValue = (uint8_t)((int32_t)29-2*lPowerdBm);
  }

  address = PA_POWER8_ADDR + 7 - cIndex;

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(address, 1, &paLevelValue);
}


/**
* @brief  Returns a specific PA_LEVEL register, returning a value in dBm.
* @param  cIndex PA_LEVEL to read. This parameter shall be in the range [0:7]
* @retval int32_t Settled power level expressed in dBm. A value
*         higher than PA_UPPER_LIMIT dBm implies no output power
*         (output stage is in high impedance).
* @note This function makes use of the @ref S2LPRadioGetReg2dBm fcn to interpolate the
*       power value.
*/
int32_t S2LPRadioGetPALeveldBm(uint8_t cIndex)
{
  uint8_t address, paLevelValue;
  s_assert_param(IS_PA_MAX_INDEX(cIndex));

  address = PA_POWER8_ADDR + 7 - cIndex;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(address, 1, &paLevelValue);

  return ((int32_t)29-paLevelValue)/2;
}


/**
* @brief  Sets a specific PA_LEVEL_MAX_INDEX.
* @param  cIndex PA_LEVEL_MAX_INDEX to set. This parameter shall be in the range [0:7].
* @retval None
*/
void S2LPRadioSetPALevelMaxIndex(uint8_t cIndex)
{
  uint8_t tmp;
  s_assert_param(IS_PA_MAX_INDEX(cIndex));

  S2LPSpiReadRegisters(PA_POWER0_ADDR, 1, &tmp);
  tmp &= (~PA_LEVEL_MAX_IDX_REGMASK);
  tmp |= cIndex;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PA_POWER0_ADDR, 1, &tmp);
}


/**
* @brief  Returns the actual PA_LEVEL_MAX_INDEX.
* @param  None.
* @retval uint8_t Actual PA_LEVEL_MAX_INDEX. This parameter will be in the range [0:7].
*/
uint8_t S2LPRadioGetPALevelMaxIndex(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(PA_POWER0_ADDR, 1, &tmp);
  return (tmp & PA_LEVEL_MAX_IDX_REGMASK);
}



/**
* @brief  Set the autoramping mode.
*       If enabled:
*       <ul><li>Disable the MAXDBM</li>
*               <li>Disable the PA_RAMPING</li>
*               <li>Enable the FIR_EN</li>
*               </ul>
* @param  xNewState Set the autoramping mode.
* @retval None.
*/
void S2LPRadioSetAutoRampingMode(SFunctionalState xNewState)
{
  uint8_t tmpBuffer[2];

  S2LPSpiReadRegisters(PA_POWER0_ADDR, 2, tmpBuffer);

  if(xNewState == S_ENABLE) {
    tmpBuffer[0] &= ~PA_MAXDBM_REGMASK;
    tmpBuffer[0] &= ~PA_RAMP_EN_REGMASK;
    tmpBuffer[1] |= FIR_EN_REGMASK;
  } else {
    tmpBuffer[1] &= ~FIR_EN_REGMASK;
  }

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PA_POWER0_ADDR, 2, tmpBuffer);
}



/**
* @brief  Set the manual ramping mode.
*       <ul><li>Disable the MAXDBM</li>
*               <li>Enable the PA_RAMPING</li>
*               <li>Disable the FIR_EN</li>
*               </ul>
* @param  xNewState Set the manual ramping mode.
* @retval None.
*/
void S2LPRadioSetManualRampingMode(SFunctionalState xNewState)
{
  uint8_t tmpBuffer[2];

  S2LPSpiReadRegisters(PA_POWER0_ADDR, 2, tmpBuffer);

  if(xNewState == S_ENABLE) {
    tmpBuffer[0] &= ~PA_MAXDBM_REGMASK;
    tmpBuffer[0] |= PA_RAMP_EN_REGMASK;
    tmpBuffer[1] &= ~FIR_EN_REGMASK;
  } else {
    tmpBuffer[0] &= ~PA_RAMP_EN_REGMASK;
  }

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PA_POWER0_ADDR, 2, tmpBuffer);
}


/**
* @brief  Enable the calibration of the VCO frequency and amplitude.
* @param  xAmplitudeCalibration Enables or Disables the calibration of the VCO Amplitude .
* @param  xFrequencyCalibration Enables or Disables the calibration of the VCO Frequency .
* @retval None.
*/
void S2LPRadioCalibrationVco(SFunctionalState xAmplitudeCalibration, SFunctionalState xFrequencyCalibration)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xAmplitudeCalibration));
  s_assert_param(IS_SFUNCTIONAL_STATE(xFrequencyCalibration));

  S2LPSpiReadRegisters(VCO_CONFIG_ADDR, 1, &tmp);

  if(xAmplitudeCalibration == S_ENABLE) {
    tmp |= VCO_CALAMP_EXT_SEL_REGMASK;
  } else {
    tmp &= ~VCO_CALAMP_EXT_SEL_REGMASK;
  }

  if(xFrequencyCalibration == S_ENABLE) {
    tmp |= VCO_CALFREQ_EXT_SEL_REGMASK;
  } else {
    tmp &= ~VCO_CALFREQ_EXT_SEL_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(VCO_CONFIG_ADDR, 1, &tmp);
}

/**
* @brief  Set calibration word of the aplitude in TX.
* @param  value Value of the amplitude calibration word.
* @retval None.
*/
void S2LPRadioSetTxCalibVcoAmpWord(uint8_t value)
{
  value <<= 4;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(VCO_CALIBR_IN2_ADDR, 1, &value);
}

/**
* @brief  Set calibration word of the aplitude in RX.
* @param  value Value of the amplitude calibration word.
* @retval None.
*/
void S2LPRadioSetRxCalibVcoAmpWord(uint8_t value)
{
  value &= VCO_CALAMP_RX_REGMASK;
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(VCO_CALIBR_IN2_ADDR, 1, &value);
}

/**
* @brief  Set calibration word of the frequency in TX.
* @param  value Value of the frequency calibration word.
* @retval None.
*/
void S2LPRadioSetTxCalibVcoFreqWord(uint8_t value)
{
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(VCO_CALIBR_IN1_ADDR, 1, &value);
}

/**
* @brief  Set calibration word of the frequency in RX.
* @param  value Value of the frequency calibration word.
* @retval None.
*/
void S2LPRadioSetRxCalibVcoFreqWord(uint8_t value)
{
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(VCO_CALIBR_IN0_ADDR, 1, &value);
}


/**
* @brief  Initialize the AFC block according to the passed parameters.
* @param  xSAfcInit pointer to the AFC struct that contains the parameters value to be set.
* @retval None.
*/
void S2LPRadioAfcInit(SAfcInit* xSAfcInit)
{
  uint8_t tmpBuffer[3];
  s_assert_param(IS_SFUNCTIONAL_STATE(xSAfcInit->xAfcEnable));
  s_assert_param(IS_SFUNCTIONAL_STATE(xSAfcInit->xAfcFreezeOnSync));
  s_assert_param(IS_AFC_MODE(xSAfcInit->xAfcMode));
  s_assert_param(IS_AFC_GAIN(xSAfcInit->cAfcFastGain));
  s_assert_param(IS_AFC_GAIN(xSAfcInit->cAfcSlowGain));

  /* Reads the PCKT_FLT_OPTIONS ragister */
  S2LPSpiReadRegisters(AFC2_ADDR, 1, &tmpBuffer[0]);

  /* Enables or disables filtering on my address */
  if(xSAfcInit->xAfcEnable == S_ENABLE) {
    tmpBuffer[0] |= AFC_ENABLED_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~AFC_ENABLED_REGMASK;
  }

  /* Enables or disables filtering on multicast address */
  if(xSAfcInit->xAfcFreezeOnSync == S_ENABLE) {
    tmpBuffer[0] |= AFC_FREEZE_ON_SYNC_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~AFC_FREEZE_ON_SYNC_REGMASK;
  }

  /* Enables or disables filtering on broadcast address */
  if(xSAfcInit->xAfcMode == AFC_MODE_LOOP_CLOSED_ON_SLICER) {
    tmpBuffer[0] &= ~AFC_MODE_REGMASK;
  }
  else {
    tmpBuffer[0] |= AFC_MODE_REGMASK;
  }

  tmpBuffer[1] = xSAfcInit->cAfcFastPeriod;
  tmpBuffer[2] = (xSAfcInit->cAfcFastGain<<4) | xSAfcInit->cAfcSlowGain;

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(AFC2_ADDR, 3, tmpBuffer);
}


/**
* @brief  Return the AFC struct that corresponds to the AFC block parameters set on the chip.
* @param  xSAfcInit pointer to the AFC struct that must be filled with the parameters.
* @retval None.
*/
void S2LPRadioGetAfcInfo(SAfcInit* xSAfcInit)
{
  uint8_t tmpBuffer[3];

  S2LPSpiReadRegisters(AFC2_ADDR, 3, tmpBuffer);

  xSAfcInit->xAfcFreezeOnSync = (SFunctionalState)((tmpBuffer[0]&AFC_FREEZE_ON_SYNC_REGMASK)>>7);
  xSAfcInit->xAfcEnable = (SFunctionalState)((tmpBuffer[0]&AFC_ENABLED_REGMASK)>>6);
  xSAfcInit->xAfcMode = (SAfcMode)(tmpBuffer[0]&AFC_MODE_REGMASK);

  xSAfcInit->cAfcFastPeriod = tmpBuffer[1];
  xSAfcInit->cAfcFastGain = (tmpBuffer[2] & AFC_FAST_GAIN_REGMASK)>>4;
  xSAfcInit->cAfcSlowGain = (tmpBuffer[2] & AFC_SLOW_GAIN_REGMASK);

}


/**
* @brief  Set the ISI equalizer.
* @param  xSIsiMode It can be one of the values of @ref SIsiEqu.
* @retval None.
*/
void S2LPRadioSetIsiEqualizationMode(SIsiEqu xSIsiMode)
{
  uint8_t tmp;
  s_assert_param(IS_ISI_EQU(xSIsiMode));

  S2LPSpiReadRegisters(ANT_SELECT_CONF_ADDR, 1, &tmp);
  tmp &= ~EQU_CTRL_REGMASK;
  tmp |= (((uint8_t)xSIsiMode)<<5);
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(ANT_SELECT_CONF_ADDR, 1, &tmp);
}


/**
* @brief  Returnthe ISI equalization.
* @param  None.
* @retval xSIsiMode It can be one of the values of @ref SIsiEqu.
*/
SIsiEqu S2LPRadioGetIsiEqualizationMode(void)
{
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(ANT_SELECT_CONF_ADDR, 1, &tmp);
  return (SIsiEqu)((tmp&EQU_CTRL_REGMASK)>>5);

}


/**
* @brief  Clock recovery configuration
* @param  xSSymClkRecInit pointer to the clock recovery struct to be used to configure the algorithm.
* @retval None.
*/
void S2LPRadioSymClkRecoverInit(SSymClkRecInit* xSSymClkRecInit)
{
  uint8_t tmpBuffer[2] = {0x00, 0x00};
  s_assert_param(IS_CLKREC_MODE(xSSymClkRecInit->xSClkRecMode));
  s_assert_param(IS_SFUNCTIONAL_STATE(xSSymClkRecInit->cClkRec16SymPostFlt));
  s_assert_param(IS_CLKREC_I_GAIN(xSSymClkRecInit->cClkRecIGainFast));
  s_assert_param(IS_CLKREC_I_GAIN(xSSymClkRecInit->cClkRecIGainSlow));
  s_assert_param(IS_CLKREC_P_GAIN(xSSymClkRecInit->cClkRecPGainFast));
  s_assert_param(IS_CLKREC_P_GAIN(xSSymClkRecInit->cClkRecPGainSlow));

  /* Enables or disables filtering on my address */
  if(xSSymClkRecInit->xSClkRecMode == CLKREC_PLL_MODE) {
    tmpBuffer[0] |= CLK_REC_ALGO_SEL_REGMASK;
  }
  tmpBuffer[0] |= xSSymClkRecInit->cClkRecIGainSlow;
  tmpBuffer[0] |= (xSSymClkRecInit->cClkRecPGainSlow<<5);

  /* Enables or disables filtering on my address */
  if(xSSymClkRecInit->cClkRec16SymPostFlt == S_ENABLE) {
    tmpBuffer[1] |= PSTFLT_LEN_REGMASK;
  }
  tmpBuffer[1] |= xSSymClkRecInit->cClkRecIGainFast;
  tmpBuffer[1] |= (xSSymClkRecInit->cClkRecPGainFast<<5);

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(CLOCKREC1_ADDR, 2, tmpBuffer);
}


/**
* @brief  Return the clock recovery configuration
* @param  xSSymClkRecInit pointer to the clock recovery struct to be filled with the clock recovery settings of the chip.
* @retval None.
*/
void S2LPRadioGetSymClkRecoverInfo(SSymClkRecInit* xSSymClkRecInit)
{
  uint8_t tmpBuffer[2];

  S2LPSpiReadRegisters(CLOCKREC1_ADDR, 2, tmpBuffer);

  xSSymClkRecInit->xSClkRecMode = (SClkRecMode)((tmpBuffer[0]&CLK_REC_ALGO_SEL_REGMASK)>>4);
  xSSymClkRecInit->cClkRecIGainSlow = tmpBuffer[0]&CLK_REC_I_GAIN_SLOW_REGMASK;
  xSSymClkRecInit->cClkRecPGainSlow = (tmpBuffer[0]&CLK_REC_P_GAIN_SLOW_REGMASK)>>5;

  xSSymClkRecInit->cClkRec16SymPostFlt = (SFunctionalState)((tmpBuffer[0]&CLK_REC_ALGO_SEL_REGMASK)>>4);
  xSSymClkRecInit->cClkRecIGainFast = tmpBuffer[0]&CLK_REC_I_GAIN_FAST_REGMASK;
  xSSymClkRecInit->cClkRecPGainFast = (tmpBuffer[0]&CLK_REC_P_GAIN_FAST_REGMASK)>>5;

}



/**
* @}
*/


/**
* @}
*/


/**
* @}
*/



/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

