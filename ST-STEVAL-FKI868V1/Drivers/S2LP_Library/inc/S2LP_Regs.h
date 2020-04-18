/**
 * @file    S2LP_Regs.h
 * @author  ST Microelectronics
 * @version 1.3.0
 * @date    June, 2019
 * @brief This file  contains all the registers address and masks.
 * @details
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */



/**
 * \brief GPIO0_CONF register
 * \code
 * Read and Write
 * Default value: 0x0A
 * 7:3 GPIO_SELECT: Specify the GPIO0 I/O signal, default setting POR.
 * 2 RESERVED: -
 * 1:0 GPIO_MODE: GPIO0 Mode:,  01b: Digital Input,  10b: Digital Output Low Power,  11b: Digital Output High Power
 * \endcode
 */
#define GPIO0_CONF_ADDR			((uint8_t)0x00)

#define GPIO_SELECT_REGMASK			((uint8_t)0xF8)
#define GPIO_MODE_REGMASK			((uint8_t)0x03)


/**
 * \brief GPIO1_CONF register
 * \code
 * Read and Write
 * Default value: 0xA2
 * 7:3 GPIO_SELECT: Specify the GPIO1 I/O signal, default setting digital GND.
 * 2 RESERVED: -
 * 1:0 GPIO_MODE: GPIO1 Mode:,  01b: Digital Input1,  0b: Digital Output Low Power,  11b: Digital Output High Power
 * \endcode
 */
#define GPIO1_CONF_ADDR			((uint8_t)0x01)

#define GPIO_SELECT_REGMASK			((uint8_t)0xF8)
#define GPIO_MODE_REGMASK			((uint8_t)0x03)


/**
 * \brief GPIO2_CONF register
 * \code
 * Read and Write
 * Default value: 0xA2
 * 7:3 GPIO_SELECT: Specify the GPIO2 I/O signal, default setting digital GND.
 * 2 RESERVED: -
 * 1:0 GPIO_MODE: GPIO2 Mode:,  01b: Digital Input,  10b: Digital Output Low Power,  11b: Digital Output High Power
 * \endcode
 */
#define GPIO2_CONF_ADDR			((uint8_t)0x02)

#define GPIO_SELECT_REGMASK			((uint8_t)0xF8)
#define GPIO_MODE_REGMASK			((uint8_t)0x03)


/**
 * \brief GPIO3_CONF register
 * \code
 * Read and Write
 * Default value: 0xA2
 * 7:3 GPIO_SELECT: Specify the GPIO3 I/O signal, default setting digital GND.
 * 2 RESERVED: -
 * 1:0 GPIO_MODE: GPIO3 Mode:,  00b: Analog,  01b: Digital Input,  10b: Digital Output Low Power,  11b: Digital Output High Power
 * \endcode
 */
#define GPIO3_CONF_ADDR			((uint8_t)0x03)

#define GPIO_SELECT_REGMASK			((uint8_t)0xF8)
#define GPIO_MODE_REGMASK			((uint8_t)0x03)


/**
 * \brief MCU_CK_CONF register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7 EN_MCU_CLK: 1: The internal divider logic is running, so the MCU clock is available (but proper GPIO configuration is needed)
 * 6:5 CLOCK_TAIL: Number of extra clock cycles provided to the MCU before switching to STANDBY state.,  00b: 0 extra clock cycle,  01b: 64 extra clock cycles,  10b: 256 extra clock cycles,  11b: 512 extra clock cycles
 * 4:1 XO_RATIO: Divider for the XO clock output
 * 0 RCO_RATIO: Divider for the RCO clock output,  0: 1 , 1: 1/128
 * \endcode
 */
#define MCU_CK_CONF_ADDR			((uint8_t)0x04)

#define EN_MCU_CLK_REGMASK			((uint8_t)0x80)
#define CLOCK_TAIL_REGMASK			((uint8_t)0x60)
#define XO_RATIO_REGMASK			((uint8_t)0x1E)
#define RCO_RATIO_REGMASK			((uint8_t)0x01)


/**
 * \brief SYNT3 register
 * \code
 * Read and Write
 * Default value: 0x42
 * 7:5 PLL_CP_ISEL: Set the charge pump current according to the XTAL frequency.
 * 4 BS: Synthesizer band select. This parameter selects the out-of loop divide factor of the synthesizer:,  0: 4, band select factor for high band,  1: 8, band select factor for middle band.
 * 3:0 SYNT_27_24: MSB bits of the PLL programmable divider.
 * \endcode
 */
#define SYNT3_ADDR			((uint8_t)0x05)

#define PLL_CP_ISEL_REGMASK			((uint8_t)0xE0)
#define BS_REGMASK			((uint8_t)0x10)
#define SYNT_27_24_REGMASK			((uint8_t)0x0F)


/**
 * \brief SYNT2 register
 * \code
 * Read and Write
 * Default value: 0x16
 * 7:0 SYNT[23:16]: Intermediate bits of the PLL programmable divider.
 * \endcode
 */
#define SYNT2_ADDR			((uint8_t)0x06)

#define SYNT_23_16_REGMASK			((uint8_t)0xFF)


/**
 * \brief SYNT1 register
 * \code
 * Read and Write
 * Default value: 0x27
 * 7:0 SYNT[15:8]: Intermediate bits of the PLL programmable divider.
 * \endcode
 */
#define SYNT1_ADDR			((uint8_t)0x07)

#define SYNT_15_8_REGMASK			((uint8_t)0xFF)


/**
 * \brief SYNT0 register
 * \code
 * Read and Write
 * Default value: 0x62
 * 7:0 SYNT[7:0]: LSB bits of the PLL programmable divider.
 * \endcode
 */
#define SYNT0_ADDR			((uint8_t)0x08)

#define SYNT_7_0_REGMASK			((uint8_t)0xFF)


/**
 * \brief IF_OFFSET_ANA register
 * \code
 * Read and Write
 * Default value: 0x2A
 * 7:0 IF_OFFSET_ANA: Intermediate frequency setting for the analog RF synthesizer, default: 300 kHz.
 * \endcode
 */
#define IF_OFFSET_ANA_ADDR			((uint8_t)0x09)

#define IF_OFFSET_ANA_REGMASK			((uint8_t)0xFF)


/**
 * \brief IF_OFFSET_DIG register
 * \code
 * Read and Write
 * Default value: 0xB8
 * 7:0 IF_OFFSET_DIG: Intermediate frequency setting for the digital shift-to-baseband circuits, default: 300 kHz.
 * \endcode
 */
#define IF_OFFSET_DIG_ADDR			((uint8_t)0x0A)

#define IF_OFFSET_DIG_REGMASK			((uint8_t)0xFF)


/**
 * \brief CH_SPACE register
 * \code
 * Read and Write
 * Default value: 0x3F
 * 7:0 CH_SPACE: Channel spacing. From ~793Hz to ~200KHz in 793Hz steps, default 100kHz.
 * \endcode
 */
#define CH_SPACE_ADDR			((uint8_t)0x0C)

#define CH_SPACE_REGMASK			((uint8_t)0xFF)


/**
 * \brief CHNUM register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 CH_NUM: Channel number. This value is multiplied by the channel spacing and added to the synthesizer base frequency to generate the actual RF carrier frequency.
 * \endcode
 */
#define CHNUM_ADDR			((uint8_t)0x0D)

#define CH_NUM_REGMASK			((uint8_t)0xFF)


/**
 * \brief MOD4 register
 * \code
 * Read and Write
 * Default value: 0x83
 * 7:0 DATARATE_M[15:8]: The MSB of the mantissa value of the data rate equation, default 38.4 ksps.
 * \endcode
 */
#define MOD4_ADDR			((uint8_t)0x0E)

#define DATARATE_M_15_8_REGMASK			((uint8_t)0xFF)


/**
 * \brief MOD3 register
 * \code
 * Read and Write
 * Default value: 0x2B
 * 7:0 DATARATE_M[7:0]: The LSB of the mantissa value of the data rate equation, default 38.4 ksps.
 * \endcode
 */
