#ifndef __PRIOS_H
#define __PRIOS_H

#include <stdint.h>

uint8_t getMetricsFromPRIOSWMBusFrame(uint8_t *frame, uint32_t *total_consumption, uint32_t *last_month_total_consumption);

#endif
