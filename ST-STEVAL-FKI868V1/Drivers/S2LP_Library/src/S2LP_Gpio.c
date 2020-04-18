/**
 * @file    S2LP_Gpio.c
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   This file provides all the low level API to manage S2-LP GPIO.
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
#include "S2LP_Gpio.h"


/** @addtogroup S2LP_Libraries
 * @{
 */


/** @addtogroup S2LP_Gpio
 * @{
 */




/** @defgroup Gpio_Private_Macros               GPIO Private Macros
 * @{
 */


#define IS_S2LP_GPIO(PIN)  ((PIN == S2LP_GPIO_0) || \
                              (PIN == S2LP_GPIO_1) || \
                              (PIN == S2LP_GPIO_2) || \
                              (PIN == S2LP_GPIO_3))


#define IS_S2LP_GPIO_MODE(MODE)   ((MODE == S2LP_GPIO_MODE_DIGITAL_INPUT) || \
                                   (MODE == S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP) || \
                                   (MODE == S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP))

#define IS_S2LP_GPIO_IO(IO_SEL)        ((IO_SEL == S2LP_GPIO_DIG_OUT_IRQ) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_POR_INV) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WUT_EXP) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_LBD) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TX_DATA) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TX_STATE) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_EMPTY) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_FULL) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_RX_DATA) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_RX_CLOCK) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_RX_STATE) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_NOT_STANDBY_SLEEP) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_STANDBY) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_ANTENNA_SWITCH) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_VALID_PREAMBLE) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_SYNC_DETECTED) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_RSSI_THRESHOLD) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_MCU_CLOCK) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TX_RX_MODE) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_VDD) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_GND) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_SMPS_EXT) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_SLEEP) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_READY) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_LOCK) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WAIT_FOR_LOCK_SIG) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TX_DATA_OOK_SIGNAL) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WAIT_FOR_READY2_SIG) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WAIT_FOR_TIMER_FOR_PM_SET) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WAIT_VCO_CALIBRATION) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_ENABLE_SYNTH_FULL_CIRCUIT) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_TX_COMMAND) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_RX_COMMAND) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_TX_DATA_INPUT_FOR_DIRECTRF) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_DATA_WAKEUP) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_EXT_CLOCK_AT_34_7KHZ))


#define IS_S2LP_GPIO_LEVEL(LEVEL)      ((LEVEL == LOW) || \
                                          (LEVEL == HIGH))

#define IS_S2LP_CLOCK_OUTPUT_XO(RATIO) ((RATIO == XO_RATIO_1) || \
                                           (RATIO == XO_RATIO_1_2) || \
                                           (RATIO == XO_RATIO_1_4) || \
                                           (RATIO == XO_RATIO_1_8) || \
                                           (RATIO == XO_RATIO_1_16) || \
                                           (RATIO == XO_RATIO_1_32) || \
                                           (RATIO == XO_RATIO_1_64) || \
                                           (RATIO == XO_RATIO_1_128) || \
                                           (RATIO == XO_RATIO_1_256))

#define IS_S2LP_CLOCK_OUTPUT_RCO(RATIO) ((RATIO == RCO_RATIO_1) || \
                                           (RATIO == RCO_RATIO_1_128))


#define IS_S2LP_CLOCK_OUTPUT_EXTRA_CYCLES(CYCLES) ((CYCLES == EXTRA_CLOCK_CYCLES_0) || \
                                                      (CYCLES == EXTRA_CLOCK_CYCLES_128) || \
                                                      (CYCLES == EXTRA_CLOCK_CYCLES_256) || \
                                                      (CYCLES == EXTRA_CLOCK_CYCLES_512))


