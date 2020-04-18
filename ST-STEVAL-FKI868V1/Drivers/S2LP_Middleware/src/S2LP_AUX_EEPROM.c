/**
* @file    S2LP_EVAL_EEPROM_DMA.c
* @author  LowPower RF BU - AMG
* @version 1.0.0
* @date    March, 2020
* @brief   S2LP EVAL EEPROM management
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

/* Includes -----------------------------------------------------------------*/
#include "S2LP_Middleware_Config.h"
#include "S2LP_AUX_EEPROM.h"

/** @addtogroup S2LP_MIDDLEWARE_STM32                     S2LP Middleware - STM32
* @{
*/

/** @defgroup S2LP_AUX_EEPROM						S2LP AUX EEPROM
  * @brief  S2-LP EEPROM handling module.
  * This module exports all the main operations to deal with EEPROM.
  * @details See the file <i>@ref S2LP_AUX_EEPROM.h</i> for more details.
  * @{
*/


/**
* @brief  SPI buffers used for DMA application
*/

static volatile uint8_t spi_buffer_rx[128];
static SPI_HandleTypeDef EepromSpiHandle;

#if EEPROM_PRESENT == EEPROM_YES
  static uint8_t s_eeprom  = 0;
#endif

/* To Store the state of the main CS reg */
static uint32_t MainCs_ModerGpioReg;
static uint32_t MainCs_OtyperGpioReg;
static uint32_t MainCs_OspeedrGpioReg;
static uint32_t MainCs_PupdrGpioReg;
static uint32_t MainCs_OdrGpioReg;
static uint32_t MainCs_AfrGpioReg[2];


void EepromSPICSLow()
{
  if (SdkEvalGetDaughterBoardType()==X_NUCLEO_SERIES)
    HAL_GPIO_WritePin(EEPROM_SPI_XNUCLEO_CS_PORT, EEPROM_SPI_XNUCLEO_CS_PIN, GPIO_PIN_RESET);
  else
    HAL_GPIO_WritePin(EEPROM_SPI_CS_PORT, EEPROM_SPI_CS_PIN, GPIO_PIN_RESET);
}