#define MOD3_ADDR			((uint8_t)0x0F)

#define DATARATE_M_7_0_REGMASK			((uint8_t)0xFF)


/**
 * \brief MOD2 register
 * \code
 * Read and Write
 * Default value: 0x77
 * 7:4 MOD_TYPE: Modulation type:,  0: 2-FSK,  1: 4-FSK,  2: 2-GFSK BT=1,  3: 4-GFSK BT=1,  5: ASK/OOK,  7: unmodulated,  10: 2-GFSK BT=0.5,  12: 4-GFSK BT=0.5
 * 3:0 DATARATE_E: The exponent value of the data rate equation, default 38.4 ksps.
 * \endcode
 */
#define MOD2_ADDR			((uint8_t)0x10)

#define MOD_TYPE_REGMASK			((uint8_t)0xF0)
#define DATARATE_E_REGMASK			((uint8_t)0x0F)


/**
 * \brief MOD1 register
 * \code
 * Read and Write
 * Default value: 0x03
 * 7 PA_INTERP_EN: 1: enable the power interpolator
 * 6 MOD_INTERP_EN: 1: enable frequency interpolator
 * 5:4 G4FSK_CONST_MAP: Select the constellation map for 4-GFSK.
 * 3:0 FDEV_E: The exponent value of the frequency deviation equation, default 20 kHz.
 * \endcode
 */
#define MOD1_ADDR			((uint8_t)0x11)

#define PA_INTERP_EN_REGMASK			((uint8_t)0x80)
#define MOD_INTERP_EN_REGMASK			((uint8_t)0x40)
#define G4FSK_CONST_MAP_REGMASK			((uint8_t)0x30)
#define FDEV_E_REGMASK			((uint8_t)0x0F)


/**
 * \brief MOD0 register
 * \code
 * Read and Write
 * Default value: 0x93
 * 7:0 FDEV_M: The mantissa value of the frequency deviation equation, default 20 kHz.
 * \endcode
 */
#define MOD0_ADDR			((uint8_t)0x12)

#define FDEV_M_REGMASK			((uint8_t)0xFF)


/**
 * \brief CHFLT register
 * \code
 * Read and Write
 * Default value: 0x23
 * 7:4 CHFLT_M: The mantissa value of the receiver channel filter, default 100 kHz.
 * 3:0 CHFLT_E: The exponent value of the receiver channel filter, default 100 kHz.
 * \endcode
 */
#define CHFLT_ADDR			((uint8_t)0x13)

#define CHFLT_M_REGMASK			((uint8_t)0xF0)
#define CHFLT_E_REGMASK			((uint8_t)0x0F)


/**
 * \brief AFC2 register
 * \code
 * Read and Write
 * Default value: 0xC8
 * 7 AFC_FREEZE_ON_SYNC: 1: enable the freeze AFC correction upon sync word detection.
 * 6 AFC_ENABLED: 1: enable the AFC correction.
 * 5 AFC_MODE: Select AFC mode:,  0: AFC loop closed on slicer,  1: AFC loop closed on second conversion stage.
 * 4:0 RESERVED: -
 * \endcode
 */
#define AFC2_ADDR			((uint8_t)0x14)

#define AFC_FREEZE_ON_SYNC_REGMASK			((uint8_t)0x80)
#define AFC_ENABLED_REGMASK			((uint8_t)0x40)
#define AFC_MODE_REGMASK			((uint8_t)0x20)


/**
 * \brief AFC1 register
 * \code
 * Read and Write
 * Default value: 0x18
 * 7:0 AFC_FAST_PERIOD: The length of the AFC fast period.
 * \endcode
 */
#define AFC1_ADDR			((uint8_t)0x15)

#define AFC_FAST_PERIOD_REGMASK			((uint8_t)0xFF)


/**
 * \brief AFC0 register
 * \code
 * Read and Write
 * Default value: 0x25
 * 7:4 AFC_FAST_GAIN: The AFC loop gain in fast mode (2's log).
 * 3:0 AFC_SLOW_GAIN: The AFC loop gain in slow mode (2's log).
 * \endcode
 */
#define AFC0_ADDR			((uint8_t)0x16)

#define AFC_FAST_GAIN_REGMASK			((uint8_t)0xF0)
#define AFC_SLOW_GAIN_REGMASK			((uint8_t)0x0F)


/**
 * \brief RSSI_FLT register
 * \code
 * Read and Write
 * Default value: 0xE3
 * 7:4 RSSI_FLT: Gain of the RSSI filter.
 * 3:2 CS_MODE: Carrier sense mode:,  00b: Static CS,  01b: Dynamic CS with 6dB dynamic threshold,  10b: Dynamic CS with 12dB dynamic threshold,  11b: Dynamic CS with 18dB dynamic threshold.
 * 1:0 RESERVED: -
 * \endcode
 */
#define RSSI_FLT_ADDR			((uint8_t)0x17)
#define RSSI_FLT_REGMASK		((uint8_t)0xF0)
#define CS_MODE_REGMASK			((uint8_t)0x0C)


/**
 * \brief RSSI_TH register
 * \code
 * Read and Write
 * Default value: 0x28
 * 7:0 RSSI_TH: Signal detect threshold in 0.5 dB steps, default -120 dBm corresponds to 0x28.
 * \endcode
 */
#define RSSI_TH_ADDR			((uint8_t)0x18)

#define RSSI_TH_REGMASK			((uint8_t)0xFF)


/**
 * \brief ANT_SELECT_CONF register
 * \code
 * Read and Write
 * Default value: 0x45
 * 7 RESERVED: -
 * 6:5 EQU_CTRL: ISI cancellation equalizer:,  00: equalization disabled,  01: single pass equalization,  10: dual pass equalization.
 * 4 CS_BLANKING: Do not fill the RX FIFO with data if the CS is above threhold.
 * 3 AS_ENABLE: 1: enable the antenna switching.
 * 2:0 AS_MEAS_TIME: Set the measurement time.
 * \endcode
 */
#define ANT_SELECT_CONF_ADDR			((uint8_t)0x1F)

#define EQU_CTRL_REGMASK			((uint8_t)0x60)
#define CS_BLANKING_REGMASK			((uint8_t)0x10)
#define AS_ENABLE_REGMASK			((uint8_t)0x08)
#define AS_MEAS_TIME_REGMASK			((uint8_t)0x07)


/**
 * \brief CLOCKREC1 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:5 CLK_REC_P_GAIN_SLOW: Clock recovery slow loop gain (log2).
 * 4 CLK_REC_ALGO_SEL: Select the symbol timing recovery algorithm: 0: DLL, 1: PLL.
 * 3:0 CLK_REC_I_GAIN_SLOW: Set the integral slow gain for symbol timing recovery (PLL mode only).
 * \endcode
 */
#define CLOCKREC1_ADDR			((uint8_t)0x20)

#define CLK_REC_P_GAIN_SLOW_REGMASK			((uint8_t)0xE0)
#define CLK_REC_ALGO_SEL_REGMASK			((uint8_t)0x10)
#define CLK_REC_I_GAIN_SLOW_REGMASK			((uint8_t)0x0F)


/**
 * \brief CLOCKREC0 register
 * \code
 * Read and Write
 * Default value: 0x58
 * 7:5 CLK_REC_P_GAIN_FAST: Clock recovery fast loop gain (log2).
 * 4 PSTFLT_LEN: Select the post filter length: 0: 8 symbols, 1: 16 symbols.
 * 3:0 CLK_REC_I_GAIN_FAST: Set the integral fast gain for symbol timing recovery (PLL mode only)..
 * \endcode
 */
#define CLOCKREC0_ADDR			((uint8_t)0x21)

#define CLK_REC_P_GAIN_FAST_REGMASK			((uint8_t)0xE0)
#define PSTFLT_LEN_REGMASK			((uint8_t)0x10)
#define CLK_REC_I_GAIN_FAST_REGMASK			((uint8_t)0x0F)