#define IS_S2LP_IRQ_LIST(VALUE)   ((VALUE == RX_DATA_READY) || \
                                     (VALUE == RX_DATA_DISC)  || \
                                     (VALUE == TX_DATA_SENT)  || \
                                     (VALUE == MAX_RE_TX_REACH)  || \
                                     (VALUE == CRC_ERROR)  || \
                                     (VALUE == TX_FIFO_ERROR)  || \
                                     (VALUE == RX_FIFO_ERROR)  || \
                                     (VALUE == TX_FIFO_ALMOST_FULL)  || \
                                     (VALUE == TX_FIFO_ALMOST_EMPTY)  || \
                                     (VALUE == RX_FIFO_ALMOST_FULL)  || \
                                     (VALUE == RX_FIFO_ALMOST_EMPTY)  || \
                                     (VALUE == MAX_BO_CCA_REACH)  || \
                                     (VALUE == VALID_PREAMBLE)  || \
                                     (VALUE == VALID_SYNC)  || \
                                     (VALUE == RSSI_ABOVE_TH)  || \
                                     (VALUE == WKUP_TOUT_LDC)  || \
                                     (VALUE == READY)  || \
                                     (VALUE == STANDBY_DELAYED)  || \
                                     (VALUE == LOW_BATT_LVL)  || \
                                     (VALUE == POR)  || \
                                     (VALUE == BOR)  || \
                                     (VALUE == LOCK)  || \
                                     (VALUE == VCO_CALIBRATION_END)  || \
                                     (VALUE == PA_CALIBRATION_END)  || \
                                     (VALUE == PM_COUNT_EXPIRED)  || \
                                     (VALUE == XO_COUNT_EXPIRED)  || \
                                     (VALUE == TX_START_TIME)  || \
                                     (VALUE == RX_START_TIME)  || \
                                     (VALUE == RX_TIMEOUT)  || \
                                     (VALUE == RX_SNIFF_TIMEOUT)   || \
				                              (VALUE == ALL_IRQ ))



/**
 * @}
 */



/** @defgroup Gpio_Private_Functions            GPIO Private Functions
 * @{
 */

/**
 * @brief  Initialize the S2LP GPIOx according to the specified
 *         parameters in the pxGpioInitStruct.
 * @param  pxGpioInitStruct pointer to a SGpioInit structure that
 *         contains the configuration information for the specified S2LP GPIO.
 * @retval None.
 */
void S2LPGpioInit(SGpioInit* pxGpioInitStruct)
{
  uint8_t tmp;

  s_assert_param(IS_S2LP_GPIO(pxGpioInitStruct->xS2LPGpioPin));
  s_assert_param(IS_S2LP_GPIO_MODE(pxGpioInitStruct->xS2LPGpioMode));
  s_assert_param(IS_S2LP_GPIO_IO(pxGpioInitStruct->xS2LPGpioIO));

  tmp = ((uint8_t)(pxGpioInitStruct->xS2LPGpioMode) | (uint8_t)(pxGpioInitStruct->xS2LPGpioIO));
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(pxGpioInitStruct->xS2LPGpioPin, 1, &tmp);

}



/**
 * @brief  Force S2LP GPIO_x configured as digital output, to VDD or GND.
 * @param  xGpioX Specifies the GPIO to be configured.
 *   This parameter can be one of following parameters:
 *     @arg S2LP_GPIO_0: S2LP GPIO_0
 *     @arg S2LP_GPIO_1: S2LP GPIO_1
 *     @arg S2LP_GPIO_2: S2LP GPIO_2
 *     @arg S2LP_GPIO_3: S2LP GPIO_3
 * @param  xLevel Specifies the level.
 *   This parameter can be: HIGH or LOW.
 * @retval None.
 */
void S2LPGpioSetLevel(S2LPGpioPin xGpioX, OutputLevel xLevel)
{
  uint8_t tmp;

  s_assert_param(IS_S2LP_GPIO(xGpioX));
  s_assert_param(IS_S2LP_GPIO_LEVEL(xLevel));

  /* Sets the value of the S2LP GPIO register according to the specified level */
  if(xLevel == HIGH) {
    tmp = (uint8_t)S2LP_GPIO_DIG_OUT_VDD | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP;
  }
  else {
    tmp = (uint8_t)S2LP_GPIO_DIG_OUT_GND | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP;
  }

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(xGpioX, 1, &tmp);

}


