#ifndef __PRIOS_H
#define __PRIOS_H

#include <stdint.h>

uint8_t decodePRIOSPayload(uint8_t *in, uint8_t payload_len, uint32_t key, uint8_t *out);
uint8_t getMetricsFromPRIOSWMBusFrame(uint8_t *frame, uint32_t *total_consumption, uint32_t *last_month_total_consumption);

#endif
