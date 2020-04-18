/**
 * @file    S2LP_Gpio.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief   Configuration and management of S2-LP GPIO.
 * @details
 *
 * This module can be used to configure the S2-LP GPIO pins to perform
 * specific functions.
 * The structure <i>@ref gpioIRQ</i> can be used to specify these features for
 * one of the four S2-LP Gpio pin.
 * The following example shows how to configure a pin (GPIO 3) to be used as an IRQ source
 * for a microcontroller using the <i>@ref S2LPGpioInit()</i> function.
 *
 * <b>Example:</b>
 * @code
 *
 * SGpioInit gpioIRQ={
 *   S2LP_GPIO_3,
 *   S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP,
 *   S2LP_GPIO_DIG_OUT_IRQ
 * };
 *
 * ...
 *
 * S2LPGpioInit(&gpioIRQ);
 *
 * @endcode
 *
 * @note Please read the functions documentation for the other GPIO features.
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
#ifndef __S2LP_GPIO_H
#define __S2LP_GPIO_H


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


/** @defgroup S2LP_Gpio       GPIO
 * @brief Configuration and management of S2LP GPIO.
 * @details See the file <i>@ref S2LP_Gpio.h</i> for more details.
 * @{
 */



/** @defgroup Gpio_Exported_Types       GPIO Exported Types
 * @{
 */

/**
 * @brief  S2LP GPIO pin enumeration.
 */
typedef enum {
  S2LP_GPIO_0  = GPIO0_CONF_ADDR, /*!< GPIO_0 selected */
  S2LP_GPIO_1  = GPIO1_CONF_ADDR, /*!< GPIO_1 selected */
  S2LP_GPIO_2  = GPIO2_CONF_ADDR, /*!< GPIO_2 selected */
  S2LP_GPIO_3  = GPIO3_CONF_ADDR  /*!< GPIO_3 selected */
} S2LPGpioPin;


/**
 * @brief  S2LP GPIO mode enumeration.
 */
typedef enum {
  S2LP_GPIO_MODE_DIGITAL_INPUT      = 0x01, /*!< Digital Input on GPIO */
  S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP ,        /*!< Digital Output on GPIO (low current) */
  S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP          /*!< Digital Output on GPIO (high current) */
} S2LPGpioMode;



/**
 * @brief  S2LP I/O selection enumeration.
 */