/**
 * @brief  Return output value (VDD or GND) of S2LP GPIO_x, when it is configured as digital output.
 * @param  xGpioX Specifies the GPIO to be read.
 *         This parameter can be one of following parameters:
 *         @arg S2LP_GPIO_0: S2LP GPIO_0
 *         @arg S2LP_GPIO_1: S2LP GPIO_1
 *         @arg S2LP_GPIO_2: S2LP GPIO_2
 *         @arg S2LP_GPIO_3: S2LP GPIO_3
 * @retval OutputLevel Logical level of selected GPIO configured as digital output.
 *         This parameter can be: HIGH or LOW.
 */
OutputLevel S2LPGpioGetLevel(S2LPGpioPin xGpioX)
{
  uint8_t tmp = 0x00;

  s_assert_param(IS_S2LP_GPIO(xGpioX));

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(xGpioX, 1, &tmp);

  /* Mask the GPIO_SELECT field and returns the value according */
  tmp &= GPIO_SELECT_REGMASK;
  if(tmp == S2LP_GPIO_DIG_OUT_VDD) {
    return HIGH;
  }
  else {
    return LOW;
  }

}


/**
 * @brief  Deinit the S2LPIrqs structure setting all the bitfield to 0.
 *         Moreover, it sets the IRQ mask registers to 0x00000000, disabling all IRQs.
 * @param  pxIrqInit pointer to a variable of type @ref S2LPIrqs, in which all the
 *         bitfields will be settled to zero.
 * @retval None.
 */
void S2LPGpioIrqDeInit(S2LPIrqs* pxIrqInit)
{
  uint8_t tmp[4] = {0x00,0x00,0x00,0x00};

  if(pxIrqInit!=NULL) {
    uint32_t tempValue = 0x00000000;

    *pxIrqInit = (*(S2LPIrqs*)&tempValue);
  }

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(IRQ_MASK3_ADDR, 4, tmp);
}


/**
 * @brief  Enable the IRQs according to the user defined pxIrqInit structure.
 * @param  pxIrqInit pointer to a variable of type @ref S2LPIrqs, through which the
 *         user enable specific IRQs. This parameter is a pointer to a S2LPIrqs.
 *         For example suppose to enable only the two IRQ Low Battery Level and Tx Data Sent:
 * @code
 * S2LPIrqs myIrqInit = {0};
 * myIrqInit.IRQ_LOW_BATT_LVL = 1;
 * myIrqInit.IRQ_TX_DATA_SENT = 1;
 * S2LPGpioIrqInit(&myIrqInit);
 * @endcode
 * @retval None.
 */
void S2LPGpioIrqInit(S2LPIrqs* pxIrqInit)
{
  uint8_t tmp[4];
  uint8_t* tmpPoint;

  tmpPoint = (uint8_t*)(pxIrqInit);
  for(char i=0; i<4; i++) {
    tmp[3-i]= tmpPoint[i];
  }

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(IRQ_MASK3_ADDR, 4, tmp);

}


/**
 * @brief  Enable or disables a specific IRQ.
 * @param  xIrq IRQ to enable or disable.
 *         This parameter can be any value of @ref IrqList.
 * @param  xNewState new state for the IRQ.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LPGpioIrqConfig(IrqList xIrq, SFunctionalState xNewState)
{
  uint8_t tmpBuffer[4];
  uint32_t tempValue = 0;

  s_assert_param(IS_S2LP_IRQ_LIST(xIrq));
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(IRQ_MASK3_ADDR, 4, tmpBuffer);

  /* Build the IRQ mask word */
  for(char i=0; i<4; i++) {
    tempValue += ((uint32_t)tmpBuffer[i])<<(8*(3-i));
  }

  /* Rebuild the new mask according to user request */
  if(xNewState == S_DISABLE) {
    tempValue &= (~xIrq);
  }
  else {
    tempValue |= (xIrq);
  }

  /* Build the array of bytes to write in the IRQ_MASK registers */
  for(char j=0; j<4; j++) {
    tmpBuffer[j] = (uint8_t)(tempValue>>(8*(3-j)));
  }

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(IRQ_MASK3_ADDR, 4, tmpBuffer);

}


