#include "S2LP_WMBus.h"
#include "S2LP_Config.h"
#include "S2LP_Middleware_Config.h"
#include "SDK_UTILS_Timers.h"

#include "WMBus.h"
#include "PRIOS.h"

#define IRQ_PREEMPTION_PRIORITY         0x03

S2LPIrqs xIrqStatus;
uint8_t s2lpRxData[64];

void S2LP_HandleGPIOInterrupt() {
      /* Get the IRQ status */
    S2LPGpioIrqGetStatus(&xIrqStatus);

    /* Check the S2LP RX_DATA_READY IRQ flag */
    if(xIrqStatus.IRQ_RX_DATA_READY) {
	/* Get the RX FIFO size */
	uint8_t cRxData = S2LPFifoReadNumberBytesRxFifo();

	/* Read the RX FIFO */
	S2LPSpiReadFifo(cRxData, s2lpRxData);

	/* Flush the RX FIFO */
	S2LPCmdStrobeFlushRxFifo();

	/* RX command - to ensure the device will be ready for the next reception */
	S2LPCmdStrobeRx();

        /* Let's see if we're dealing with a correct WMBus frame */
        uint8_t LField;
        uint8_t CField;
        uint16_t MField;
        uint32_t A_Id;
        uint8_t A_Ver;
        uint8_t A_Type;
        uint8_t isMBusFrame = CheckWMBusFrame(s2lpRxData, cRxData, &LField, &CField, &MField, &A_Id, &A_Ver, &A_Type);
        if (!isMBusFrame) {
            return;
        }
        
        /* The frame is an IZAR meter reporting data, let's handle it */
        if (LField == 0x19 && CField == 0x44 && MField == 0x4C30 && A_Ver == 0xD4 && A_Type == 0x01) {
            uint32_t total_consumption, last_month_total_consumption;
            if (!getMetricsFromPRIOSWMBusFrame(s2lpRxData, &total_consumption, &last_month_total_consumption)) {
                return;
            }
            
            uint8_t t = cRxData;
            t++;
        }
    }
}

void S2LP_ConfigureSlaveBoardLink(uint32_t *PinIRQ) {
    /* Put the radio off */
    S2LPShutdownInit();
    SdkDelayMs(10);
    S2LPShutdownExit();

    /* SPI init */
    S2LPSpiInit();
    
    /* uC IRQ config */
    S2LP_Middleware_GpioInit(M2S_GPIO_3, M2S_MODE_EXTI_IN);
    *PinIRQ = S2LP_Middleware_GpioGetPin(M2S_GPIO_3);

    SGpioInit xGpioIRQ = {
      S2LP_GPIO_3,
      S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP,
      S2LP_GPIO_DIG_OUT_IRQ
    };
    /* S2LP IRQ config */
    S2LPGpioInit(&xGpioIRQ);

    /* uC IRQ enable */
    S2LP_Middleware_GpioInterruptCmd(M2S_GPIO_3, IRQ_PREEMPTION_PRIORITY, 0, ENABLE);
}

#define BASE_FREQUENCY              868950000
#define MODULATION_SELECT           MOD_2FSK
#define DATARATE                    100000
#define FREQ_DEVIATION              75000
#define BANDWIDTH                   400000
#define POWER_DBM                   12/*.0*/
#define PREAMBLE_LENGTH             19
#define SYNC_LENGTH                 10
#define SYNC_WORD                   0x0F400000
#define VARIABLE_LENGTH             S_DISABLE
#define EXTENDED_LENGTH_FIELD       S_DISABLE
#define CRC_MODE                    PKT_NO_CRC
#define EN_ADDRESS                  S_DISABLE
#define EN_FEC                      S_DISABLE
#define EN_WHITENING                S_DISABLE


