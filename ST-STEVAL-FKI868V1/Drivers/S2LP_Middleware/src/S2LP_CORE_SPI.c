/**
 * @file    S2LP_CORE_SPI.c
 * @author  LowPower RF BU - AMG
 * @version 2.1.0
 * @date    March, 2020
 * @brief   This file provides all the low level API to manage SPI interface for eval board.
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
 * <h2><center>&copy; COPYRIGHT 2020 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "S2LP_Middleware_Config.h"
#include "S2LP_CORE_SPI.h"

/** @addtogroup S2LP_CORE_STM32                     S2LP CORE - STM32
* @{
*/

/** @defgroup S2LP_CORE_SPI					S2LP CORE SPI
  * @brief  S2-LP SPI handling module.
  * This module exports all the main operations to deal with SPI.
  * @details See the file <i>@ref S2LP_CORE_SPI.h</i> for more details.
  * @{
*/

#define DELAY_CS_SCLK			0x70	/* Delay between CSn falling edge & start of SCLK */

#define HEADER_WRITE_MASK		0x00 /* !< Write mask for header byte */
#define HEADER_READ_MASK		0x01 /* !< Read mask for header byte */
#define HEADER_ADDRESS_MASK		0x00 /* !< Address mask for header byte */
#define HEADER_COMMAND_MASK		0x80 /* !< Command mask for header byte */

#define LINEAR_FIFO_ADDRESS		0xFF  /* !< Linear FIFO address */
#define RSSI_LEVEL_RUN_ADDRESS	0xEF	/* !< RSSI RUN Register address */

#define BUILT_HEADER(add_comm, w_r) (add_comm | w_r)  /*!< macro to build the header byte*/
#define WRITE_HEADER    BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_WRITE_MASK) /*!< macro to build the write header byte*/
#define READ_HEADER     BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_READ_MASK)  /*!< macro to build the read header byte*/
#define COMMAND_HEADER  BUILT_HEADER(HEADER_COMMAND_MASK, HEADER_WRITE_MASK) /*!< macro to build the command header byte*/

static SPI_HandleTypeDef SpiHandle;
static DMA_HandleTypeDef hdma_tx;
static DMA_HandleTypeDef hdma_rx;
static uint8_t waiting_irq=0;

#define WAIT_FOR_SPI_TC()               {while(!__HAL_DMA_GET_FLAG(SpiHandle.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(SpiHandle.hdmarx)));\
                                          do{HAL_DMA_IRQHandler(SpiHandle.hdmarx);\
                                          HAL_DMA_IRQHandler(SpiHandle.hdmatx);\
                                          }while(HAL_SPI_GetState(&SpiHandle) != HAL_SPI_STATE_READY);}


#define SPI_ENTER_CRITICAL()           __disable_irq()
#define SPI_EXIT_CRITICAL()            __enable_irq()


#define SdkEvalSPICSLow()        HAL_GPIO_WritePin(S2LP_SPI_CS_PORT, S2LP_SPI_CS_PIN, GPIO_PIN_RESET)
#define SdkEvalSPICSHigh()       HAL_GPIO_WritePin(S2LP_SPI_CS_PORT, S2LP_SPI_CS_PIN, GPIO_PIN_SET)

static volatile uint8_t spi_in_use=0;

static uint8_t tx_buff[130];
static uint8_t rx_buff[130];

