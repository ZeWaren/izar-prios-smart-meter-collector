/**
* @file    SDK_EVAL_Com.c
* @author  VMA division - AMS
* @version 3.2.1
* @date    February, 2020
* @brief   This file provides all the low level API to manage SDK UART.
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
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
*/


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Com.h"
#include "SDK_EVAL_Config.h"

#ifdef __ICCARM__
	#include <yfuns.h>
	#include <stdint.h>
	#define stdout _LLIO_STDOUT
#else
#ifdef __CC_ARM
	#include <stdio.h>
	/* Keil debug port defines */
	#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
	#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
	#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
	#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
	#define TRCENA          0x01000000

	struct __FILE { int handle; /* Add whatever needed */ };
	FILE __stdout;
	FILE __stdin;
#endif
#endif


/** @addtogroup SDK_EVAL_NUCLEO
* @{
*/


/** @addtogroup SDK_EVAL_Com
* @{
*/




/** @defgroup SDK_EVAL_Com_Private_Variables                    SDK EVAL Com Private Variables
* @{
*/


#define RECEIVE_QUEUE_SIZE                      NUCLEO_UARTx_RX_QUEUE_SIZE
#define TRANSMIT_QUEUE_SIZE                     NUCLEO_UARTx_TX_QUEUE_SIZE

#define UART_ENTER_CRITICAL()          {NVIC_DisableIRQ(NUCLEO_UARTx_IRQn); HAL_NVIC_DisableIRQ(NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn);} //  __disable_irq()
#define UART_EXIT_CRITICAL()           {NVIC_EnableIRQ(NUCLEO_UARTx_IRQn); HAL_NVIC_EnableIRQ(NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn);} // __enable_irq()

uint8_t rxQ[RECEIVE_QUEUE_SIZE];
uint16_t rxHead = 0;
uint16_t rxTail = 0;
volatile uint16_t rxUsed = 0;
UART_HandleTypeDef huart;
DMA_HandleTypeDef dma_handle_rx,dma_handle_tx;

uint8_t txQ[TRANSMIT_QUEUE_SIZE];
uint16_t txHead = 0;
uint16_t txTail = 0;
volatile uint16_t txUsed = 0,txLastRequest=0;
uint8_t dmaTransmitting = 0;


/**
* @}
*/




/** @defgroup SDK_EVAL_Com_Private_Functions                            SDK EVAL Com Private Functions
* @{
*/

static void prepareDmaTx(void);

/**
* @brief  Configures UART port in DMA mode for both RX and TX.
* @param  None.
* @retval None.
*/
void SdkEvalComInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  NUCLEO_UARTx_GPIO_CLK_ENABLE();
  NUCLEO_UARTx_CLK_ENABLE();
  NUCLEO_UARTx_DMA_CLK_ENABLE();

#ifdef USE_STM32F4XX_NUCLEO
    dma_handle_rx.Instance           = NUCLEO_UARTx_RX_DMA_STREAM;
    dma_handle_rx.Init.Channel       = NUCLEO_UARTx_RX_DMA_CHANNEL;
#else
    dma_handle_rx.Instance           = NUCLEO_UARTx_RX_DMA_CHANNEL;
#endif


#ifdef USE_STM32L0XX_NUCLEO
  dma_handle_rx.Init.Request=NUCLEO_UARTx_RX_DMA_REQUEST;