/**
 * \brief PCKTCTRL6 register
 * \code
 * Read and Write
 * Default value: 0x80
 * 7:2 SYNC_LEN: The number of bits used for the SYNC field in the packet.
 * 1:0 PREAMBLE_LEN_9_8: The MSB of the number of '01 or '10' of the preamble of the packet.
 * \endcode
 */
#define PCKTCTRL6_ADDR			((uint8_t)0x2B)

#define SYNC_LEN_REGMASK			((uint8_t)0xFC)
#define PREAMBLE_LEN_9_8_REGMASK			((uint8_t)0x03)


/**
 * \brief PCKTCTRL5 register
 * \code
 * Read and Write
 * Default value: 0x10
 * 7:0 PREAMBLE_LEN[7:0]: The LSB of the number of '01 or '10' of the preamble of the packet.
 * \endcode
 */
#define PCKTCTRL5_ADDR			((uint8_t)0x2C)

#define PREAMBLE_LEN_7_0_REGMASK			((uint8_t)0xFF)


/**
 * \brief PCKTCTRL4 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7 LEN_WID: The number of bytes used for the length field: 0: 1 byte, 1: 2 bytes.
 * 6:4 RESERVED: -
 * 3 ADDRESS_LEN: 1: include the ADDRESS field in the packet.
 * 2:0 RESERVED: -
 * \endcode
 */
#define PCKTCTRL4_ADDR			((uint8_t)0x2D)

#define LEN_WID_REGMASK			((uint8_t)0x80)
#define ADDRESS_LEN_REGMASK			((uint8_t)0x08)


/**
 * \brief PCKTCTRL3 register
 * \code
 * Read and Write
 * Default value: 0x20
 * 7:6 PCKT_FRMT: Format of packet:,  0: Basic, 1: 802.15.4g, 2: UART OTA,  3: STack
 * 5:4 RX_MODE: RX mode:,  0: normal mode,  1: direct through FIFO,  2: direct through GPIO
 * 3 FSK4_SYM_SWAP: Select the symbol mapping for 4(G)FSK.
 * 2 BYTE_SWAP: Select the transmission order between MSB and LSB.
 * 1:0 PREAMBLE_SEL: Select the preamble pattern between '10' and '01'.
 * \endcode
 */
#define PCKTCTRL3_ADDR			((uint8_t)0x2E)

#define PCKT_FRMT_REGMASK			((uint8_t)0xC0)
#define RX_MODE_REGMASK			((uint8_t)0x30)
#define FSK4_SYM_SWAP_REGMASK			((uint8_t)0x08)
#define BYTE_SWAP_REGMASK			((uint8_t)0x04)
#define PREAMBLE_SEL_REGMASK			((uint8_t)0x03)


/**
 * \brief PCKTCTRL2 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:6 RESERVED: -
 * 5 FCS_TYPE_4G: FCS type in header field of 802.15.4g packet.
 * 4 FEC_TYPE_4G: Select the FEC type of 802.15.4g packet: 0: NRNSC. 1: RSC.
 * 3 INT_EN_4G: 1: enable the interleaving of 802.15.4g packet.
 * 2 MBUS_3OF6_EN: 1: enable the 3-out-of-6 encoding/decoding.
 * 1 MANCHESTER_EN: 1: enable the Manchester encoding/decoding.
 * 0 FIX_VAR_LEN: Packet length mode:,  0: fixed,  1: variable (in variable mode the field LEN_WID of PCKTCTRL3 register must be configured)
 * \endcode
 */
#define PCKTCTRL2_ADDR			((uint8_t)0x2F)

#define FCS_TYPE_4G_REGMASK			((uint8_t)0x20)
#define FEC_TYPE_4G_REGMASK			((uint8_t)0x10)
#define INT_EN_4G_REGMASK			((uint8_t)0x08)
#define MBUS_3OF6_EN_REGMASK			((uint8_t)0x04)
#define MANCHESTER_EN_REGMASK			((uint8_t)0x02)
#define FIX_VAR_LEN_REGMASK			((uint8_t)0x01)


/**
 * \brief PCKTCTRL1 register
 * \code
 * Read and Write
 * Default value: 0x2C
 * 7:5 CRC_MODE: CRC field:,  0: no CRC field,  1: CRC using poly 0x07,  2: CRC using poly 0x8005,  3: CRC using poly 0x1021,  4: CRC using poly 0x864CBF
 * 4 WHIT_EN: 1: enable the whitening mode.
 * 3:2 TXSOURCE: Tx source data:,  0: normal mode,  1: direct through FIFO,  2: direct through GPIO,  3: PN9
 * 1 SECOND_SYNC_SEL: In TX mode: 0 select the primary SYNC word, 1 select the secondary SYNC word., In RX mode: enable the dual SYNC word detection mode.
 * 0 FEC_EN: 1: enable the FEC encoding in TX or the Viterbi decoding in RX.
 * \endcode
 */
#define PCKTCTRL1_ADDR			((uint8_t)0x30)

#define CRC_MODE_REGMASK		((uint8_t)0xE0)
#define WHIT_EN_REGMASK			((uint8_t)0x10)
#define TXSOURCE_REGMASK		((uint8_t)0x0C)
#define SECOND_SYNC_SEL_REGMASK	((uint8_t)0x02)
#define FEC_EN_REGMASK			((uint8_t)0x01)


/**
 * \brief PCKTLEN1 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 PCKTLEN1: MSB of length of packet in bytes.
 * \endcode
 */
#define PCKTLEN1_ADDR			((uint8_t)0x31)

#define PCKTLEN1_REGMASK		((uint8_t)0xFF)


/**
 * \brief PCKTLEN0 register
 * \code
 * Read and Write
 * Default value: 0x14
 * 7:0 PCKTLEN0: LSB of length of packet in bytes.
 * \endcode
 */
#define PCKTLEN0_ADDR			((uint8_t)0x32)

#define PCKTLEN0_REGMASK			((uint8_t)0xFF)


/**
 * \brief SYNC3 register
 * \code
 * Read and Write
 * Default value: 0x88
 * 7:0 SYNC3: SYNC word byte 4.
 * \endcode
 */
#define SYNC3_ADDR			((uint8_t)0x33)

#define SYNC3_REGMASK			((uint8_t)0xFF)


/**
 * \brief SYNC2 register
 * \code
 * Read and Write
 * Default value: 0x88
 * 7:0 SYNC2: SYNC word byte 3.
 * \endcode
 */
#define SYNC2_ADDR			((uint8_t)0x34)

#define SYNC2_REGMASK			((uint8_t)0xFF)


/**
 * \brief SYNC1 register
 * \code
 * Read and Write
 * Default value: 0x88
 * 7:0 SYNC1: SYNC word byte 1.
 * \endcode
 */
#define SYNC1_ADDR			((uint8_t)0x35)

#define SYNC1_REGMASK			((uint8_t)0xFF)


/**
 * \brief SYNC0 register
 * \code
 * Read and Write
 * Default value: 0x88
 * 7:0 SYNC0: SYNC word byte 0.
 * \endcode
 */
#define SYNC0_ADDR			((uint8_t)0x36)

#define SYNC0_REGMASK			((uint8_t)0xFF)


/**
 * \brief QI register
 * \code
 * Read and Write
 * Default value: 0x01
 * 7:5 SQI_TH: SQI threshold.
 * 4:1 PQI_TH: PQI threshold.
 * 0 SQI_EN: 1: enable the SQI check.
 * \endcode
 */
#define QI_ADDR			((uint8_t)0x37)

#define SQI_TH_REGMASK			((uint8_t)0xE0)
#define PQI_TH_REGMASK			((uint8_t)0x1E)
#define SQI_EN_REGMASK			((uint8_t)0x01)


/**
 * \brief PCKT_PSTMBL register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 PCKT_PSTMBL: Set the packet postamble length.
 * \endcode
 */
#define PCKT_PSTMBL_ADDR			((uint8_t)0x38)

#define PCKT_PSTMBL_REGMASK			((uint8_t)0xFF)


