/**
 * @file    S2LP_Radio.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP RF Analog and Digital part.
 * @details
 *
 * In order to configure the Radio main parameters, the user can
 * fit <i>SRadioInit</i> structure the and call the <i>S2LPRadioInit()</i>
 * function passing its pointer as an argument.
 *
 * <b>Example:</b>
 * @code
 *
 * SRadioInit radioInit = {
 *     433400000,                 // base frequency
 *     MOD_2FSK,                  // modulation select
 *     38400,                     // datarate
 *     20000,                     // frequency deviation
 *     100000                     // channel filter bandwidth
 * };
 *
 * ...
 *
 * S2LPRadioInit(&radioInit);
 * @endcode
 *
 * Another important parameter for the radio configuration is the
 * transmission power.
 * The user is allowed to configure it using the function <i>S2LPRadioSetPALeveldBm()</i>
 * which sets the PA LEVEL specified by the first argument to the
 * power expressed in dBm by the second parameter.
 *
 * <b>Example:</b>
 * @code
 *
 *  S2LPRadioSetPALeveldBm(0 , 10.0);
 *
 * @endcode
 *
 *
 * @note The effective power that is set can be a little different from the
 * passed argument in dBm because the function performs an approximation.
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
#ifndef __S2LP_RADIO_H
#define __S2LP_RADIO_H


/* Includes ------------------------------------------------------------------*/
#include "S2LP_Regs.h"
#include "S2LP_Types.h"
#include "S2LP_CORE_SPI.h"


