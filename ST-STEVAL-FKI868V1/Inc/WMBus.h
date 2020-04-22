#ifndef __WMBUS_H
#define __WMBUS_H

uint8_t CheckWMBusFrame(const uint8_t * const frame, const uint8_t len, uint8_t * const LField, uint8_t * const CField, uint16_t * const Manufacturer, uint32_t * const A_Id, uint8_t * const A_Ver, uint8_t * const A_Type);

#endif