void S2LP_ConfigureForWMBusT1Receiver(void) {
    SRadioInit xRadioInit = {
        BASE_FREQUENCY,
        MODULATION_SELECT,
        DATARATE,
        FREQ_DEVIATION,
        BANDWIDTH
    };


    /**
     * @brief Packet Basic structure fitting
     */
    PktBasicInit xBasicInit={
        PREAMBLE_LENGTH,
        SYNC_LENGTH,
        SYNC_WORD,
        VARIABLE_LENGTH,
        EXTENDED_LENGTH_FIELD,
        CRC_MODE,
        EN_ADDRESS,
        EN_FEC,
        EN_WHITENING
    };
    
      /* S2LP Radio config */
    S2LPRadioInit(&xRadioInit);

    /* S2LP Packet config */
    S2LPPktBasicInit(&xBasicInit);

    /* S2LP IRQs enable */
    S2LPGpioIrqDeInit(&xIrqStatus);
    S2LPGpioIrqConfig(RX_DATA_DISC,S_ENABLE);
    S2LPGpioIrqConfig(RX_DATA_READY,S_ENABLE);

    /* payload length config */
    S2LPPktBasicSetPayloadLength(28);

    /* RX timeout config */
    S2LPTimerSetRxTimerUs(700000);
    
    uint8_t tmp[6];

    tmp[0] = 0xA3; /* reg. GPIO3_CONF (0x03) */
    //S2LPSpiWriteRegisters(0x03, 1, tmp);
    tmp[0] = 0x62; /* reg. SYNT3 (0x05) */
    tmp[1] = 0x2C; /* reg. SYNT2 (0x06) */
    tmp[2] = 0x22; /* reg. SYNT1 (0x07) */
    tmp[3] = 0x05; /* reg. SYNT0 (0x08) */
    tmp[4] = 0x2F; /* reg. IF_OFFSET_ANA (0x09) */
    tmp[5] = 0xC2; /* reg. IF_OFFSET_DIG (0x0A) */
    //S2LPSpiWriteRegisters(0x05, 6, tmp);
    tmp[0] = 0x06; /* reg. MOD4 (0x0E) */
    tmp[1] = 0x25; /* reg. MOD3 (0x0F) */
    tmp[2] = 0x09; /* reg. MOD2 (0x10) */
    tmp[3] = 0x05; /* reg. MOD1 (0x11) */
    tmp[4] = 0x89; /* reg. MOD0 (0x12) */
    tmp[5] = 0x41; /* reg. CHFLT (0x13) */
    S2LPSpiWriteRegisters(0x0E, 6, tmp);
    tmp[0] = 0x2E; /* reg. RSSI_TH (0x18) */
    S2LPSpiWriteRegisters(0x18, 1, tmp);
    tmp[0] = 0x55; /* reg. ANT_SELECT_CONF (0x1F) */
    S2LPSpiWriteRegisters(0x1F, 1, tmp);
    tmp[0] = 0x28; /* reg. PCKTCTRL6 (0x2B) */
    tmp[1] = 0x13; /* reg. PCKTCTRL5 (0x2C) */
    S2LPSpiWriteRegisters(0x2B, 2, tmp);
    tmp[0] = 0x00; /* reg. PCKTCTRL3 (0x2E) */
    tmp[1] = 0x04; /* reg. PCKTCTRL2 (0x2F) */
    tmp[2] = 0x00; /* reg. PCKTCTRL1 (0x30) */
    S2LPSpiWriteRegisters(0x2E, 3, tmp);
    tmp[0] = 0x1E; /* reg. PCKTLEN0 (0x32) */
    tmp[1] = 0x00; /* reg. SYNC3 (0x33) */
    tmp[2] = 0x00; /* reg. SYNC2 (0x34) */
    tmp[3] = 0x40; /* reg. SYNC1 (0x35) */
    tmp[4] = 0x0F; /* reg. SYNC0 (0x36) */
    S2LPSpiWriteRegisters(0x32, 5, tmp);
    tmp[0] = 0x44; /* reg. PROTOCOL2 (0x39) */
    tmp[1] = 0x01; /* reg. PROTOCOL1 (0x3A) */
    //S2LPSpiWriteRegisters(0x39, 2, tmp);
    tmp[0] = 0x40; /* reg. FIFO_CONFIG3 (0x3C) */
    tmp[1] = 0x40; /* reg. FIFO_CONFIG2 (0x3D) */
    tmp[2] = 0x40; /* reg. FIFO_CONFIG1 (0x3E) */
    tmp[3] = 0x40; /* reg. FIFO_CONFIG0 (0x3F) */
    //S2LPSpiWriteRegisters(0x3C, 4, tmp);
    tmp[0] = 0xCE; /* reg. TIMERS5 (0x46) */
    tmp[1] = 0x09; /* reg. TIMERS4 (0x47) */
    //S2LPSpiWriteRegisters(0x46, 2, tmp);
    tmp[0] = 0x33; /* reg. TIMERS2 (0x49) */
    //S2LPSpiWriteRegisters(0x49, 1, tmp);
    tmp[0] = 0x10; /* reg. IRQ_MASK3 (0x50) */
    //S2LPSpiWriteRegisters(0x50, 1, tmp);
    tmp[0] = 0x10; /* reg. IRQ_MASK0 (0x53) */
    //S2LPSpiWriteRegisters(0x53, 1, tmp);
    tmp[0] = 0x18; /* reg. PA_POWER8 (0x5A) */
    //S2LPSpiWriteRegisters(0x5A, 1, tmp);
    tmp[0] = 0x07; /* reg. PA_POWER0 (0x62) */
    tmp[1] = 0x01; /* reg. PA_CONFIG1 (0x63) */
    tmp[2] = 0x8B; /* reg. PA_CONFIG0 (0x64) */
    //S2LPSpiWriteRegisters(0x62, 3, tmp);
    tmp[0] = 0x62; /* reg. RCO_CALIBR_CONF3 (0x6E) */
    //S2LPSpiWriteRegisters(0x6E, 1, tmp);
    tmp[0] = 0x8F; /* reg. PM_CONF3 (0x76) */
    tmp[1] = 0xF9; /* reg. PM_CONF2 (0x77) */
    //S2LPSpiWriteRegisters(0x76, 2, tmp);
}

void S2LP_ConfigureEnableIrqs(void) {
    /* S2LP IRQs enable */
    S2LPGpioIrqDeInit(&xIrqStatus);
    S2LPGpioIrqConfig(RX_DATA_READY,S_ENABLE);
}

void S2LP_CompleteConfiguration(void) {
    /* Clear the initial state of the IRQ registers */
    S2LPGpioIrqClearStatus();
    
    /* RX command */
    S2LPCmdStrobeRx();
}