#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup S2LP_Libraries
 * @{
 */


/** @defgroup S2LP_Radio          Radio
 * @brief Configuration and management of S2LP RF Analog and Digital part.
 * @details See the file <i>@ref S2LP_Radio.h</i> for more details.
 * @{
 */



/** @defgroup Radio_Exported_Types      Radio Exported Types
 * @{
 */


/**
 * @brief  S2LP Modulation enumeration
 */
typedef enum {
  MOD_NO_MOD       = 0x70, /*!< CW modulation selected */
  MOD_2FSK         = 0x00, /*!< 2-FSK modulation selected */
  MOD_4FSK         = 0x10, /*!< 4-FSK modulation selected */
  MOD_2GFSK_BT05   = 0xA0, /*!< G2FSK modulation selected with BT = 0.5 */
  MOD_2GFSK_BT1    = 0x20, /*!< G2FSK modulation selected with BT = 1 */
  MOD_4GFSK_BT05   = 0xB0, /*!< G4FSK modulation selected with BT = 0.5 */
  MOD_4GFSK_BT1    = 0x30, /*!< G4FSK modulation selected with BT = 1 */
  MOD_ASK_OOK      = 0x50, /*!< OOK modulation selected. */
  MOD_POLAR        = 0x60, /*!< OOK modulation selected. */
} ModulationSelect;


/**
 * @brief  S2LP Radio Init structure definition
 */
typedef struct {
  uint32_t          lFrequencyBase;     /*!< Specifies the base carrier frequency (in Hz),
                                        i.e. the carrier frequency of channel #0.
                                        This parameter can be in one of the following ranges:
                                        High_Band: from 860 MHz to 940 MHz
                                        Middle Band: from 430 MHz to 470 MHz */
  ModulationSelect  xModulationSelect;  /*!< Specifies the modulation @ref ModulationSelect */
  uint32_t          lDatarate;          /*!< Specifies the datarate expressed in bps.
                                        This parameter can be in the range
                                        between 100 bps and 500 kbps */
  uint32_t          lFreqDev;           /*!< Specifies the frequency deviation expressed in Hz.
                                        This parameter can be in the range:
                                        [F_Xo*8/2^18, F_Xo*7680/2^18] Hz */
  uint32_t          lBandwidth;         /*!< Specifies the channel filter bandwidth
                                        expressed in Hz. This parameter can be
                                        in the range between 1100 and 800100 Hz */
} SRadioInit;


/**
 * @brief  S2LP AFC mode enumeration
 */
typedef enum {
  AFC_MODE_LOOP_CLOSED_ON_SLICER = 0,     /* loop closed on slicer */
  AFC_MODE_LOOP_CLOSED_ON_2ND_CONV_STAGE  /* loop closed on 2nd conversion stage */
} SAfcMode;


/**
 * @brief  S2LP AFC Init structure definition
 */
typedef struct {
  SFunctionalState  xAfcEnable;       /*!< AFC enable */
  SFunctionalState  xAfcFreezeOnSync; /*!< Freeze the parameters on SYNC word detect */
  SAfcMode          xAfcMode;         /*!< Specify the AFC mode. @ref SAfcMode */
  uint8_t          cAfcFastPeriod;    /*!< Fast period duration */
  uint8_t          cAfcFastGain;      /*!< Gain used during fast loop */
  uint8_t          cAfcSlowGain;      /*!< Gain used during slow loop */
} SAfcInit;


/**
 * @brief  S2LP ISI Equalization type enumeration
 */
typedef enum {
  ISI_EQUALIZATION_DISABLED = 0,  /* static mode */
  ISI_EQUALIZATION_SINGLE_PASS,   /* dynamic mode 6 dB above threshold*/
  ISI_EQUALIZATION_DUAL_PASS      /* dynamic mode 18 dB above threshold */
} SIsiEqu;


/**
 * @brief  S2LP Clock Recovery mode enumeration
 */
typedef enum {
  CLKREC_DLL_MODE = 0, /* DLL mode */
  CLKREC_PLL_MODE,     /* PLL mode */
} SClkRecMode;


/**
 * @brief  S2LP Clock Recovery Init structure definition
 */
typedef struct {
  SClkRecMode      xSClkRecMode;          /*!< Select the clock recovery mode. @ref SClkRecMode */
  uint8_t          cClkRecPGainSlow;      /*!< Gain P for slow stage. From 0 to 7. */
  uint8_t          cClkRecPGainFast;      /*!< Gain P for fast stage. From 0 to 7. */
  uint8_t          cClkRecIGainSlow;      /*!< Gain I for slow stage. From 0 to 15. */
  uint8_t          cClkRecIGainFast;      /*!< Gain I for fast stage. From 0 to 15. */
  SFunctionalState cClkRec16SymPostFlt;   /*!< Set the post filter to 16 symbols. Default is 8 symbols */
} SSymClkRecInit;


/**
 * @}
 */



/** @defgroup Radio_Exported_Functions                          Radio Exported Functions
 * @{
 */

uint8_t S2LPRadioInit(SRadioInit* pxSRadioInitStruct);
void S2LPRadioGetInfo(SRadioInit* pxSRadioInitStruct);
void S2LPRadioSetSynthWord(uint32_t lSynthWord);
uint32_t S2LPRadioGetSynthWord(void);
void S2LPRadioSetChannel(uint8_t cChannel);
uint8_t S2LPRadioGetChannel(void);
void S2LPRadioSetRefDiv(SFunctionalState xNewState);
SFunctionalState S2LPRadioGetRefDiv(void);
void S2LPRadioSetDigDiv(SFunctionalState xNewState);
SFunctionalState S2LPRadioGetDigDiv(void);
void S2LPRadioSetChannelSpace(uint32_t lChannelSpace);
uint32_t S2LPRadioGetChannelSpace(void);
uint8_t S2LPRadioSetFrequencyBase(uint32_t lFBase);
uint32_t S2LPRadioGetFrequencyBase(void);
void S2LPRadioSetDatarate(uint32_t lDatarate);
uint32_t S2LPRadioGetDatarate(void);
void S2LPRadioSetFrequencyDev(uint32_t lFDev);
uint32_t S2LPRadioGetFrequencyDev(void);
void S2LPRadioSetChannelBW(uint32_t lBandwidth);
uint32_t S2LPRadioGetChannelBW(void);
void S2LPRadioSetModulation(ModulationSelect xModulation);
ModulationSelect S2LPRadioGetModulation(void);
void S2LPRadioSetXtalFrequency(uint32_t lXtalFrequency);
uint32_t S2LPRadioGetXtalFrequency(void);
void S2LPRadioSetIsiEqualizationMode(SIsiEqu xSIsiMode);
SIsiEqu S2LPRadioGetIsiEqualizationMode(void);
void S2LPRadioCalibrationVco(SFunctionalState xAmplitudeCalibration, SFunctionalState xFrequencyCalibration);
void S2LPRadioSetTxCalibVcoAmpWord(uint8_t value);
void S2LPRadioSetRxCalibVcoAmpWord(uint8_t value);
void S2LPRadioSetTxCalibVcoFreqWord(uint8_t value);
void S2LPRadioSetRxCalibVcoFreqWord(uint8_t value);

void S2LPRadioSetMaxPALevel(SFunctionalState xNewState);
void S2LPRadioSetPALeveldBm(uint8_t cIndex, int32_t wPowerdBm);
int32_t S2LPRadioGetPALeveldBm(uint8_t cIndex);
uint8_t S2LPRadioGetPALevelMaxIndex(void);
void S2LPRadioSetPALevelMaxIndex(uint8_t cIndex);
void S2LPRadioSetManualRampingMode(SFunctionalState xNewState);
void S2LPRadioSetAutoRampingMode(SFunctionalState xNewState);

void S2LPRadioAfcInit(SAfcInit* xSAfcInit);
void S2LPRadioGetAfcInfo(SAfcInit* xSAfcInit);
void S2LPRadioSymClkRecoverInit(SSymClkRecInit* xSSymClkRecInit);
void S2LPRadioGetSymClkRecoverInfo(SSymClkRecInit* xSSymClkRecInit);


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
