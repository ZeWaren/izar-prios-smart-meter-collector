/**
 * @file    S2LP_Fifo.c
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP Fifo.
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
#include "S2LP_Fifo.h"


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_Fifo
 * @{
 */


/**
 * @defgroup Fifo_Private_TypesDefinitions         FIFO Private Types Definitions
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Fifo_Private_Defines                  FIFO Private Defines
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Fifo_Private_Macros                   FIFO Private Macros
 * @{
 */
#define IS_FIFO_THR(VAL)  (VAL<=128)

/**
 *@}
 */


/**
 * @defgroup Fifo_Private_Variables                FIFO Private Variables
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Fifo_Private_FunctionPrototypes       FIFO Private Function Prototypes
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Fifo_Private_Functions                FIFO Private Functions
 * @{
 */

/**
 * @brief  Return the number of elements in the Rx FIFO.
 * @param  None.
 * @retval uint8_t Number of elements in the Rx FIFO.
 */
uint8_t S2LPFifoReadNumberBytesRxFifo(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(RX_FIFO_STATUS_ADDR, 1, &tmp);
  return tmp;

}


/**
 * @brief  Return the number of elements in the Tx FIFO.
 * @param  None.
 * @retval uint8_t Number of elements in the Tx FIFO.
 */
uint8_t S2LPFifoReadNumberBytesTxFifo(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(TX_FIFO_STATUS_ADDR, 1, &tmp);
  return tmp;

}


/**
 * @brief  Set the almost full threshold for the Rx FIFO. When the number of elements in RX FIFO reaches this value an interrupt can be generated to the MCU.
 * @note   The almost full threshold is encountered from the top of the FIFO. For example, if it is set to 7 the almost
 *         full FIFO irq will be raised when the number of elements is equals to 128-7 = 121.
 * @param  cThrRxFifo almost full threshold.
 * 	   This parameter is an uint8_t.
 * @retval None.
 */
void S2LPFifoSetAlmostFullThresholdRx(uint8_t cThrRxFifo)
{
  uint8_t tmp;

  s_assert_param(IS_FIFO_THR(cThrRxFifo));

  S2LPSpiReadRegisters(FIFO_CONFIG3_ADDR, 1, &tmp);

  tmp &= ~RX_AFTHR_REGMASK;
  tmp |= cThrRxFifo;

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(FIFO_CONFIG3_ADDR, 1, &tmp);

}


/**
 * @brief  Return the almost full threshold for RX FIFO.
 * @note   The almost full threshold is encountered from the top of the FIFO. For example, if it is 7 the almost
 *         full FIFO irq will be raised when the number of elements is equals to 128-7 = 121.
 * @param  None.
 * @retval uint8_t Almost full threshold for Rx FIFO.
 */
uint8_t S2LPFifoGetAlmostFullThresholdRx(void)
{
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(FIFO_CONFIG3_ADDR, 1, &tmp);

  return (tmp & RX_AFTHR_REGMASK);

}


/**
 * @brief  Set the almost empty threshold for the Rx FIFO. When the number of elements in RX FIFO reaches this value an interrupt can be generated to the MCU.
 * @param  cThrRxFifo almost empty threshold.
 * 	   This parameter is an uint8_t.
 * @retval None.
 */
void S2LPFifoSetAlmostEmptyThresholdRx(uint8_t cThrRxFifo)
{
  uint8_t tmp;

  s_assert_param(IS_FIFO_THR(cThrRxFifo));

  S2LPSpiReadRegisters(FIFO_CONFIG2_ADDR, 1, &tmp);

  tmp &= ~RX_AETHR_REGMASK;
  tmp |= cThrRxFifo;

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(FIFO_CONFIG2_ADDR, 1, &tmp);

}


/**
 * @brief  Return the almost empty threshold for Rx FIFO.
 * @param  None.
 * @retval uint8_t Almost empty threshold for Rx FIFO.
 */
uint8_t S2LPFifoGetAlmostEmptyThresholdRx(void)
{
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(FIFO_CONFIG2_ADDR, 1, &tmp);

  return (tmp & RX_AETHR_REGMASK);

}


/**
 * @brief  Set the almost full threshold for the Tx FIFO. When the number of elements in TX FIFO reaches this value an interrupt can be generated to the MCU.
 * @note   The almost full threshold is encountered from the top of the FIFO. For example, if it is set to 7 the almost
 *         full FIFO irq will be raised when the number of elements is equals to 128-7 = 121.
 * @param  cThrTxFifo almost full threshold.
 * 	   This parameter is an uint8_t.
 * @retval None.
 */
void S2LPFifoSetAlmostFullThresholdTx(uint8_t cThrTxFifo)
{
  uint8_t tmp;

  s_assert_param(IS_FIFO_THR(cThrTxFifo));

  S2LPSpiReadRegisters(FIFO_CONFIG1_ADDR, 1, &tmp);

  tmp &= ~TX_AFTHR_REGMASK;
  tmp |= cThrTxFifo;

  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(FIFO_CONFIG1_ADDR, 1, &tmp);

}


/**
 * @brief  Return the almost full threshold for Tx FIFO.
 * @note   The almost full threshold is encountered from the top of the FIFO. For example, if it is set to 7 the almost
 *         full FIFO irq will be raised when the number of elements is equals to 128-7 = 121.
 * @param  None.
 * @retval uint8_t Almost full threshold for Tx FIFO.
 */
uint8_t S2LPFifoGetAlmostFullThresholdTx(void)
{
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(FIFO_CONFIG1_ADDR, 1, &tmp);

  return (tmp & TX_AFTHR_REGMASK);

}


/**
 * @brief  Set the almost empty threshold for the Tx FIFO. When the number of elements in Tx FIFO reaches this value an interrupt can can be generated to the MCU.
 * @param  cThrTxFifo: almost empty threshold.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void S2LPFifoSetAlmostEmptyThresholdTx(uint8_t cThrTxFifo)
{
  uint8_t tmp;

  s_assert_param(IS_FIFO_THR(cThrTxFifo));

  S2LPSpiReadRegisters(FIFO_CONFIG0_ADDR, 1, &tmp);

  /* Build the register value */
  tmp &= ~TX_AETHR_REGMASK;
  tmp |= cThrTxFifo;

  /* Writes the Almost Empty threshold for Tx in the corresponding register */
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(FIFO_CONFIG0_ADDR, 1, &tmp);

}


/**
 * @brief  Return the almost empty threshold for Tx FIFO.
 * @param  None.
 * @retval uint8_t Almost empty threshold for Tx FIFO.
 */
uint8_t S2LPFifoGetAlmostEmptyThresholdTx(void)
{
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(FIFO_CONFIG0_ADDR, 1, &tmp);

  return (tmp & TX_AETHR_REGMASK);

}


/**
 * @brief  Enable the Mux for the Rx FIFO IRQ. To be enabled when RX FIFO THRESHOLD is used.
 * @param  xNewState.
 * @retval None.
 */
void S2LPFifoMuxRxFifoIrqEnable(SFunctionalState xNewState)
{
  /* default TX selection*/
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PROTOCOL2_ADDR, 1, &tmp);

  if(xNewState == S_ENABLE) {
    tmp |= FIFO_GPIO_OUT_MUX_SEL_REGMASK;
  } else {
    tmp &= ~FIFO_GPIO_OUT_MUX_SEL_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PROTOCOL2_ADDR, 1, &tmp);
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
