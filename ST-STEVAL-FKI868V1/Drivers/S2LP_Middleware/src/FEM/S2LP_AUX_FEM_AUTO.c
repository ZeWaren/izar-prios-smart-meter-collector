/**
* @file    S2LP_AUX_FEM_AUTO.c
* @author  LowPower RF BU - AMG
* @version 2.0.0
* @date    March, 2020
* @brief   Platform dependent file for external front end module (aka power amplifier)
*          management. This file is only useful for ST eval kits.
*          The evaluation kit supported are: STEVAL-FKI868V2 and STEVAL-FKI915V1
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
#include "S2LP_Middleware_Config.h"
#include "S2LP_CORE_SPI.h"
#include "S2LP_AUX_FEM.h"
#include "S2LP_AUX_UTILS.h"

/** @addtogroup S2LP_MIDDLEWARE_STM32                     S2LP Middleware - STM32
* @{
*/

/** @defgroup S2LP_AUX_FEM_AUTO						S2LP AUX FEM AUTO
  * @brief  S2-LP FEM handling module.
  * This module exports all the main operations to deal with FEM.
  * @details See the file <i>@ref S2LP_FEM.h</i> for more details.
  * @{
*/

/**
* In order to correctly configure the PA there's need to set the PA Level
* in terms of dBM and the MaxIndex for ramping.
* You can define this functions in your own library
* or include ST's S2LP Library that defines this functions in S2LP_Radio.c
*/
__weak void S2LPRadioSetPALeveldBm(uint8_t cIndex, int32_t wPowerdBm){};
__weak void S2LPRadioSetPALevelMaxIndex(uint8_t cIndex){};

#define S2LP_GPIO_DIG_OUT_TX_RX_MODE	0x90	/*!< TX or RX mode indicator (to enable an external range extender) */
#define S2LP_GPIO_DIG_OUT_RX_STATE		0x50	/*!< RX state indication: "1" when demodulator is ON */
#define S2LP_GPIO_DIG_OUT_TX_STATE		0x28	/*!< TX state indication: "1" when S2LP1 is passing in the TX state */

#define S2LP_GPIO_MODE_DIGITAL_INPUT      0x01	/*!< Digital Input on GPIO */
#define S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP	0x02	/*!< Digital Output on GPIO (low current) */
#define S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP	0x03	/*!< Digital Output on GPIO (high current) */

#define PA_CSD_PIN				GPIO_PIN_0
#define PA_CSD_PORT				GPIOA
#define PA_CSD_GPIO_CLK				__GPIOA_CLK_ENABLE

#define PA_CPS_PIN				GPIO_PIN_4
#define PA_CPS_PORT				GPIOA
#define PA_CPS_GPIO_CLK				__GPIOA_CLK_ENABLE

#define PA_CTX_PIN				GPIO_PIN_0
#define PA_CTX_PORT				GPIOB
#define PA_CTX_GPIO_CLK				__GPIOB_CLK_ENABLE

static uint8_t _isBypassEnabled = 0;


/** @defgroup S2LP_AUX_FEM_AUTO_Functions			S2LP AUX FEM Auto exported functions
* @{
*/


/**
* @brief  Front End Module initialization function.
* This function automatically sets the FEM according to the information stored in the device EEPROM.
* This function can be redefined for special needs.
* @param  None
* @retval None
*/
__weak void FEM_Init()
{
  RangeExtType femType = S2LPManagementGetRangeExtender();

  if(femType == RANGE_EXT_NONE)
  {
    /* ... */
  }
  else if(femType == RANGE_EXT_SKYWORKS_SE2435L)
  {
    /* Configuration of S2LP GPIO to control external PA signal CSD, CPS, CTX */
    uint8_t tmp[]={
	(uint8_t)S2LP_GPIO_DIG_OUT_TX_RX_MODE | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP,
	(uint8_t)S2LP_GPIO_DIG_OUT_RX_STATE   | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP,
	(uint8_t)S2LP_GPIO_DIG_OUT_TX_STATE   | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP
    };

    S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);
  }
  else if(femType == RANGE_EXT_SKYWORKS_SKY66420)
  {
    /* Configures MCU GPIOs to drive external PA */
    GPIO_InitTypeDef GPIO_InitStructure;

    /*CSD*/
    PA_CSD_GPIO_CLK();
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = PA_CSD_PIN;
    HAL_GPIO_Init(PA_CSD_PORT, &GPIO_InitStructure);

    /*CPS*/
    PA_CPS_GPIO_CLK();
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = PA_CPS_PIN;
    HAL_GPIO_Init(PA_CPS_PORT, &GPIO_InitStructure);

    /*CTX*/
    PA_CTX_GPIO_CLK();
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = PA_CTX_PIN;
    HAL_GPIO_Init(PA_CTX_PORT, &GPIO_InitStructure);
  }
}

