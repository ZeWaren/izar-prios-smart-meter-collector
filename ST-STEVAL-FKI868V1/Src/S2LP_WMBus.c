/**
  ******************************************************************************
  * @file           : S2LP_WMBus.c
  * @brief          : Configure a ST-L2LP module to act as an IZAR WMBus
                      receiver.
  * @author         : Erwan Martin <public@fzwte.net>
  ******************************************************************************
  */
/* C includes */
#include <stdio.h>

/* Platform includes */
#include "S2LP_WMBus.h"
#include "S2LP_Config.h"
#include "S2LP_Middleware_Config.h"
#include "S2LP_Qi.h"
#include "SDK_UTILS_Timers.h"

/* Application includes */
#include "WMBus.h"
#include "PRIOS.h"
#include "S2LP_WMBus_T1.h"

/* Interruption related elements */
#define IRQ_PREEMPTION_PRIORITY         0x03
S2LPIrqs xIrqStatus;

/* Variable to hold the received data */
uint8_t s2lpRxData[64];

void S2LP_HandleGPIOInterrupt() {
    /* Get the IRQ status */
    S2LPGpioIrqGetStatus(&xIrqStatus);

    /* Check the S2LP RX_DATA_DISC IRQ flag */
    if(xIrqStatus.IRQ_RX_DATA_DISC) {
        /* Uncomment this for debugging purposes */
	//printf("DATA DISCARDED\n\r");

	/* RX command - to ensure the device will be ready for the next reception */
	S2LPCmdStrobeRx();
    }

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

        if (!(s2lpRxData[0] == 0x19 && s2lpRxData[1] == 0x44 && s2lpRxData[2] == 0x30 && s2lpRxData[3] == 0x4C)) {
          /* Let's not waste time checking the whole frame or calculting CRCs if the 1st 4 bytes are not the ones we're looking for */
          return;
        }

        /* Uncomment this for debugging purposes */
        /*for (uint8_t i = 0; i<cRxData; i++) {
            printf("%.2X ", s2lpRxData[i]);
        }
        printf("\r\n");*/

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
            izar_reading reading;
            if (!getMetricsFromPRIOSWMBusFrame(s2lpRxData, &reading)) {
                return;
            }
            
            /* Output the data on the COM port */
            printIZARReadingAsCSV(A_Id, &reading);
        }
    }
}

/**
  * @brief Configure the SPI link between the nucleo board and the S2LP board.
  * @param uint32_t *PinIRQ Return the GPIO IRQ reference
  */
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

/**
  * @brief Configure the S2-LP as a WMBus T1 receiver.
  */
void S2LP_ConfigureForWMBusT1Receiver(void) {
    SRadioInit xRadioInit = {
        BASE_FREQUENCY,
        MODULATION_SELECT,
        DATARATE,
        FREQ_DEVIATION,
        BANDWIDTH
    };

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
    S2LPPktBasicSetPayloadLength(PAYLOAD_LENGTH);

    /* RX timeout config */
    S2LPTimerSetRxTimerUs(RX_TIMER_TIMEOUT_US);
    
    /* RSSI threshold */
    S2LPRadioSetRssiThreshdBm(RSSI_THRESHOLD);
}

/**
  * @brief Enable the IRQ of the S2-LP board.
  */
void S2LP_ConfigureEnableIrqs(void) {
    /* S2LP IRQs enable */
    S2LPGpioIrqDeInit(&xIrqStatus);
    S2LPGpioIrqConfig(RX_DATA_DISC,S_ENABLE);
    S2LPGpioIrqConfig(RX_DATA_READY,S_ENABLE);
}

/**
  * @brief Complete the configuration of the S2-LP board, and start reception.
  */
void S2LP_CompleteConfigurationAndStart(void) {
    /* Clear the initial state of the IRQ registers */
    S2LPGpioIrqClearStatus();
    
    /* RX command */
    S2LPCmdStrobeRx();
}