/**
 * \brief PROTOCOL2 register
 * \code
 * Read and Write
 * Default value: 0x40
 * 7 CS_TIMEOUT_MASK: 1: enable the CS value contributes to timeout disabling.
 * 6 SQI_TIMEOUT_MASK: 1: enable the SQI value contributes to timeout disabling.
 * 5 PQI_TIMEOUT_MASK: 1: enable the PQI value contributes to timeout disabling.
 * 4:3 TX_SEQ_NUM_RELOAD: TX sequence number to be used when counting reset is required using the related command.
 * 2 FIFO_GPIO_OUT_MUX_SEL: 1: select the almost empty/full control for TX FIFO., 0: select the almost empty/full control for RX FIFO.
 * 1:0 LDC_TIMER_MULT: Set the LDC timer multiplier factor: 00b: x1, 01b: x2, 10b: x4, 11b: x8.
 * \endcode
 */
#define PROTOCOL2_ADDR			((uint8_t)0x39)

#define CS_TIMEOUT_MASK_REGMASK			((uint8_t)0x80)
#define SQI_TIMEOUT_MASK_REGMASK			((uint8_t)0x40)
#define PQI_TIMEOUT_MASK_REGMASK			((uint8_t)0x20)
#define TX_SEQ_NUM_RELOAD_REGMASK			((uint8_t)0x18)
#define FIFO_GPIO_OUT_MUX_SEL_REGMASK			((uint8_t)0x04)
#define LDC_TIMER_MULT_REGMASK			((uint8_t)0x03)


/**
 * \brief PROTOCOL1 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7 LDC_MODE: 1: enable the Low Duty Cycle mode.
 * 6 LDC_RELOAD_ON_SYNC: 1: enable the LDC timer reload mode.
 * 5 PIGGYBACKING: 1: enable the piggybacking.
 * 4 FAST_CS_TERM_EN: 1: enable the RX sniff timer.
 * 3 SEED_RELOAD: 1: enable the reload of the back-off random generator seed using the value written in the BU_COUNTER_SEED.
 * 2 CSMA_ON: 1 enable the CSMA channel access mode.
 * 1 CSMA_PERS_ON: 1: enable the CSMA persistent mode (no backoff cycles).
 * 0 AUTO_PCKT_FLT: 1: enable the automatic packet filtering control.
 * \endcode
 */
#define PROTOCOL1_ADDR			((uint8_t)0x3A)

#define LDC_MODE_REGMASK			((uint8_t)0x80)
#define LDC_RELOAD_ON_SYNC_REGMASK			((uint8_t)0x40)
#define PIGGYBACKING_REGMASK			((uint8_t)0x20)
#define FAST_CS_TERM_EN_REGMASK			((uint8_t)0x10)
#define SEED_RELOAD_REGMASK			((uint8_t)0x08)
#define CSMA_ON_REGMASK			((uint8_t)0x04)
#define CSMA_PERS_ON_REGMASK			((uint8_t)0x02)
#define AUTO_PCKT_FLT_REGMASK			((uint8_t)0x01)


/**
 * \brief PROTOCOL0 register
 * \code
 * Read and Write
 * Default value: 0x08
 * 7:4 NMAX_RETX: Max. number of re-TX (from 0 to 15)(0: re-transmission is not performed).
 * 3 NACK_TX: 1: field NO_ACK=1 on transmitted packet.
 * 2 AUTO_ACK: 1: enable the automatic acknowledgement if packet received request.
 * 1 PERS_RX: 1: enable the persistent RX mode.
 * 0 RESERVED: -
 * \endcode
 */
#define PROTOCOL0_ADDR			((uint8_t)0x3B)

#define NMAX_RETX_REGMASK			((uint8_t)0xF0)
#define NACK_TX_REGMASK			((uint8_t)0x08)
#define AUTO_ACK_REGMASK			((uint8_t)0x04)
#define PERS_RX_REGMASK			((uint8_t)0x02)


/**
 * \brief FIFO_CONFIG3 register
 * \code
 * Read and Write
 * Default value: 0x30
 * 7 RESERVED: -
 * 6:0 RX_AFTHR: Set the RX FIFO almost full threshold.
 * \endcode
 */
#define FIFO_CONFIG3_ADDR			((uint8_t)0x3C)

#define RX_AFTHR_REGMASK			((uint8_t)0x7F)


/**
 * \brief FIFO_CONFIG2 register
 * \code
 * Read and Write
 * Default value: 0x30
 * 7 RESERVED: -
 * 6:0 RX_AETHR: Set the RX FIFO almost empty threshold.
 * \endcode
 */
#define FIFO_CONFIG2_ADDR			((uint8_t)0x3D)

#define RX_AETHR_REGMASK			((uint8_t)0x7F)


/**
 * \brief FIFO_CONFIG1 register
 * \code
 * Read and Write
 * Default value: 0x30
 * 7 RESERVED: -
 * 6:0 TX_AFTHR: Set the TX FIFO almost full threshold.
 * \endcode
 */
#define FIFO_CONFIG1_ADDR			((uint8_t)0x3E)

#define TX_AFTHR_REGMASK			((uint8_t)0x7F)


/**
 * \brief FIFO_CONFIG0 register
 * \code
 * Read and Write
 * Default value: 0x30
 * 7 RESERVED: -
 * 6:0 TX_AETHR: Set the TX FIFO almost empty threshold.
 * \endcode
 */
#define FIFO_CONFIG0_ADDR			((uint8_t)0x3F)

#define TX_AETHR_REGMASK			((uint8_t)0x7F)


/**
 * \brief PCKT_FLT_OPTIONS register
 * \code
 * Read and Write
 * Default value: 0x40
 * 7 RESERVED: -
 * 6 RX_TIMEOUT_AND_OR_SEL: Logical boolean function applied to CS/SQI/PQI values: 1: OR, 0: AND.
 * 5 RESERVED: -
 * 4 SOURCE_ADDR_FLT: 1: RX packet accepted if its source field matches with RX_SOURCE_ADDR register
 * 3 DEST_VS_BROADCAST_ADDR: 1: RX packet accepted if its source field matches with BROADCAST_ADDR register.
 * 2 DEST_VS_MULTICAST_ADDR: 1: RX packet accepted if its destination address matches with MULTICAST_ADDR register.
 * 1 DEST_VS_SOURCE_ADDR: 1: RX packet accepted if its destination address matches with RX_SOURCE_ADDR register.
 * 0 CRC_FLT: 1: packet discarded if CRC is not valid.
 * \endcode
 */
#define PCKT_FLT_OPTIONS_ADDR			((uint8_t)0x40)

#define RX_TIMEOUT_AND_OR_SEL_REGMASK			((uint8_t)0x40)
#define SOURCE_ADDR_FLT_REGMASK			((uint8_t)0x10)
#define DEST_VS_BROADCAST_ADDR_REGMASK			((uint8_t)0x08)
#define DEST_VS_MULTICAST_ADDR_REGMASK			((uint8_t)0x04)
#define DEST_VS_SOURCE_ADDR_REGMASK			((uint8_t)0x02)
#define CRC_FLT_REGMASK			((uint8_t)0x01)


/**
 * \brief PCKT_FLT_GOALS4 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 RX_SOURCE_MASK/DUAL_SYNC3: If dual sync mode enabled: dual SYNC word byte 3., Otherwise mask register for source affress filtering.
 * \endcode
 */
#define PCKT_FLT_GOALS4_ADDR			((uint8_t)0x41)

#define RX_SOURCE_MASK_DUAL_SYNC3_REGMASK			((uint8_t)0xFF)


/**
 * \brief PCKT_FLT_GOALS3 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 RX_SOURCE_ADDR/DUAL_SYNC2: If dual sync mode enabled: dual SYNC word byte 2., Otherwise RX packet source or TX packet destination field.
 * \endcode
 */
#define PCKT_FLT_GOALS3_ADDR			((uint8_t)0x42)

#define RX_SOURCE_ADDR_DUAL_SYNC2_REGMASK			((uint8_t)0xFF)