typedef enum {
  S2LP_GPIO_DIG_OUT_IRQ                               = 0x00, /*!< nIRQ (Interrupt Request, active low) , default configuration after POR */
  S2LP_GPIO_DIG_OUT_POR_INV                           = 0x08, /*!< POR inverted (active low) */
  S2LP_GPIO_DIG_OUT_WUT_EXP                           = 0x10, /*!< Wake-Up Timer expiration: "1" when WUT has expired */
  S2LP_GPIO_DIG_OUT_LBD                               = 0x18, /*!< Low battery detection: "1" when battery is below threshold setting */
  S2LP_GPIO_DIG_OUT_TX_DATA                           = 0x20, /*!< TX data internal clock output (TX data are sampled on the rising edge of it) */
  S2LP_GPIO_DIG_OUT_TX_STATE                          = 0x28, /*!< TX state indication: "1" when S2LP1 is passing in the TX state */
  S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_EMPTY            = 0x30, /*!< TX/RX FIFO Almost Empty Flag */
  S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_FULL             = 0x38, /*!< TX/RX FIFO Almost Full Flag */
  S2LP_GPIO_DIG_OUT_RX_DATA                           = 0x40, /*!< RX data output */
  S2LP_GPIO_DIG_OUT_RX_CLOCK                          = 0x48, /*!< RX clock output (recovered from received data) */
  S2LP_GPIO_DIG_OUT_RX_STATE                          = 0x50, /*!< RX state indication: "1" when demodulator is ON */
  S2LP_GPIO_DIG_OUT_NOT_STANDBY_SLEEP                 = 0x58, /*!< VDD when the device is not in SLEEP or STANDBY */
  S2LP_GPIO_DIG_OUT_STANDBY                           = 0x60, /*!< VDD when device is in STANDBY */
  S2LP_GPIO_DIG_OUT_ANTENNA_SWITCH                    = 0x68, /*!< Antenna switch used for antenna diversity  */
  S2LP_GPIO_DIG_OUT_VALID_PREAMBLE                    = 0x70, /*!< Valid Preamble Detected Flag */
  S2LP_GPIO_DIG_OUT_SYNC_DETECTED                     = 0x78, /*!< Sync WordSync Word Detected Flag */
  S2LP_GPIO_DIG_OUT_RSSI_THRESHOLD                    = 0x80, /*!< RSSI above threshold */
  S2LP_GPIO_DIG_OUT_MCU_CLOCK                         = 0x88, /*!< MCU Clock */
  S2LP_GPIO_DIG_OUT_TX_RX_MODE                        = 0x90, /*!< TX or RX mode indicator (to enable an external range extender) */
  S2LP_GPIO_DIG_OUT_VDD                               = 0x98, /*!< VDD (to emulate an additional GPIO of the MCU, programmable by SPI) */
  S2LP_GPIO_DIG_OUT_GND                               = 0xA0, /*!< GND (to emulate an additional GPIO of the MCU, programmable by SPI) */
  S2LP_GPIO_DIG_OUT_SMPS_EXT                          = 0xA8, /*!< External SMPS enable signal (active high) */
  S2LP_GPIO_DIG_OUT_SLEEP                             = 0xB0, /*!< Device in SLEEP (active high) */
  S2LP_GPIO_DIG_OUT_READY                             = 0xB8, /*!< Device in READY (active high) */
  S2LP_GPIO_DIG_OUT_LOCK                              = 0xC0, /*!< Device in LOCK (active high) */
  S2LP_GPIO_DIG_OUT_WAIT_FOR_LOCK_SIG                 = 0xC8, /*!< Device waiting for LOCK (active high) */
  S2LP_GPIO_DIG_OUT_TX_DATA_OOK_SIGNAL                = 0xD0,
  S2LP_GPIO_DIG_OUT_WAIT_FOR_READY2_SIG               = 0xD8,
  S2LP_GPIO_DIG_OUT_WAIT_FOR_TIMER_FOR_PM_SET         = 0xE0,
  S2LP_GPIO_DIG_OUT_WAIT_VCO_CALIBRATION              = 0xE8,
  S2LP_GPIO_DIG_OUT_ENABLE_SYNTH_FULL_CIRCUIT         = 0xF0,

  S2LP_GPIO_DIG_IN_TX_COMMAND                         = 0x00,
  S2LP_GPIO_DIG_IN_RX_COMMAND                         = 0x08,
  S2LP_GPIO_DIG_IN_TX_DATA_INPUT_FOR_DIRECTRF         = 0x10,
  S2LP_GPIO_DIG_IN_DATA_WAKEUP                        = 0x18,
  S2LP_GPIO_DIG_IN_EXT_CLOCK_AT_34_7KHZ               = 0x20
} S2LPGpioIO;


/**
 * @brief  S2LP OutputLevel enumeration.
 */
typedef enum {
  LOW = 0,
  HIGH = !LOW
} OutputLevel;


/**
 * @brief  S2LP GPIO Init structure definition.
 */
typedef struct {
  S2LPGpioPin xS2LPGpioPin;    /*!< Select the GPIO pins to be configured. @ref S2LPGpioPin */
  S2LPGpioMode xS2LPGpioMode;  /*!< Set the pin operating mode. @ref S2LPGpioMode */
  S2LPGpioIO xS2LPGpioIO;      /*!< Set the I/O selection for the pin. @ref S2LPGpioIO */
} SGpioInit;


/**
 * @brief  S2LP clock output XO prescaler enumeration.
 */
