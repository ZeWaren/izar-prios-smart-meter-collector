#ifndef __PRIOS_H
#define __PRIOS_H

#include <stdint.h>
#include <stdbool.h>

/** Contains all the booleans required to store the alarms of a PRIOS device. */
typedef struct _izar_alarms {
    bool general_alarm;
    bool leakage_currently;
    bool leakage_previously;
    bool meter_blocked;
    bool back_flow;
    bool underflow;
    bool overflow;
    bool submarine;
    bool sensor_fraud_currently;
    bool sensor_fraud_previously;
    bool mechanical_fraud_currently;
    bool mechanical_fraud_previously;
} izar_alarms;

/** Describe the various unit types we can encounter */
typedef enum unit_type {UNKNOWN_UNIT, VOLUME_CUBIC_METER} unit_type_t;

/** Can store everything a PRIOS frame can offer */
typedef struct _izar_reading {
    izar_alarms alarms;
    uint8_t random_generator;
    uint8_t radio_interval;
    float remaining_battery_life;
    unit_type_t unit_type;
    float current_reading;
    float h0_reading;
    uint16_t h0_year;
    uint8_t h0_month;
    uint8_t h0_day;
} izar_reading;

void printIZARReadingAsCSV(const uint32_t A_Id, const izar_reading * const reading);
uint8_t decodePRIOSPayload(const uint8_t * const in, const uint8_t payload_len, const uint32_t key, uint8_t *out);
uint8_t getMetricsFromPRIOSWMBusFrame(const uint8_t * const frame, izar_reading * const reading);

#endif