/**
 * \brief PCKT_FLT_GOALS2 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 BROADCAST_ADDR/DUAL_SYNC1: If dual sync mode enabled: dual SYNC word byte 1., Broadcast address.
 * \endcode
 */
#define PCKT_FLT_GOALS2_ADDR			((uint8_t)0x43)

#define BROADCAST_ADDR_DUAL_SYNC1_REGMASK			((uint8_t)0xFF)


/**
 * \brief PCKT_FLT_GOALS1 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 MULTICAST_ADDR/DUAL_SYNC0: If dual sync mode enabled: dual SYNC word byte 0., Multicast address.
 * \endcode
 */
#define PCKT_FLT_GOALS1_ADDR			((uint8_t)0x44)

#define MULTICAST_ADDR_DUAL_SYNC0_REGMASK			((uint8_t)0xFF)


/**
 * \brief PCKT_FLT_GOALS0 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 TX_SOURCE_ADDR: Tx packet source or RX packet destination field.
 * \endcode
 */
#define PCKT_FLT_GOALS0_ADDR			((uint8_t)0x45)

#define TX_SOURCE_ADDR_REGMASK			((uint8_t)0xFF)


/**
 * \brief TIMERS5 register
 * \code
 * Read and Write
 * Default value: 0x01
 * 7:0 RX_TIMER_CNTR: Counter for RX timer.
 * \endcode
 */
#define TIMERS5_ADDR			((uint8_t)0x46)

#define RX_TIMER_CNTR_REGMASK			((uint8_t)0xFF)


/**
 * \brief TIMERS4 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 RX_TIMER_PRESC: Prescaler for RX timer.
 * \endcode
 */
#define TIMERS4_ADDR			((uint8_t)0x47)

#define RX_TIMER_PRESC_REGMASK			((uint8_t)0xFF)


/**
 * \brief TIMERS3 register
 * \code
 * Read and Write
 * Default value: 0x01
 * 7:0 LDC_TIMER_PRESC: Prescaler for wake up timer.
 * \endcode
 */
#define TIMERS3_ADDR			((uint8_t)0x48)

#define LDC_TIMER_PRESC_REGMASK			((uint8_t)0xFF)


/**
 * \brief TIMERS2 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 LDC_TIMER_CNTR: Counter for wake up timer.
 * \endcode
 */
#define TIMERS2_ADDR			((uint8_t)0x49)

#define LDC_TIMER_CNTR_REGMASK			((uint8_t)0xFF)


/**
 * \brief TIMERS1 register
 * \code
 * Read and Write
 * Default value: 0x01
 * 7:0 LDC_RELOAD_PRSC: Prescaler value for reload operation of wake up timer.
 * \endcode
 */
#define TIMERS1_ADDR			((uint8_t)0x4A)

#define LDC_RELOAD_PRSC_REGMASK			((uint8_t)0xFF)


/**
 * \brief TIMERS0 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 LDC_RELOAD_CNTR: Counter value for reload operation of wake up timer.
 * \endcode
 */
#define TIMERS0_ADDR			((uint8_t)0x4B)

#define LDC_RELOAD_CNTR_REGMASK			((uint8_t)0xFF)


/**
 * \brief CSMA_CONF3 register
 * \code
 * Read and Write
 * Default value: 0x4C
 * 7:0 BU_CNTR_SEED[14:8]: MSB part of the seed for the random generator used to apply the CSMA algorithm.
 * \endcode
 */
#define CSMA_CONF3_ADDR			((uint8_t)0x4C)

#define BU_CNTR_SEED_14_8_REGMASK			((uint8_t)0xFF)


/**
 * \brief CSMA_CONF2 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 BU_CNTR_SEED[7:0]: LSB part of the seed for the random generator used to apply the CSMA algorithm.
 * \endcode
 */
#define CSMA_CONF2_ADDR			((uint8_t)0x4D)

#define BU_CNTR_SEED_7_0_REGMASK			((uint8_t)0xFF)


/**
 * \brief CSMA_CONF1 register
 * \code
 * Read and Write
 * Default value: 0x04
 * 7:2 BU_PRSC: Prescaler value for the back-off unit BU.
 * 1:0 CCA_PERIOD: Multiplier for the Tcca timer.
 * \endcode
 */
#define CSMA_CONF1_ADDR			((uint8_t)0x4E)

#define BU_PRSC_REGMASK			((uint8_t)0xFC)
#define CCA_PERIOD_REGMASK			((uint8_t)0x03)


/**
 * \brief CSMA_CONF0 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:4 CCA_LEN: The number of time in which the listen operation is performed.
 * 3 RESERVED: -
 * 2:0 NBACKOFF_MAX: Max number of back-off cycles.
 * \endcode
 */
#define CSMA_CONF0_ADDR			((uint8_t)0x4F)

#define CCA_LEN_REGMASK			((uint8_t)0xF0)
#define NBACKOFF_MAX_REGMASK			((uint8_t)0x07)


/**
 * \brief IRQ_MASK3 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 INT_MASK[31:24]: Enable the routing of the interrupt flag on the configured IRQ GPIO.
 * \endcode
 */
#define IRQ_MASK3_ADDR			((uint8_t)0x50)

#define INT_MASK_31_24_REGMASK			((uint8_t)0xFF)


/**
 * \brief IRQ_MASK2 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 INT_MASK[23:16]: Enable the routing of the interrupt flag on the configured IRQ GPIO.
 * \endcode
 */
#define IRQ_MASK2_ADDR			((uint8_t)0x51)

#define INT_MASK_23_16_REGMASK			((uint8_t)0xFF)


/**
 * \brief IRQ_MASK1 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 INT_MASK[15:8]: Enable the routing of the interrupt flag on the configured IRQ GPIO.
 * \endcode
 */
#define IRQ_MASK1_ADDR			((uint8_t)0x52)

#define INT_MASK_15_8_REGMASK			((uint8_t)0xFF)


/**
 * \brief IRQ_MASK0 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 INT_MASK[7:0]: Enable the routing of the interrupt flag on the configured IRQ GPIO.
 * \endcode
 */
#define IRQ_MASK0_ADDR			((uint8_t)0x53)

#define INT_MASK_7_0_REGMASK			((uint8_t)0xFF)


/**
 * \brief FAST_RX_TIMER register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 RSSI_SETTLING_LIMIT[7:0]: RSSI settling limit word. This determines the duration of the
 *                      FAST_RX_TERM timer.
 * \endcode
 */
#define FAST_RX_TIMER_ADDR			((uint8_t)0x54)

#define RSSI_SETTLING_LIMIT			((uint8_t)0xFF)


/**
 * \brief PA_POWER8 register
 * \code
 * Read and Write
 * Default value: 0x01
 * 7 RESERVED: -
 * 6:0 PA_LEVEL8: Output power level for 8th slot.
 * \endcode
 */
#define PA_POWER8_ADDR			((uint8_t)0x5A)

#define PA_LEVEL8_REGMASK			((uint8_t)0x7F)


/**
 * \brief PA_POWER7 register
 * \code
 * Read and Write
 * Default value: 0x0C
 * 7 RESERVED: -
 * 6:0 PA_LEVEL_7: Output power level for 7th slot.
 * \endcode
 */
#define PA_POWER7_ADDR			((uint8_t)0x5B)

#define PA_LEVEL_7_REGMASK			((uint8_t)0x7F)


/**
 * \brief PA_POWER6 register
 * \code
 * Read and Write
 * Default value: 0x18
 * 7 RESERVED: -
 * 6:0 PA_LEVEL_6: Output power level for 6th slot.
 * \endcode
 */
#define PA_POWER6_ADDR			((uint8_t)0x5C)

#define PA_LEVEL_6_REGMASK			((uint8_t)0x7F)


/**
 * \brief PA_POWER5 register
 * \code
 * Read and Write
 * Default value: 0x24
 * 7 RESERVED: -
 * 6:0 PA_LEVEL_5: Output power level for 5th slot.
 * \endcode
 */
#define PA_POWER5_ADDR			((uint8_t)0x5D)