/**
 * @brief  Fill a pointer to a structure of S2LPIrqs type reading the IRQ_MASK registers.
 * @param  pxIrqMask pointer to a variable of type @ref S2LPIrqs, through which the
 *         user can read which IRQs are enabled. All the bitfields equals to zero correspond
 *         to enabled IRQs, while all the bitfields equals to one correspond to disabled IRQs.
 *         This parameter is a pointer to a S2LPIrqs.
 *         For example suppose that the Power On Reset and RX Data ready are the only enabled IRQs.
 * @code
 * S2LPIrqs myIrqMask;
 * S2LPIrqGetStatus(&myIrqMask);
 * @endcode
 * Then
 * myIrqMask.IRQ_POR and myIrqMask.IRQ_RX_DATA_READY are equal to 0
 * while all the other bitfields are equal to one.
 * @retval None.
 */
void S2LPGpioIrqGetMask(S2LPIrqs* pxIrqMask)
{
  uint8_t tmp[4];
  uint8_t* pIrqPointer = (uint8_t*)pxIrqMask;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(IRQ_MASK3_ADDR, 4, tmp);

  for(char i=0; i<4; i++) {
    *pIrqPointer = tmp[3-i];
    pIrqPointer++;
  }

}


/**
 * @brief  Fill a pointer to a structure of S2LPIrqs type reading the IRQ_STATUS registers.
 * @param  pxIrqStatus pointer to a variable of type @ref S2LPIrqs, through which the
 *         user can read the status of all the IRQs. All the bitfields equals to one correspond
 *         to the raised interrupts. This parameter is a pointer to a S2LPIrqs.
 *         For example suppose that the XO settling timeout is raised as well as the Sync word
 *         detection.
 * @code
 * S2LPIrqs myIrqStatus;
 * S2LPGpioIrqGetStatus(&myIrqStatus);
 * @endcode
 * Then
 * myIrqStatus.IRQ_XO_COUNT_EXPIRED and myIrqStatus.IRQ_VALID_SYNC are equals to 1
 * while all the other bitfields are equals to zero.
 * @retval None.
 */
void S2LPGpioIrqGetStatus(S2LPIrqs* pxIrqStatus)
{
  uint8_t tmp[4];
  uint8_t* pIrqPointer = (uint8_t*)pxIrqStatus;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tmp);



  /* Build the IRQ Status word */
  for(uint8_t i=0; i<4; i++) {
    *pIrqPointer = tmp[3-i];
    pIrqPointer++;
  }
}


/**
 * @brief  Clear the IRQ status registers.
 * @param  None.
 * @retval None.
 */
void S2LPGpioIrqClearStatus(void)
{
  uint8_t tmp[4];
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tmp);

}


/**
 * @brief  Verifie if a specific IRQ has been generated.
 *         The call resets all the IRQ status, so it can't be used in case of multiple raising interrupts.
 * @param  xFlag IRQ flag to be checked.
 *         This parameter can be any value of @ref IrqList.
 * @retval SBool S_TRUE or S_FALSE.
 */
SBool S2LPGpioIrqCheckFlag(IrqList xFlag)
{
  uint8_t tmp[4];
  uint32_t tempValue = 0;
  SBool flag;

  s_assert_param(IS_S2LP_IRQ_LIST(xFlag));

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tmp);
  for(uint8_t i=0; i<4; i++) {
    tempValue += ((uint32_t)tmp[i])<<(8*(3-i));
  }

  if(tempValue & xFlag) {
    flag = S_TRUE;
  }
  else {
    flag = S_FALSE;
  }

  return flag;

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