typedef enum {
 XO_RATIO_1     = 0x00, /*!< XO Clock signal available on the GPIO divided by 1     */
 XO_RATIO_1_2   = 0x02, /*!< XO Clock signal available on the GPIO divided by 1/2   */
 XO_RATIO_1_4	= 0x04, /*!< XO Clock signal available on the GPIO divided by 1/4   */
 XO_RATIO_1_8	= 0x06, /*!< XO Clock signal available on the GPIO divided by 1/8   */
 XO_RATIO_1_16	= 0x08, /*!< XO Clock signal available on the GPIO divided by 1/16  */
 XO_RATIO_1_32	= 0x0A, /*!< XO Clock signal available on the GPIO divided by 1/32  */
 XO_RATIO_1_64	= 0x0C, /*!< XO Clock signal available on the GPIO divided by 1/64  */
 XO_RATIO_1_128	= 0x0E, /*!< XO Clock signal available on the GPIO divided by 1/128 */
 XO_RATIO_1_256	= 0x10  /*!< XO Clock signal available on the GPIO divided by 1/256 */
} ClockOutputXOPrescaler;


/**
 * @brief  S2LP Clock Output RCO prescaler enumeration.
 */

typedef enum {
 RCO_RATIO_1        = 0x00,   /*!< RCO Clock signal available on the GPIO divided by 1     */
 RCO_RATIO_1_128	            /*!< RCO Clock signal available on the GPIO divided by 1/128   */
} ClockOutputRCOPrescaler;


/**
 * @brief  S2LP ExtraClockCycles enumeration.
 */
typedef enum {
EXTRA_CLOCK_CYCLES_0	  = 0x00, /*!< 0   extra clock cycles provided to the MCU before switching to STANDBY state */
EXTRA_CLOCK_CYCLES_128	= 0x20, /*!< 128  extra clock cycles provided to the MCU before switching to STANDBY state */
EXTRA_CLOCK_CYCLES_256	= 0x40, /*!< 256 extra clock cycles provided to the MCU before switching to STANDBY state */
EXTRA_CLOCK_CYCLES_512	= 0x60  /*!< 512 extra clock cycles provided to the MCU before switching to STANDBY state */
} ExtraClockCycles;


/**
 * @brief  S2LP Clock Output initialization structure definition.
 */
typedef struct {
  ClockOutputXOPrescaler   xClockOutputXOPrescaler;  /*!< Set the XO Ratio. @ref ClockOutputXOPrescaler */
  ClockOutputRCOPrescaler  xClockOutputRCOPrescaler; /*!< Set the RCO Ratio. @ref ClockOutputRCOPrescaler */
  ExtraClockCycles         xExtraClockCycles;       /*!< Set the Extra Clock Cycles provided before entering in Standby state. @ref ExtraClockCycles */
} ClockOutputInit;


/**
 * @brief IRQ bitfield structure for S2LP. This structure is used to read or write the single IRQ bit.
 *        During the initialization the user has to fill this structure setting to one the single field related
 *        to the IRQ he wants to enable, and to zero the single field related to all the IRQs he wants to disable.
 *        The same structure can be used to retrieve all the IRQ events from the IRQ registers IRQ_STATUS[3:0],
 *        and read if one or more specific IRQ raised.
 * @note  The fields order in the structure depends on used endianness (little or big
 *        endian). The actual definition is valid ONLY for LITTLE ENDIAN mode. Be sure to
 *        change opportunely the fields order when use a different endianness.
 */