#endif
  dma_handle_rx.Init.Direction=DMA_PERIPH_TO_MEMORY;
  dma_handle_rx.Init.PeriphInc=DMA_PINC_DISABLE;
  dma_handle_rx.Init.MemInc=DMA_MINC_ENABLE;
  dma_handle_rx.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
  dma_handle_rx.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
  dma_handle_rx.Init.Mode=DMA_CIRCULAR;
  dma_handle_rx.Init.Priority=DMA_PRIORITY_LOW;
  HAL_DMA_Init(&dma_handle_rx);

  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStructure.Pin       = NUCLEO_UARTx_RX_PIN;
  GPIO_InitStructure.Alternate = NUCLEO_UARTx_AF;
  HAL_GPIO_Init(NUCLEO_UARTx_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin       = NUCLEO_UARTx_TX_PIN;
  GPIO_InitStructure.Alternate = NUCLEO_UARTx_AF;
  HAL_GPIO_Init(NUCLEO_UARTx_PORT, &GPIO_InitStructure);

  huart.Init.BaudRate=SDK_EVAL_UART_BAUDRATE;
  huart.Init.WordLength=UART_WORDLENGTH_8B;
  huart.Init.StopBits=UART_STOPBITS_1;
  huart.Init.Parity=UART_PARITY_NONE;
  huart.Init.HwFlowCtl=UART_HWCONTROL_NONE;
  huart.Init.Mode=UART_MODE_TX_RX;
  __HAL_LINKDMA(&huart, hdmarx, dma_handle_rx);

#ifdef USE_STM32F4XX_NUCLEO
    dma_handle_tx.Instance           = NUCLEO_UARTx_TX_DMA_STREAM;
    dma_handle_tx.Init.Channel       = NUCLEO_UARTx_TX_DMA_CHANNEL;
#else
    dma_handle_tx.Instance           = NUCLEO_UARTx_TX_DMA_CHANNEL;
#endif

#ifdef USE_STM32L0XX_NUCLEO
  dma_handle_tx.Init.Request=NUCLEO_UARTx_TX_DMA_REQUEST;
#endif
  dma_handle_tx.Init.Direction=DMA_MEMORY_TO_PERIPH;
  dma_handle_tx.Init.PeriphInc=DMA_PINC_DISABLE;
  dma_handle_tx.Init.MemInc=DMA_MINC_ENABLE;
  dma_handle_tx.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
  dma_handle_tx.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
  dma_handle_tx.Init.Mode=DMA_NORMAL;
  dma_handle_tx.Init.Priority=DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&dma_handle_tx);

  __HAL_LINKDMA(&huart, hdmatx, dma_handle_tx);

  huart.Instance=NUCLEO_UARTx;

  HAL_UART_Init(&huart);

  HAL_NVIC_EnableIRQ(NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn);
  HAL_NVIC_SetPriority(NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn, NUCLEO_UARTx_PRIORITY, 0);

  HAL_UART_Receive_DMA(&huart,rxQ,RECEIVE_QUEUE_SIZE);

  HAL_NVIC_SetPriority(NUCLEO_UARTx_IRQn, NUCLEO_UARTx_PRIORITY-1, 0);
  HAL_NVIC_EnableIRQ(NUCLEO_UARTx_IRQn);
}

void SdkEvalComTriggerTx(void)
{
  prepareDmaTx();
}

void updatetxQ(void)
{
  UART_ENTER_CRITICAL();
  uint16_t  _temp;
#ifdef USE_STM32F4XX_NUCLEO
  uint16_t dmaResidual=huart.hdmatx->Instance->NDTR;
#else
  uint16_t dmaResidual=huart.hdmatx->Instance->CNDTR;
#endif
  txTail=(txTail+(txLastRequest-dmaResidual))%TRANSMIT_QUEUE_SIZE;
  _temp = txLastRequest-dmaResidual;
  txUsed-=_temp;
  txLastRequest=dmaResidual;
  UART_EXIT_CRITICAL();
}

void enqueueTxChars(const unsigned char * buffer, uint16_t size)
{
  while ( size > 0 ) {

    while(txUsed>TRANSMIT_QUEUE_SIZE-size)
    {
      updatetxQ();
    }

    txQ[txHead] = *buffer++;
    UART_ENTER_CRITICAL();
    txUsed++;
    UART_EXIT_CRITICAL();
    txHead = (txHead+1) % TRANSMIT_QUEUE_SIZE;
    size--;
  }
  prepareDmaTx();
}

void SdkEvalComBaudrate(uint32_t baudrate)
{
  huart.Init.BaudRate=baudrate;
  __HAL_UART_DISABLE(&huart);
  HAL_UART_Init(&huart);
  __HAL_UART_ENABLE(&huart);
}

static void prepareDmaTx(void)
{
  if(!dmaTransmitting && txUsed!=0)
  {
    UART_ENTER_CRITICAL();
    dmaTransmitting=1;

    if(txTail+txUsed<TRANSMIT_QUEUE_SIZE)
    {
      txLastRequest=txUsed;
    }
    else
    {
      txLastRequest=(TRANSMIT_QUEUE_SIZE-txTail);
    }

    if(HAL_UART_Transmit_DMA(&huart,&txQ[txTail],txLastRequest)==HAL_OK)
    {

    }

    UART_EXIT_CRITICAL();
  }
}

#ifdef __ICCARM__
  // IAR Standard library hook for serial output
  size_t __write(int handle, const unsigned char * buffer, size_t size)
  {
    /* This template only writes to "standard out" and "standard err",
     * for all other file handles it returns failure. */
    if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR) {
      return _LLIO_ERROR;
    }
    enqueueTxChars(buffer,size);

    return size;
  }