#define PA_LEVEL_5_REGMASK			((uint8_t)0x7F)


/**
 * \brief PA_POWER4 register
 * \code
 * Read and Write
 * Default value: 0x30
 * 7 RESERVED: -
 * 6:0 PA_LEVEL_4: Output power level for 4th slot.
 * \endcode
 */
#define PA_POWER4_ADDR			((uint8_t)0x5E)

#define PA_LEVEL_4_REGMASK			((uint8_t)0x7F)


/**
 * \brief PA_POWER3 register
 * \code
 * Read and Write
 * Default value: 0x48
 * 7 RESERVED: -
 * 6:0 PA_LEVEL_3: Output power level for 3rd slot.
 * \endcode
 */
#define PA_POWER3_ADDR			((uint8_t)0x5F)

#define PA_LEVEL_3_REGMASK			((uint8_t)0x7F)


/**
 * \brief PA_POWER2 register
 * \code
 * Read and Write
 * Default value: 0x60
 * 7 RESERVED: -
 * 6:0 PA_LEVEL_2: Output power level for 2nd slot.
 * \endcode
 */
#define PA_POWER2_ADDR			((uint8_t)0x60)

#define PA_LEVEL_2_REGMASK			((uint8_t)0x7F)


/**
 * \brief PA_POWER1 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7 RESERVED: -
 * 6:0 PA_LEVEL_1: Output power level for 1st slot.
 * \endcode
 */
#define PA_POWER1_ADDR			((uint8_t)0x61)

#define PA_LEVEL_1_REGMASK		((uint8_t)0x7F)


/**
 * \brief PA_POWER0 register
 * \code
 * Read and Write
 * Default value: 0x47
 * 7 DIG_SMOOTH_EN: 1: enable the generation of the internal signal TX_DATA which is the input of the FIR.
 * 6 PA_MAXDBM: 1: configure the PA to send maximum output power.
 * 5 PA_RAMP_EN: 1: enable the power ramping
 * 4:3 PA_RAMP_STEP_LEN: Set the step width (unit: 1/8 of bit period).
 * 2:0 PA_LEVEL_MAX_IDX: Final level for power ramping or selected output power index.
 * \endcode
 */
#define PA_POWER0_ADDR			((uint8_t)0x62)

#define DIG_SMOOTH_EN_REGMASK			((uint8_t)0x80)
#define PA_MAXDBM_REGMASK			((uint8_t)0x40)
#define PA_RAMP_EN_REGMASK			((uint8_t)0x20)
#define PA_RAMP_STEP_LEN_REGMASK			((uint8_t)0x18)
#define PA_LEVEL_MAX_IDX_REGMASK			((uint8_t)0x07)


/**
 * \brief PA_CONFIG1 register
 * \code
 * Read and Write
 * Default value: 0x03
 * 7:5 RESERVED: -
 * 4 LIN_NLOG: 1: enable the linear-to-log conversion of the PA code output.
 * 3:2 FIR_CFG: FIR configuration: 00b: filtering, 01b : ramping, 10b: switching.
 * 1 FIR_EN: 1: enable FIR.
 * 0 RESERVED: -
 * \endcode
 */
#define PA_CONFIG1_ADDR			((uint8_t)0x63)

#define LIN_NLOG_REGMASK			((uint8_t)0x10)
#define FIR_CFG_REGMASK			((uint8_t)0xC0)
#define FIR_EN_REGMASK			((uint8_t)0x02)


/**
 * \brief SYNTH_CONFIG2 register
 * \code
 * Read and Write
 * Default value: 0xD0
 * 7 RESERVED: -
 * 6 RESERVED: -
 * 5 RESERVED: -
 * 4:3 RESERVED: -
 * 2 PLL_PFD_SPLIT_EN: Enables increased DN current pulses to improve linearization of CP/PFD.
 * 1:0 RESERVED: -
 * \endcode
 */
#define SYNTH_CONFIG2_ADDR			((uint8_t)0x65)

#define PLL_PFD_SPLIT_EN_REGMASK			((uint8_t)0x04)


/**
 * \brief VCO_CONFIG register
 * \code
 * Read and Write
 * Default value: 0x02
 * 7:6 RESERVED: -
 * 5 VCO_CALAMP_EXT_SEL: 1 --> VCO amplitude calibration will be skipped (external amplitude word forced on VCO).
 * 4 VCO_CALFREQ_EXT_SEL: 1 --> VCO frequency calibration will be skipped (external amplitude word forced on VCO).
 * 3 RESERVED: -
 * 2:0 RESERVED: -
 * \endcode
 */
#define VCO_CONFIG_ADDR			((uint8_t)0x68)

#define VCO_CALAMP_EXT_SEL_REGMASK			((uint8_t)0x20)
#define VCO_CALFREQ_EXT_SEL_REGMASK			((uint8_t)0x10)


/**
 * \brief VCO_CALIBR_IN2 register
 * \code
 * Read and Write
 * Default value: 0x88
 * 7:4 VCO_CALAMP_TX: VCO magnitude calibration word (binary coding to be internally converted in thermometric code) used in TX.
 * 3:0 VCO_CALAMP_RX: VCO magnitude calibration word (binary coding to be internally converted in thermometric code) used in RX.
 * \endcode
 */
#define VCO_CALIBR_IN2_ADDR			((uint8_t)0x69)

#define VCO_CALAMP_TX_REGMASK			((uint8_t)0xF0)
#define VCO_CALAMP_RX_REGMASK			((uint8_t)0x0F)


/**
 * \brief VCO_CALIBR_IN1 register
 * \code
 * Read and Write
 * Default value: 0x40
 * 7 RESERVED: -
 * 6:0 VCO_CALFREQ_TX: VCO Cbank frequency calibration word to be used in TX.
 * \endcode
 */
#define VCO_CALIBR_IN1_ADDR			((uint8_t)0x6A)

#define VCO_CALFREQ_TX_REGMASK			((uint8_t)0x7F)


/**
 * \brief VCO_CALIBR_IN0 register
 * \code
 * Read and Write
 * Default value: 0x40
 * 7 RESERVED: -
 * 6:0 VCO_CALFREQ_RX: VCO Cbank frequency calibration word to be used in RX.
 * \endcode
 */
#define VCO_CALIBR_IN0_ADDR			((uint8_t)0x6B)

#define VCO_CALFREQ_RX_REGMASK			((uint8_t)0x7F)


/**
 * \brief XO_RCO_CONF1 register
 * \code
 * Read and Write
 * Default value: 0x6C
 * 7:6 RESERVED: -
 * 5 RESERVED: -
 * 4 PD_CLKDIV: 1: disable both dividers of digital clock (and reference clockfor the SMPS) and IF-ADC clock.
 * 3:2 RESERVED: -
 * 1:0 RESERVED: -
 * \endcode
 */
#define XO_RCO_CONF1_ADDR			((uint8_t)0x6C)

#define PD_CLKDIV_REGMASK			((uint8_t)0x10)


/**
 * \brief XO_RCO_CONF0 register
 * \code
 * Read and Write
 * Default value: 0x30
 * 7 EXT_REF: 0: reference signal from XO circuit, 1: reference signal from XIN pin.
 * 5:4 GM_CONF: Set the driver gm of the XO at start up.
 * 3 REFDIV: 1: enable the the reference clock divider.
 * 2 RESERVED: -
 * 1 EXT_RCO_OSC: 1: the 34.7 kHz signal must be supplied from any GPIO.
 * 0 RCO_CALIBRATION: 1: enable the automatic RCO calibration.
 * \endcode
 */
#define XO_RCO_CONF0_ADDR			((uint8_t)0x6D)

#define EXT_REF_REGMASK			((uint8_t)0x80)
#define GM_CONF_REGMASK			((uint8_t)0x70)
#define REFDIV_REGMASK			((uint8_t)0x08)
#define EXT_RCO_OSC_REGMASK			((uint8_t)0x02)
#define RCO_CALIBRATION_REGMASK			((uint8_t)0x01)