typedef struct {
  SFlagStatus  IRQ_RX_DATA_READY:1;            /*!< IRQ: RX data ready */
  SFlagStatus  IRQ_RX_DATA_DISC:1;             /*!< IRQ: RX data discarded (upon filtering) */
  SFlagStatus  IRQ_TX_DATA_SENT:1;             /*!< IRQ: TX data sent */
  SFlagStatus  IRQ_MAX_RE_TX_REACH:1;          /*!< IRQ: Max re-TX reached */
  SFlagStatus  IRQ_CRC_ERROR:1;                /*!< IRQ: CRC error */
  SFlagStatus  IRQ_TX_FIFO_ERROR:1;            /*!< IRQ: TX FIFO underflow/overflow error */
  SFlagStatus  IRQ_RX_FIFO_ERROR:1;            /*!< IRQ: RX FIFO underflow/overflow error */
  SFlagStatus  IRQ_TX_FIFO_ALMOST_FULL:1;      /*!< IRQ: TX FIFO almost full */

  SFlagStatus  IRQ_TX_FIFO_ALMOST_EMPTY:1;     /*!< IRQ: TX FIFO almost empty */
  SFlagStatus  IRQ_RX_FIFO_ALMOST_FULL:1;      /*!< IRQ: RX FIFO almost full */
  SFlagStatus  IRQ_RX_FIFO_ALMOST_EMPTY:1;     /*!< IRQ: RX FIFO almost empty  */
  SFlagStatus  IRQ_MAX_BO_CCA_REACH:1;         /*!< IRQ: Max number of back-off during CCA */
  SFlagStatus  IRQ_VALID_PREAMBLE:1;           /*!< IRQ: Valid preamble detected */
  SFlagStatus  IRQ_VALID_SYNC:1;               /*!< IRQ: Sync word detected */
  SFlagStatus  IRQ_RSSI_ABOVE_TH:1;            /*!< IRQ: RSSI above threshold */
  SFlagStatus  IRQ_WKUP_TOUT_LDC:1;            /*!< IRQ: Wake-up timeout in LDC mode */

  SFlagStatus  IRQ_READY:1;                    /*!< IRQ: READY state */
  SFlagStatus  IRQ_STANDBY_DELAYED:1;          /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
  SFlagStatus  IRQ_LOW_BATT_LVL:1;             /*!< IRQ: Battery level below threshold*/
  SFlagStatus  IRQ_POR:1;                      /*!< IRQ: Power On Reset */
  SFlagStatus  IRQ_BOR:1;                      /*!< IRQ: Brown out event (both accurate and inaccurate)*/
  SFlagStatus  IRQ_LOCK:1;                     /*!< IRQ: LOCK state */
  SFlagStatus  IRQ_VCO_CALIBRATION_END:1;      /*!< IRQ: End of VCO calibration procedure */
  SFlagStatus  IRQ_PA_CALIBRATION_END:1;       /*!< IRQ: End of PA calibration procedure */

  SFlagStatus  IRQ_PM_COUNT_EXPIRED:1;         /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
  SFlagStatus  IRQ_XO_COUNT_EXPIRED:1;         /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */
  SFlagStatus  IRQ_TX_START_TIME:1;            /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
  SFlagStatus  IRQ_RX_START_TIME:1;            /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
  SFlagStatus  IRQ_RX_TIMEOUT:1;               /*!< IRQ: RX operation timeout */
  SFlagStatus  IRQ_RX_SNIFF_TIMEOUT:1;         /*!< IRQ: RX sniff opeartion timeout */
  SFlagStatus  :2;                             /*!< Reserved bit */
} S2LPIrqs;


/**
 * @brief  IRQ list enumeration for S2LP. This enumeration type can be used to address a
 *         specific IRQ.
 */