void EepromSPICSHigh()
{
  if (SdkEvalGetDaughterBoardType()==X_NUCLEO_SERIES)
    HAL_GPIO_WritePin(EEPROM_SPI_XNUCLEO_CS_PORT, EEPROM_SPI_XNUCLEO_CS_PIN, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(EEPROM_SPI_CS_PORT, EEPROM_SPI_CS_PIN, GPIO_PIN_SET);
}

void EepromSpiInitialization(void)
{
  /*----------------------- GPIO Mode Configuration --------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable SPI periph and SCLK, MOSI, MISO and CS GPIO clocks */
  EEPROM_SPI_PERIPH_RCC();

  EEPROM_SPI_MOSI_RCC();
  EEPROM_SPI_MISO_RCC();
  EEPROM_SPI_CLK_RCC();
  EEPROM_SPI_CS_RCC();

  /* Configure the AF for MOSI, MISO and SCLK GPIO pins*/
  GPIO_InitStructure.Pin       = EEPROM_SPI_CLK_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = EEPROM_SPI_CLK_AF;
  HAL_GPIO_Init(EEPROM_SPI_CLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = EEPROM_SPI_MISO_PIN;
  GPIO_InitStructure.Alternate = EEPROM_SPI_MISO_AF;
  HAL_GPIO_Init(EEPROM_SPI_MISO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = EEPROM_SPI_MOSI_PIN;
  GPIO_InitStructure.Alternate = EEPROM_SPI_MOSI_AF;
  HAL_GPIO_Init(EEPROM_SPI_MOSI_PORT, &GPIO_InitStructure);


  /*- Freeze the previous status of Main CS (Maybe it is used by the user) -*/

  GPIO_TypeDef *MainCsEepromPort;
  MainCsEepromPort = EEPROM_SPI_CS_PORT;

  /* Store IO Direction in Input Floting Mode */
  MainCs_ModerGpioReg = MainCsEepromPort->MODER;

  /* Store the previous Alternate Function in current IO */
  MainCs_AfrGpioReg[0] = MainCsEepromPort->AFR[0] ;
  MainCs_AfrGpioReg[1] = MainCsEepromPort->AFR[1] ;

  /* Store the previous value for IO Speed */
  MainCs_OspeedrGpioReg = MainCsEepromPort->OSPEEDR;

  /* Store the previous value IO Output Type */
  MainCs_OtyperGpioReg = MainCsEepromPort->OTYPER ;

  /* Store the previous Output Value  */
  MainCs_OdrGpioReg = MainCsEepromPort->ODR ;

  /* Store the previous Pull-up oand Pull-down value */
  MainCs_PupdrGpioReg = MainCsEepromPort->PUPDR;
  /*--------------------------- GPIO Mode Configuration --------------------*/

  //Chip Select (CS) GPIO Conf
  GPIO_InitStructure.Pin = EEPROM_SPI_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(EEPROM_SPI_CS_PORT, &GPIO_InitStructure);


  /* Configure SPI peripheral */
  if(HAL_SPI_GetState(&EepromSpiHandle) == HAL_SPI_STATE_RESET)
  {
    /* Set the SPI parameters */
    EepromSpiHandle.Instance               = EEPROM_SPI_PERIPH_NB;
    EepromSpiHandle.Init.Mode              = SPI_MODE_MASTER;
    EepromSpiHandle.Init.BaudRatePrescaler = S2LPGetSPIPrescaler();

    EepromSpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    EepromSpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    EepromSpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    EepromSpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
    EepromSpiHandle.Init.CRCPolynomial     = 7;
    EepromSpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    EepromSpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    EepromSpiHandle.Init.NSS               = SPI_NSS_SOFT;
    EepromSpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;

    if(HAL_SPI_Init(&EepromSpiHandle) != HAL_OK) {
      return;
    }
    __HAL_SPI_ENABLE(&EepromSpiHandle);
  }

  EepromSPICSHigh();
}

void EepromCsPinInitialization(void)
{
  SdkEvalSetDaughterBoardType(FKI_SERIES);

  GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure SPI pin: CS */
  GPIO_InitStructure.Pin = EEPROM_SPI_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(EEPROM_SPI_CS_PORT, &GPIO_InitStructure);

  /* Enable CS GPIO clock */
  EEPROM_SPI_CS_RCC();

  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh();
}

void EepromCsXnucleoPinInitialization(void)
{
  /*--------------- To Restore the previous statut of CS ----------------*/
  GPIO_TypeDef *MainCsEepromPort;
  MainCsEepromPort = EEPROM_SPI_CS_PORT;

  /* Store IO Direction in Input Floting Mode */
  MainCsEepromPort->MODER = MainCs_ModerGpioReg;

  /* Store the previous Alternate Function in current IO */
  MainCsEepromPort->AFR[0] = MainCs_AfrGpioReg[0];
  MainCsEepromPort->AFR[1]=  MainCs_AfrGpioReg[1];

  /* Store the previous value for IO Speed */
  MainCsEepromPort->OSPEEDR = MainCs_OspeedrGpioReg;

  /* Store the previous value IO Output Type */
  MainCsEepromPort->OTYPER = MainCs_OtyperGpioReg;

  /* Store the previous Output Value  */
  MainCsEepromPort->ODR = MainCs_OdrGpioReg;

  /* Store the previous Pull-up oand Pull-down value */
  MainCsEepromPort->PUPDR = MainCs_PupdrGpioReg;

  /* It affects EepromSPICSHigh/Low functions */
  SdkEvalSetDaughterBoardType(X_NUCLEO_SERIES);

  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure SPI pin: CS */
  GPIO_InitStructure.Pin = EEPROM_SPI_XNUCLEO_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(EEPROM_SPI_XNUCLEO_CS_PORT, &GPIO_InitStructure);

  EEPROM_SPI_XNUCLEO_CS_RCC();

  //Put the SPI chip select high to end the transaction
  EepromSPICSHigh();
}

uint8_t EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t cmd[3];
  uint8_t dummy[255];
  cmd[0] = EEPROM_CMD_READ;

  for(uint8_t k=0; k<2; k++) {
    cmd[k+1] = (uint8_t)(nAddress>>((1-k)*8));
  }

  /* Wait the end of a previous write operation */
  EepromWaitEndWriteOperation();

  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow();

  /* Write the header bytes and read the status bytes */
  HAL_SPI_TransmitReceive(&EepromSpiHandle, cmd, dummy, 3, 1000);

  /* Read the registers according to the number of bytes */
  HAL_SPI_TransmitReceive(&EepromSpiHandle, dummy, pcBuffer, cNbBytes, 1000);

  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh();

  return 0;
}

void EepromWaitEndWriteOperation(void)
{
  uint8_t cmd = EEPROM_CMD_RDSR;
  uint8_t dummy = 0xFF;
  uint8_t status;

  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow();

  /* Send command */
  HAL_SPI_TransmitReceive(&EepromSpiHandle, &cmd, &status, 1, 1000);

  /* Polling on status register */
  do{
    HAL_SPI_TransmitReceive(&EepromSpiHandle, &dummy, &status, 1, 1000);
  }while(status&EEPROM_STATUS_WIP);

  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh();

}

void EepromWriteEnable(void)
{
  uint8_t cmd = EEPROM_CMD_WREN;
  uint8_t status;

  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow();
  /* Send command */
  HAL_SPI_TransmitReceive(&EepromSpiHandle, &cmd, &status, 1, 1000);

  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh();
}

uint8_t EepromWrite(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t cmd = EEPROM_CMD_WRITE, tmp[255];
  uint8_t address[2];

  /* Wait the end of a previous write operation */
  EepromWaitEndWriteOperation();

  /* SET the WREN flag */
  EepromWriteEnable();

  for(uint8_t k=0; k<2; k++) {
    address[k] = (uint8_t)(nAddress>>((1-k)*8));
  }
  //EepromWaitEndWriteOperation();

  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow();

  /* Write the header bytes and read the SPIRIT status bytes */
  HAL_SPI_TransmitReceive(&EepromSpiHandle, &cmd, tmp, 1, 1000);

  HAL_SPI_TransmitReceive(&EepromSpiHandle, address, tmp, 2, 1000);

  HAL_SPI_TransmitReceive(&EepromSpiHandle, pcBuffer, tmp, cNbBytes, 1000);

  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh();

  return 0;
}

uint8_t EepromStatus(void)
{
  uint8_t status[2];
  uint8_t cmd[] = {EEPROM_CMD_RDSR, 0xFF};

  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow();

  /* Send command */
  HAL_SPI_TransmitReceive(&EepromSpiHandle, cmd, status, 2, 1000);

  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh();

  return status[1];

}

uint8_t EepromSetSrwd(void)
{
  uint8_t status[2];
  uint8_t cmd[] = {EEPROM_CMD_WRSR, EEPROM_STATUS_SRWD};

  /* Put the SPI chip select low to start the transaction */
  EepromSPICSLow();

  /* Send command */
  HAL_SPI_TransmitReceive(&EepromSpiHandle, cmd, status, 2, 1000);

  /* Put the SPI chip select high to end the transaction */
  EepromSPICSHigh();


  return status[1];
}

uint8_t S2LPEvalGetHasEeprom(void)
{
#if EEPROM_PRESENT == EEPROM_YES
  return s_eeprom;
#else
  return 0;
#endif
}

void S2LPEvalSetHasEeprom(uint8_t eeprom)
{
#if EEPROM_PRESENT == EEPROM_YES
  s_eeprom = eeprom;
#else
  s_eeprom = 0;
#endif
}


/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