/**
 * \brief RCO_CALIBR_CONF3 register
 * \code
 * Read and Write
 * Default value: 0x70
 * 7:4 RWT_IN: RWT word value for the RCO.
 * 3:0 RFB_IN_4_1: MSB part of RFB word value for RCO.
 * \endcode
 */
#define RCO_CALIBR_CONF3_ADDR			((uint8_t)0x6E)

#define RWT_IN_REGMASK			((uint8_t)0xF0)
#define RFB_IN_4_1_REGMASK			((uint8_t)0x0F)


/**
 * \brief RCO_CALIBR_CONF2 register
 * \code
 * Read and Write
 * Default value: 0x4D
 * 7 RFB_IN[0]: LSB part of RFB word value for RCO.
 * 6:4 RESERVED: -
 * 6:4 RESERVED: -
 * 3 RESERVED: -
 * 2:0 RESERVED: -
 * \endcode
 */
#define RCO_CALIBR_CONF2_ADDR			((uint8_t)0x6F)

#define RFB_IN_0_REGMASK			((uint8_t)0x80)


/**
 * \brief PM_CONF4 register
 * \code
 * Read and Write
 * Default value: 0x17
 * 7 TEMP_SENSOR_EN: 1: enable the temperature sensor.
 * 6 TEMP_SENS_BUFF_EN: 1: enable the output buffer for the temperature sensor.
 * 5 EXT_SMPS: 1: disable the internal SMPS.
 * 4 RESERVED: -
 * 3 RESERVED: -
 * 2 RESERVED: -
 * 1:0 RESERVED: -
 * \endcode
 */
#define PM_CONF4_ADDR			((uint8_t)0x75)

#define TEMP_SENSOR_EN_REGMASK			((uint8_t)0x80)
#define TEMP_SENS_BUFF_EN_REGMASK			((uint8_t)0x40)
#define EXT_SMPS_REGMASK			((uint8_t)0x20)


/**
 * \brief PM_CONF3 register
 * \code
 * Read and Write
 * Default value: 0x20
 * 7 KRM_EN: 0: divider by 4 enabled (SMPS' switching frequency is FSW=FCLK/4), 1: rate multiplier enabled (SMPS' switching frequency is FSW=KRM*FOSC/(2^15).
 * 6:0 KRM_14_8: Sets the divider ratio (MSB) of the rate multiplier (default: FSW=FCLK/4)
 * \endcode
 */
#define PM_CONF3_ADDR			((uint8_t)0x76)

#define KRM_EN_REGMASK			((uint8_t)0x80)
#define KRM_14_8_REGMASK			((uint8_t)0x7F)


/**
 * \brief PM_CONF2 register
 * \code
 * Read and Write
 * Default value: 0x00
 * 7:0 KRM[7:0]: Sets the divider ratio (LSB) of the rate multiplier (default: FSW=FCLK/4)
 * \endcode
 */
#define PM_CONF2_ADDR			((uint8_t)0x77)

#define KRM_7_0_REGMASK			((uint8_t)0xFF)


/**
 * \brief PM_CONF1 register
 * \code
 * Read and Write
 * Default value: 0x39
 * 7 RESERVED: -
 * 6 BATTERY_LVL_EN: 1: enable battery level detector circuit.
 * 5:4 SET_BLD_TH: Set the BLD threshold: 00b: 2.7V, 01b: 2.5V, 10b: 2.3V, 00b: 2.1V.
 * 3 RESERVED: -
 * 2 RESERVED: -
 * 1 RESERVED: -
 * 0 RESERVED: -
 * \endcode
 */
#define PM_CONF1_ADDR			((uint8_t)0x78)

#define BATTERY_LVL_EN_REGMASK			((uint8_t)0x40)
#define SET_BLD_TH_REGMASK			((uint8_t)0x30)


/**
 * \brief PM_CONF0 register
 * \code
 * Read and Write
 * Default value: 0x42
 * 7 RESERVED: -
 * 6:4 SET_SMPS_LVL: 000: SMPS output voltage 1.1 V, 001: SMPS output voltage 1.2 V, 010: SMPS output voltage 1.3 V, 011: SMPS output voltage 1.4 V, 100: SMPS output voltage 1.5 V, 101: SMPS output voltage 1.6 V, 110: SMPS output voltage 1.7 V, 111: SMPS output voltage 1.8 V.
 * 3:2 RESERVED: -
 * 1 RESERVED: -
 * 0 SLEEP_MODE_SEL: 0: SLEEP without FIFO retention; 1: SLEEP with FIFO retention.
 * \endcode
 */
#define PM_CONF0_ADDR			((uint8_t)0x79)

#define SET_SMPS_LVL_REGMASK			((uint8_t)0x70)
#define SLEEP_MODE_SEL_REGMASK			((uint8_t)0x01)


/**
 * \brief MC_STATE1 register
 * \code
 * Read only
 * Default value: 0x52
 * 7:5 RESERVED: -
 * 4 RCO_CAL_OK: RCO calibration succesfully terminated.
 * 3 ANT_SEL: Currently selected antenna.
 * 2 TX_FIFO_FULL: 1: TX FIFO is full.
 * 1 RX_FIFO_EMPTY: 1: RX FIFO is empty.
 * 0 ERROR_LOCK: 1: RCO calibrator error.
 * \endcode
 */
#define MC_STATE1_ADDR			((uint8_t)0x8D)

#define RCO_CAL_OK_REGMASK			((uint8_t)0x10)
#define ANT_SEL_REGMASK			((uint8_t)0x08)
#define TX_FIFO_FULL_REGMASK			((uint8_t)0x04)
#define RX_FIFO_EMPTY_REGMASK			((uint8_t)0x02)
#define ERROR_LOCK_REGMASK			((uint8_t)0x01)


/**
 * \brief MC_STATE0 register
 * \code
 * Read only
 * Default value: 0x07
 * 7:1 STATE: Current state.
 * 0 XO_ON: 1: XO is operating.
 * \endcode
 */
#define MC_STATE0_ADDR			((uint8_t)0x8E)

#define STATE_REGMASK			((uint8_t)0xFE)
#define XO_ON_REGMASK			((uint8_t)0x01)


/**
 * \brief TX_FIFO_STATUS register
 * \code
 * Read only
 * Default value: 0x00
 * 7 RESERVED: -
 * 6:0 NELEM_TXFIFO: Number of elements in TX FIFO.
 * \endcode
 */
#define TX_FIFO_STATUS_ADDR			((uint8_t)0x8F)

#define NELEM_TXFIFO_REGMASK			((uint8_t)0x7F)


/**
 * \brief RX_FIFO_STATUS register
 * \code
 * Read only
 * Default value: 0x00
 * 7 RESERVED: -
 * 6:0 NELEM_RXFIFO: Number of elements in RX FIFO.
 * \endcode
 */
#define RX_FIFO_STATUS_ADDR			((uint8_t)0x90)

#define NELEM_RXFIFO_REGMASK			((uint8_t)0x7F)


/**
 * \brief RCO_CALIBR_OUT4 register
 * \code
 * Read only
 * Default value: 0x70
 * 7:4 RWT_OUT: RWT word from internal RCO calibrator.
 * 3:0 RFB_OUT_4_1: RFT word (MSB) from internal RCO calibrator.
 * \endcode
 */
#define RCO_CALIBR_OUT4_ADDR			((uint8_t)0x94)

#define RWT_OUT_REGMASK			((uint8_t)0xF0)
#define RFB_OUT_4_1_REGMASK			((uint8_t)0x0F)


/**
 * \brief RCO_CALIBR_OUT3 register
 * \code
 * Read only
 * Default value: 0x00
 * 7 RFB_OUT[0]: RFT word (LSB) from internal RCO calibrator.
 * 6:0 RESERVED: -
 * \endcode
 */
#define RCO_CALIBR_OUT3_ADDR			((uint8_t)0x95)

#define RFB_OUT_0_REGMASK			((uint8_t)0x80)