typedef enum {
  RX_DATA_READY = 0x00000001,           /*!< IRQ: RX data ready */
  RX_DATA_DISC = 0x00000002,            /*!< IRQ: RX data discarded (upon filtering) */
  TX_DATA_SENT = 0x00000004,            /*!< IRQ: TX data sent */
  MAX_RE_TX_REACH = 0x00000008,         /*!< IRQ: Max re-TX reached */
  CRC_ERROR = 0x00000010,               /*!< IRQ: CRC error */
  TX_FIFO_ERROR = 0x00000020,           /*!< IRQ: TX FIFO underflow/overflow error */
  RX_FIFO_ERROR = 0x00000040,           /*!< IRQ: RX FIFO underflow/overflow error */
  TX_FIFO_ALMOST_FULL = 0x00000080,     /*!< IRQ: TX FIFO almost full */
  TX_FIFO_ALMOST_EMPTY = 0x00000100,    /*!< IRQ: TX FIFO almost empty */
  RX_FIFO_ALMOST_FULL = 0x00000200,     /*!< IRQ: RX FIFO almost full */
  RX_FIFO_ALMOST_EMPTY = 0x00000400,    /*!< IRQ: RX FIFO almost empty  */
  MAX_BO_CCA_REACH = 0x00000800,        /*!< IRQ: Max number of back-off during CCA */
  VALID_PREAMBLE = 0x00001000,          /*!< IRQ: Valid preamble detected */
  VALID_SYNC = 0x00002000,              /*!< IRQ: Sync word detected */
  RSSI_ABOVE_TH = 0x00004000,           /*!< IRQ: RSSI above threshold */
  WKUP_TOUT_LDC = 0x00008000,           /*!< IRQ: Wake-up timeout in LDC mode */
  READY = 0x00010000,                   /*!< IRQ: READY state */
  STANDBY_DELAYED = 0x00020000,         /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
  LOW_BATT_LVL = 0x00040000,            /*!< IRQ: Battery level below threshold*/
  POR = 0x00080000,                     /*!< IRQ: Power On Reset */
  BOR = 0x00100000,                     /*!< IRQ: Brown out event (both accurate and inaccurate)*/
  LOCK = 0x00200000,                    /*!< IRQ: LOCK state */
  VCO_CALIBRATION_END = 0x00400000,        /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
  PA_CALIBRATION_END = 0x00800000,        /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */
  PM_COUNT_EXPIRED = 0x01000000,        /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
  XO_COUNT_EXPIRED = 0x02000000,        /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */
  TX_START_TIME = 0x04000000,	        /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
  RX_START_TIME = 0x08000000,	        /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
  RX_TIMEOUT = 0x10000000,	        /*!< IRQ: RX operation timeout */
  RX_SNIFF_TIMEOUT = 0x20000000,                 /*!< IRQ: RX sniff operation timeout */
  ALL_IRQ = 0x7FFFFFFF			/*!< All the above mentioned IRQs */
} IrqList;


/**
 * @}
 */



/** @defgroup Gpio_Exported_Functions   GPIO Exported Functions
 * @{
 */

void S2LPGpioInit(SGpioInit* pxGpioInitStruct);
void S2LPGpioSetLevel(S2LPGpioPin xGpioX, OutputLevel xLevel);
OutputLevel S2LPGpioGetLevel(S2LPGpioPin xGpioX);
void S2LPGpioClockOutput(SFunctionalState xNewState);
void S2LPGpioClockOutputInit(ClockOutputInit* pxClockOutputInitStruct);
void S2LPGpioSetXOPrescaler(ClockOutputXOPrescaler xXOPrescaler);
ClockOutputXOPrescaler S2LPGpioGetXOPrescaler(void);
void S2LPGpioSetRCOPrescaler(ClockOutputRCOPrescaler xRCOPrescaler);
ClockOutputRCOPrescaler S2LPGpioGetRCOPrescaler(void);
void S2LPGpioSetExtraClockCycles(ExtraClockCycles xExtraCycles);
ExtraClockCycles S2LPGpioGetExtraClockCycles(void);

void S2LPGpioIrqDeInit(S2LPIrqs* pxIrqInit);
void S2LPGpioIrqInit(S2LPIrqs* pxIrqInit);
void S2LPGpioIrqConfig(IrqList xIrq, SFunctionalState xNewState);
void S2LPGpioIrqGetMask(S2LPIrqs* pxIrqMask);
void S2LPGpioIrqGetStatus(S2LPIrqs* pxIrqStatus);
void S2LPGpioIrqClearStatus(void);
SBool S2LPGpioIrqCheckFlag(IrqList xFlag);

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