#else
#ifdef __CC_ARM
  /* KEIL fputc implementation template allowing to redirect printf output towards serial port (UART/USB) */
  int fputc(int c, FILE *f)
  {
    updatetxQ();
    const unsigned char c1 = c;
    enqueueTxChars(&c1,1);

    return 1;
  }

  uint8_t __io_getcharNonBlocking(uint8_t *data)
  {
    int c = fgetc(&__stdin);

    if (c == EOF)
      return 0;
    else {
      *data = (uint8_t)c;
      return 1;
    }
  }/* end serialReadByte() */

  ErrorStatus Read_Buffer_Pop(uint8_t *byte)
  {
    int nChars = 0;

    if(huart.hdmarx)
    {
  #ifdef USE_STM32F4XX_NUCLEO
      rxHead=(RECEIVE_QUEUE_SIZE-huart.hdmarx->Instance->NDTR)%RECEIVE_QUEUE_SIZE;
  #else
      rxHead=(RECEIVE_QUEUE_SIZE-huart.hdmarx->Instance->CNDTR)%RECEIVE_QUEUE_SIZE;
  #endif

      if(rxHead>=rxTail)
	rxUsed=rxHead-rxTail;
      else
	rxUsed=RECEIVE_QUEUE_SIZE-rxTail+rxHead;

    }

    for(nChars = 0; (rxUsed>0) && (nChars < 1); nChars++) {
      *byte++ = rxQ[rxTail];
      rxTail = (rxTail+1) % RECEIVE_QUEUE_SIZE;
      rxUsed--;
    }

    return (ErrorStatus)nChars;
  }

  int fgetc (FILE *f) {
    int data = -1;
    Read_Buffer_Pop((uint8_t *)&data);
    return data;
  }

  void __io_flush( void )
  {
    fputc(0, &__stdout);
  }

  void __io_putchar( char c )
  {
    fputc(c, &__stdout);
  }
#endif
#endif


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  uint16_t  _temp;

  txTail=(txTail+txLastRequest)%TRANSMIT_QUEUE_SIZE;

  _temp = txLastRequest;
  txUsed-=_temp;

  txLastRequest=0;
  dmaTransmitting=0;

  prepareDmaTx();
}


void NUCLEO_UARTx_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart);
}


void NUCLEO_UARTx_TX_DMA_CHANNEL_IRQHandler(void)
{
  HAL_DMA_IRQHandler(huart.hdmatx);

#if defined(USE_STM32L0XX_NUCLEO) || defined(USE_STM32F0XX_NUCLEO)
  HAL_DMA_IRQHandler(huart.hdmarx);
#endif
}


#ifdef __ICCARM__
  // IAR Standard library hook for serial input
  size_t __read(int handle, unsigned char * buffer, size_t size)
  {
    int nChars = 0;

    /* This template only reads from "standard in", for all other file
    * handles it returns failure. */
    if (handle != _LLIO_STDIN)
    {
      return _LLIO_ERROR;
    }

    if(huart.hdmarx)
    {
  #ifdef USE_STM32F4XX_NUCLEO
      rxHead=(RECEIVE_QUEUE_SIZE-huart.hdmarx->Instance->NDTR)%RECEIVE_QUEUE_SIZE;
  #else
      rxHead=(RECEIVE_QUEUE_SIZE-huart.hdmarx->Instance->CNDTR)%RECEIVE_QUEUE_SIZE;
  #endif

      if(rxHead>=rxTail)
	rxUsed=rxHead-rxTail;
      else
	rxUsed=RECEIVE_QUEUE_SIZE-rxTail+rxHead;
    }

    for(nChars = 0; (rxUsed>0) && (nChars < size); nChars++) {
      *buffer++ = rxQ[rxTail];
      rxTail = (rxTail+1) % RECEIVE_QUEUE_SIZE;
      rxUsed--;
    }

    return nChars;
  }

  uint8_t __io_getcharNonBlocking(uint8_t *data)
  {
    if (__read(_LLIO_STDIN,data,1))
      return 1;
    else
      return 0;
  }


  void __io_putchar( char c )
  {
    __write(_LLIO_STDOUT, (unsigned char *)&c, 1);
  }


  int __io_getchar()
  {
    unsigned char c;
    __read(_LLIO_STDIN, &c, 1);
    return (int)(c);
  }


  void __io_flush( void )
  {
    __write(_LLIO_STDOUT, NULL, 0);
  }
#endif

void enqueueRxChars(unsigned char * buffer, uint16_t size)
{
  while (( size > 0 ) && (rxUsed < (RECEIVE_QUEUE_SIZE-1))) {
      rxQ[rxHead] = *buffer++;
      rxHead = (rxHead+1) % RECEIVE_QUEUE_SIZE;
      rxUsed++;
      size--;
  }
}

uint8_t append_to_buf(void)
{
  uint8_t c;

  HAL_UART_Receive(&huart, &c, 1, 100);
  rxQ[rxHead] = c;
  rxHead=(rxHead+1)%RECEIVE_QUEUE_SIZE;
  rxUsed++;

  if(c=='\n' || c=='\r')
    return 1;

  return 0;
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


/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