/**
 * \brief VCO_CALIBR_OUT1 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:4 RESERVED: -
 * 3:0 VCO_CAL_AMP_OUT: VCO magnitude calibration output word (binary coding internally converted from thermometric coding).
 * \endcode
 */
#define VCO_CALIBR_OUT1_ADDR			((uint8_t)0x99)

#define VCO_CAL_AMP_OUT_REGMASK			((uint8_t)0x0F)


/**
 * \brief VCO_CALIBR_OUT0 register
 * \code
 * Read only
 * Default value: 0x00
 * 7 RESERVED: -
 * 6:0 VCO_CAL_FREQ_OUT: VCO Cbank frequency calibration output word (binary coding internally converted from thermometric coding).
 * \endcode
 */
#define VCO_CALIBR_OUT0_ADDR			((uint8_t)0x9A)

#define VCO_CAL_FREQ_OUT_REGMASK			((uint8_t)0x7F)


/**
 * \brief TX_PCKT_INFO register
 * \code
 * Read only
 * Default value: 0x00
 * 7:6 RESERVED: -
 * 5:4 TX_SEQ_NUM: Current TX packet sequence number.
 * 3:0 N_RETX: Number of re-transmissions done for the last TX packet.
 * \endcode
 */
#define TX_PCKT_INFO_ADDR			((uint8_t)0x9C)

#define TX_SEQ_NUM_REGMASK			((uint8_t)0x30)
#define N_RETX_REGMASK			((uint8_t)0x0F)


/**
 * \brief RX_PCKT_INFO register
 * \code
 * Read only
 * Default value: 0x00
 * 7:3 RESERVED: -
 * 2 NACK_RX: NACK field of the received packet.
 * 1:0 RX_SEQ_NUM: Sequence number of the received packet.
 * \endcode
 */
#define RX_PCKT_INFO_ADDR			((uint8_t)0x9D)

#define NACK_RX_REGMASK			((uint8_t)0x04)
#define RX_SEQ_NUM_REGMASK			((uint8_t)0x03)


/**
 * \brief AFC_CORR register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 AFC_CORR: AFC corrected value.
 * \endcode
 */
#define AFC_CORR_ADDR			((uint8_t)0x9E)

#define AFC_CORR_REGMASK			((uint8_t)0xFF)


/**
 * \brief LINK_QUALIF2 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 PQI: PQI value of the received packet.
 * \endcode
 */
#define LINK_QUALIF2_ADDR			((uint8_t)0x9F)

#define PQI_REGMASK			((uint8_t)0xFF)


/**
 * \brief LINK_QUALIF1 register
 * \code
 * Read only
 * Default value: 0x00
 * 7 CS: Carrier Sense indication.
 * 6:0 SQI: SQI value of the recived packet.
 * \endcode
 */
#define LINK_QUALIF1_ADDR			((uint8_t)0xA0)

#define CS_REGMASK			((uint8_t)0x80)
#define SQI_REGMASK			((uint8_t)0x7F)


/**
 * \brief RSSI_LEVEL register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 RSSI_LEVEL: RSSI level captured at the end of the SYNC word detection of the received packet.
 * \endcode
 */
#define RSSI_LEVEL_ADDR			((uint8_t)0xA2)

#define RSSI_LEVEL_REGMASK			((uint8_t)0xFF)


/**
 * \brief RX_PCKT_LEN1 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 RX_PCKT_LEN[14:8]: MSB valueof the length of the packet received.
 * \endcode
 */
#define RX_PCKT_LEN1_ADDR			((uint8_t)0xA4)

#define RX_PCKT_LEN_14_8_REGMASK			((uint8_t)0xFF)


/**
 * \brief RX_PCKT_LEN0 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 RX_PCKT_LEN[7:0]: LSB valueof the length of the packet received.
 * \endcode
 */
#define RX_PCKT_LEN0_ADDR			((uint8_t)0xA5)

#define RX_PCKT_LEN_7_0_REGMASK			((uint8_t)0xFF)


/**
 * \brief CRC_FIELD3 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 CRC_FIELD3: CRC field 3 of the received packet.
 * \endcode
 */
#define CRC_FIELD3_ADDR			((uint8_t)0xA6)

#define CRC_FIELD3_REGMASK			((uint8_t)0xFF)


/**
 * \brief CRC_FIELD2 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 CRC_FIELD2: CRC field 2 of the received packet.
 * \endcode
 */
#define CRC_FIELD2_ADDR			((uint8_t)0xA7)

#define CRC_FIELD2_REGMASK			((uint8_t)0xFF)


/**
 * \brief CRC_FIELD1 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 CRC_FIELD1: CRC field 1 of the received packet.
 * \endcode
 */
#define CRC_FIELD1_ADDR			((uint8_t)0xA8)

#define CRC_FIELD1_REGMASK			((uint8_t)0xFF)


/**
 * \brief CRC_FIELD0 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 CRC_FIELD0: CRC field 0 of the received packet.
 * \endcode
 */
#define CRC_FIELD0_ADDR			((uint8_t)0xA9)

#define CRC_FIELD0_REGMASK			((uint8_t)0xFF)


/**
 * \brief RX_ADDRE_FIELD1 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 RX_ADDRE_FIELD1: Source address field of the received packet.
 * \endcode
 */
#define RX_ADDRE_FIELD1_ADDR			((uint8_t)0xAA)

#define RX_ADDRE_FIELD1_REGMASK			((uint8_t)0xFF)


/**
 * \brief RX_ADDRE_FIELD0 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 RX_ADDRE_FIELD0: Destination address field of the received packet.
 * \endcode
 */
#define RX_ADDRE_FIELD0_ADDR			((uint8_t)0xAB)

#define RX_ADDRE_FIELD0_REGMASK			((uint8_t)0xFF)


/**
 * \brief RSSI_LEVEL_RUN register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 RSSI_LEVEL_RUN: RSSI level of the received packet, which supports continuos fast SPI reading.
 * \endcode
 */
#define RSSI_LEVEL_RUN_ADDR			((uint8_t)0xEF)

#define RSSI_LEVEL_RUN_REGMASK			((uint8_t)0xFF)


/**
 * \brief DEVICE_INFO1 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 PARTNUM: S2-LP part number
 * \endcode
 */
#define DEVICE_INFO1_ADDR			((uint8_t)0xF0)

#define PARTNUM_REGMASK			((uint8_t)0xFF)


/**
 * \brief DEVICE_INFO0 register
 * \code
 * Read only
 * Default value: 0x41
 * 7:0 RSSI_LEVEL: S2-LP version number
 * \endcode
 */
#define DEVICE_INFO0_ADDR			((uint8_t)0xF1)

#define RSSI_LEVEL_REGMASK			((uint8_t)0xFF)


/**
 * \brief IRQ_STATUS3 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 INT_LEVEL[31:24]: Interrupt status register 3
 * \endcode
 */
#define IRQ_STATUS3_ADDR			((uint8_t)0xFA)

#define INT_LEVEL_31_24_REGMASK			((uint8_t)0xFF)


/**
 * \brief IRQ_STATUS2 register
 * \code
 * Read only
 * Default value: 0x09
 * 7:0 INT_LEVEL[23:16]: Interrupt status register 2
 * \endcode
 */
#define IRQ_STATUS2_ADDR			((uint8_t)0xFB)

#define INT_LEVEL_23_16_REGMASK			((uint8_t)0xFF)


/**
 * \brief IRQ_STATUS1 register
 * \code
 * Read only
 * Default value: 0x05
 * 7:0 INT_LEVEL[15:8]: Interrupt status register 1
 * \endcode
 */
#define IRQ_STATUS1_ADDR			((uint8_t)0xFC)

#define INT_LEVEL_15_8_REGMASK			((uint8_t)0xFF)


/**
 * \brief IRQ_STATUS0 register
 * \code
 * Read only
 * Default value: 0x00
 * 7:0 INT_LEVEL[7:0]: Interrupt status register 0
 * \endcode
 */
#define IRQ_STATUS0_ADDR			((uint8_t)0xFD)

#define INT_LEVEL_7_0_REGMASK			((uint8_t)0xFF)


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