void S2LPSpiInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable SPI periph and SCLK, MOSI, MISO and CS GPIO clocks */
  S2LP_SPI_PERIPH_CLK_ENABLE();

  S2LP_SPI_CS_CLK_ENABLE();
  S2LP_SPI_SCLK_CLK_ENABLE();
  S2LP_SPI_MISO_CLK_ENABLE();
  S2LP_SPI_MOSI_CLK_ENABLE();
  S2LP_SPI_DMA_CLK_ENABLE();

  /* Configure the AF for MOSI, MISO and SCLK GPIO pins*/
  GPIO_InitStructure.Pin       = S2LP_SPI_SCLK_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = S2LP_SPI_SCLK_AF;
  HAL_GPIO_Init(S2LP_SPI_SCLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = S2LP_SPI_MISO_PIN;
  GPIO_InitStructure.Alternate = S2LP_SPI_MISO_AF;
  HAL_GPIO_Init(S2LP_SPI_MISO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = S2LP_SPI_MOSI_PIN;
  GPIO_InitStructure.Alternate = S2LP_SPI_MOSI_AF;
  HAL_GPIO_Init(S2LP_SPI_MOSI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = S2LP_SPI_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(S2LP_SPI_CS_PORT, &GPIO_InitStructure);

  /* Configure SPI peripheral */
  if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
  {
    /* Set the SPI parameters */
    SpiHandle.Instance               = S2LP_SPI_PERIPH_NB;
    SpiHandle.Init.Mode              = SPI_MODE_MASTER;
    SpiHandle.Init.BaudRatePrescaler = S2LPGetSPIPrescaler();

    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;

    /*##-3- Configure the DMA ##################################################*/
    /* Configure the DMA handler for Transmission process */

#ifdef USE_STM32F4XX_NUCLEO
    hdma_tx.Instance                 = S2LP_SPI_TX_DMA_STREAM;
    hdma_tx.Init.Channel             = S2LP_SPI_TX_DMA_CHANNEL;
    hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
    hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
#else
    hdma_tx.Instance                 = S2LP_SPI_TX_DMA_CHANNEL;
#endif

#ifdef USE_STM32L0XX_NUCLEO
    hdma_tx.Init.Request=S2LP_SPI_TX_DMA_REQUEST;
#endif
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&hdma_tx);

    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(&SpiHandle, hdmatx, hdma_tx);

    /* Configure the DMA handler for Transmission process */

#ifdef USE_STM32F4XX_NUCLEO
    hdma_rx.Instance                 = S2LP_SPI_RX_DMA_STREAM;
    hdma_rx.Init.Channel             = S2LP_SPI_RX_DMA_CHANNEL;
    hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
    hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;
#else
    hdma_rx.Instance                 = S2LP_SPI_RX_DMA_CHANNEL;
#endif

#ifdef USE_STM32L0XX_NUCLEO
    hdma_rx.Init.Request=S2LP_SPI_RX_DMA_REQUEST;
#endif
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_NORMAL;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&hdma_rx);

    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(&SpiHandle, hdmarx, hdma_rx);

    if(HAL_SPI_Init(&SpiHandle) != HAL_OK) {
      return;
    }
    __HAL_SPI_ENABLE(&SpiHandle);
  }

  SdkEvalSPICSHigh();
}

void SdkEvalSpiDeinit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable SPI periph and SCLK, MOSI, MISO and CS GPIO clocks */
  S2LP_SPI_PERIPH_CLK_ENABLE();

  S2LP_SPI_CS_CLK_ENABLE();
  S2LP_SPI_SCLK_CLK_ENABLE();
  S2LP_SPI_MISO_CLK_ENABLE();
  S2LP_SPI_MOSI_CLK_ENABLE();

  /* Configure the AF for MOSI, MISO and SCLK GPIO pins*/
  GPIO_InitStructure.Pin       = S2LP_SPI_SCLK_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;

  HAL_GPIO_Init(S2LP_SPI_SCLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = S2LP_SPI_MISO_PIN;

  HAL_GPIO_Init(S2LP_SPI_MISO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = S2LP_SPI_MOSI_PIN;

  HAL_GPIO_Init(S2LP_SPI_MOSI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = S2LP_SPI_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(S2LP_SPI_CS_PORT, &GPIO_InitStructure);

  /* Configure SPI peripheral */
  if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
  {
    /* Set the SPI parameters */
    SpiHandle.Instance               = S2LP_SPI_PERIPH_NB;
    SpiHandle.Init.Mode              = SPI_MODE_MASTER;
    SpiHandle.Init.BaudRatePrescaler = S2LPGetSPIPrescaler();

    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;

    if(HAL_SPI_DeInit(&SpiHandle) != HAL_OK) {
      return;
    }
    __HAL_SPI_DISABLE(&SpiHandle);
  }
}

uint16_t S2LPSpiReadRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=READ_HEADER;
  tx_buff[1]=address;

  uint16_t status;

  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);

  HAL_SPI_TransmitReceive_DMA(&SpiHandle, tx_buff, rx_buff, 2+n_bytes);
  WAIT_FOR_SPI_TC();

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();

  for(uint32_t i=0;i<n_bytes;i++)
  {
    buffer[i]=rx_buff[i+2];
  }

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

uint16_t S2LPSpiWriteRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=WRITE_HEADER;
  tx_buff[1]=address;

  uint16_t status;

  for(uint32_t i=0;i<n_bytes;i++)
  {
    tx_buff[i+2]=buffer[i];
  }

  SPI_ENTER_CRITICAL();

  /* Puts the SPI chip select low to start the transaction */
  SdkEvalSPICSLow();
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);

  HAL_SPI_TransmitReceive_DMA(&SpiHandle, tx_buff, rx_buff, 2+n_bytes);
  WAIT_FOR_SPI_TC();

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  /* Puts the SPI chip select high to end the transaction */
  SdkEvalSPICSHigh();

  SPI_EXIT_CRITICAL();

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

