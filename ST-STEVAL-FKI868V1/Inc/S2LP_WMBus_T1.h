#ifndef __WMBUS_T1_H
#define __WMBUS_T1_H

#define BASE_FREQUENCY              868950000
#define MODULATION_SELECT           MOD_2FSK
#define DATARATE                    100000
#define FREQ_DEVIATION              75000
#define BANDWIDTH                   350000
#define POWER_DBM                   12/*.0*/
#define PREAMBLE_LENGTH             19
#define SYNC_LENGTH                 10
#define SYNC_WORD                   0x0F400000
#define VARIABLE_LENGTH             S_DISABLE
#define EXTENDED_LENGTH_FIELD       S_DISABLE
#define PAYLOAD_LENGTH              0x1E
#define CRC_MODE                    PKT_NO_CRC
#define EN_ADDRESS                  S_DISABLE
#define EN_FEC                      S_DISABLE
#define EN_WHITENING                S_DISABLE
#define RSSI_THRESHOLD              -100
#define RX_TIMER_TIMEOUT_US         700E3

#endif
