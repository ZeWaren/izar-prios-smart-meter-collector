/**
 * @file    S2LP_General.c
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP General functionalities.
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
#include "S2LP_General.h"


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_General
 * @{
 */


/**
 * @defgroup General_Private_TypesDefinitions   General Private Types Definitions
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup General_Private_Defines            General Private Defines
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup General_Private_Macros             General Private Macros
 * @{
 */

#define IS_MODE_EXT(MODE)   (MODE == MODE_EXT_XO || \
                             MODE == MODE_EXT_XIN)
/**
 *@}
 */


/**
 * @defgroup General_Private_Variables          General Private Variables
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup General_Private_FunctionPrototypes         General Private Function Prototypes
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup General_Private_Functions                          General Private Functions
 * @{
 */


/**
 * @brief  Set External Reference.
 * @param  xExtMode new state for the external reference.
 *         This parameter can be: MODE_EXT_XO or MODE_EXT_XIN.
 * @retval None.
 */
void S2LPGeneralSetExtRef(ModeExtRef xExtMode)
{
  uint8_t tmp;
  s_assert_param(IS_MODE_EXT(xExtMode));

  S2LPSpiReadRegisters(XO_RCO_CONF0_ADDR, 1, &tmp);
  if(xExtMode == MODE_EXT_XO) {
    tmp &= ~EXT_REF_REGMASK;
  }
  else {
    tmp |= EXT_REF_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(XO_RCO_CONF0_ADDR, 1, &tmp);

}


/**
 * @brief  Return External Reference.
 * @param  None.
 * @retval ModeExtRef Settled external reference.
 *         This parameter can be: MODE_EXT_XO or MODE_EXT_XIN.
 */
ModeExtRef S2LPGeneralGetExtRef(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(XO_RCO_CONF0_ADDR, 1, &tmp);
  return (ModeExtRef)(tmp & EXT_REF_REGMASK);
}


/**
 * @brief  Return device part number.
 * @param  None.
 * @retval Device part number.
 */
uint8_t S2LPGeneralGetDevicePN(void)
{
  uint8_t tmp;
  *(uint8_t*)&g_xStatus = S2LPSpiReadRegisters(DEVICE_INFO1_ADDR, 1, &tmp);
  return tmp;
}

/**
 * @brief  Return S2LP version.
 * @param  None.
 * @retval S2LP version.
 */
uint8_t S2LPGeneralGetVersion(void)
{
  uint8_t tmp;
  S2LPSpiReadRegisters(DEVICE_INFO0_ADDR, 1, &tmp);
  return tmp;
}


/**
* @brief  Disable or enable the internal SMPS.
* @param  xNewState if this value is S_DISABLE the external SMPS is enabled and a vlotage must be provided from outside.
*               In this case the internal SMPS will be disabled.
* @retval None.
*/
void S2LPRadioSetExternalSmpsMode(SFunctionalState xNewState)
{
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PM_CONF4_ADDR, 1, &tmp);

  if(xNewState == S_ENABLE) {
    tmp |= EXT_SMPS_REGMASK;
  } else {
    tmp &= ~EXT_SMPS_REGMASK;
  }
  *(uint8_t*)&g_xStatus = S2LPSpiWriteRegisters(PM_CONF4_ADDR, 1, &tmp);
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