uint16_t S2LPSpiCommandStrobes(uint8_t command)
{
  tx_buff[0]=COMMAND_HEADER;
  tx_buff[1]=command;

  uint16_t status;

  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);


  HAL_SPI_TransmitReceive_DMA(&SpiHandle, tx_buff, rx_buff, 2);
  WAIT_FOR_SPI_TC();

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

uint16_t S2LPSpiReadFifo(uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=READ_HEADER;
  tx_buff[1]=LINEAR_FIFO_ADDRESS;

  uint16_t status;

  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);

  HAL_SPI_TransmitReceive_DMA(&SpiHandle, tx_buff, rx_buff, 2+n_bytes);
  WAIT_FOR_SPI_TC();

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();

  for(uint32_t i=0;i<n_bytes;i++)
  {
    buffer[i]=rx_buff[i+2];
  }
  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

uint16_t S2LPSpiWriteFifo(uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=WRITE_HEADER;
  tx_buff[1]=LINEAR_FIFO_ADDRESS;

  uint16_t status;

  for(uint32_t i=0;i<n_bytes;i++)
  {
    tx_buff[i+2]=buffer[i];
  }

  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);

  HAL_SPI_TransmitReceive_DMA(&SpiHandle, tx_buff, rx_buff, 2+n_bytes);
  WAIT_FOR_SPI_TC();

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

__weak void SdkEvalSpiRawTC(void){}

void S2LPSpiRaw(uint8_t n_bytes, uint8_t* in_buffer, uint8_t* out_buffer, uint8_t can_return_bef_tx)
{
  uint8_t* out_buffer_=out_buffer;

  if(out_buffer==NULL)
      out_buffer_=rx_buff;

  spi_in_use = 1;

  if(can_return_bef_tx)
  {
    waiting_irq=1;
    HAL_NVIC_SetPriority(S2LP_SPI_DMA_TX_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(S2LP_SPI_DMA_TX_IRQn);
    HAL_NVIC_SetPriority(S2LP_SPI_DMA_RX_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(S2LP_SPI_DMA_RX_IRQn);
  }
  else
  {
    waiting_irq=0;
    HAL_NVIC_DisableIRQ(S2LP_SPI_DMA_RX_IRQn);
    HAL_NVIC_DisableIRQ(S2LP_SPI_DMA_TX_IRQn);
    SPI_ENTER_CRITICAL();
  }

  SdkEvalSPICSLow();
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);

  HAL_SPI_TransmitReceive_DMA(&SpiHandle, in_buffer, out_buffer_, n_bytes);

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);

  if(!can_return_bef_tx)
  {
    WAIT_FOR_SPI_TC();

    SdkEvalSPICSHigh();
    SPI_EXIT_CRITICAL();
  }

  spi_in_use = 0;
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if(waiting_irq)
  {
    SdkEvalSPICSHigh();
    waiting_irq=0;
    SdkEvalSpiRawTC();
  }
}

#if defined(USE_STM32L0xx_NUCLEO) || defined(USE_STM32F0xx_NUCLEO)
  void NUCLEO_SPI_DMA_TX_IRQHandler(void)
  {
    HAL_DMA_IRQHandler(SpiHandle.hdmatx);
    HAL_DMA_IRQHandler(SpiHandle.hdmarx);
  }
#else
  void NUCLEO_SPI_DMA_TX_IRQHandler(void)
  {
    HAL_DMA_IRQHandler(SpiHandle.hdmatx);
  }

  void NUCLEO_SPI_DMA_RX_IRQHandler(void)
  {
    HAL_DMA_IRQHandler(SpiHandle.hdmarx);
  }
#endif

void S2LPSetSpiInUse(uint8_t state)
{
  spi_in_use = state;
}

uint8_t S2LPGetSpiInUse()
{
  return spi_in_use;
}

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