/**
* @brief  Front End Module Operation function.
* This function configures the PA according to the desired status.
* This function can be redefined for special needs.
* @param  operation Specifies the operation to perform.
*         This parameter can be one of following parameters:
*         @arg FEM_SHUTDOWN: Shutdown PA
*         @arg FEM_TX_BYPASS: Bypass the PA in TX
*         @arg FEM_TX: TX mode
*         @arg FEM_RX: RX mode
* @retval None
*/
__weak void FEM_Operation(FEM_OperationType operation)
{
  RangeExtType femType = S2LPManagementGetRangeExtender();

  FEM_Init();

  switch (operation)
  {
  case FEM_SHUTDOWN:
    {
	if(femType == RANGE_EXT_SKYWORKS_SKY66420)
	{
	  /* Puts CSD high to turn on PA */
	  HAL_GPIO_WritePin(PA_CSD_PORT, PA_CSD_PIN, GPIO_PIN_RESET);

	  /* Puts CTX high to go in TX state DON'T CARE */
	  HAL_GPIO_WritePin(PA_CTX_PORT, PA_CTX_PIN, GPIO_PIN_SET);

	  /*No Bypass mode select DON'T CARE  */
	  HAL_GPIO_WritePin(PA_CPS_PORT, PA_CPS_PIN, GPIO_PIN_SET);
	}

	break;
    }
  case FEM_TX_BYPASS:
    {
	if(femType == RANGE_EXT_SKYWORKS_SKY66420)
	{
	  /* Puts CSD high to turn on PA */
	  HAL_GPIO_WritePin(PA_CSD_PORT, PA_CSD_PIN, GPIO_PIN_SET);

	  /* Puts CTX high to go in TX state */
	  HAL_GPIO_WritePin(PA_CTX_PORT, PA_CTX_PIN, GPIO_PIN_SET);

	  /*Bypass mode select  */
	  HAL_GPIO_WritePin(PA_CPS_PORT, PA_CPS_PIN, GPIO_PIN_RESET);
	}

	break;
    }
  case FEM_TX:
    {
	if(femType == RANGE_EXT_SKYWORKS_SKY66420)
	{
	  /* Puts CSD high to turn on PA */
	  HAL_GPIO_WritePin(PA_CSD_PORT, PA_CSD_PIN, GPIO_PIN_SET);

	  /* Puts CTX high to go in TX state */
	  HAL_GPIO_WritePin(PA_CTX_PORT, PA_CTX_PIN, GPIO_PIN_SET);

	  /* Check Bypass mode */
	  if (FEM_GetBypass())
	    HAL_GPIO_WritePin(PA_CPS_PORT, PA_CPS_PIN, GPIO_PIN_RESET);
	  else
	    HAL_GPIO_WritePin(PA_CPS_PORT, PA_CPS_PIN, GPIO_PIN_SET);
	}

	break;
    }
  case FEM_RX:
    {
	if(femType == RANGE_EXT_SKYWORKS_SKY66420)
	{
	  /* Puts CSD high to turn on PA */
	  HAL_GPIO_WritePin(PA_CSD_PORT, PA_CSD_PIN, GPIO_PIN_SET);

	  /* Puts CTX low */
	  HAL_GPIO_WritePin(PA_CTX_PORT, PA_CTX_PIN, GPIO_PIN_RESET);

	  /*No Bypass mode select DON'T CARE  */
	  HAL_GPIO_WritePin(PA_CPS_PORT, PA_CPS_PIN, GPIO_PIN_SET);
	}

	break;
    }
  default:
    /* Error */
    break;
  }
}

/*
* @brief Set the FEM in Bypass (if bypass mode is available)
* This function configures the FEM in Bypass Mode,
* whereas the Bypass mode is available
* @param the Bypass flag (1 means "set Bypass")
* @retval None
*/
void FEM_SetBypass(uint8_t bypass_mode)
{
  _isBypassEnabled = bypass_mode;
}

/*
* @brief Get the FEM Bypass Mode
* This function returns 1 if the FEM is in Bypass Mode
* @param None
* @retval the Bypass state
*/
uint8_t FEM_GetBypass()
{
  return _isBypassEnabled;
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
