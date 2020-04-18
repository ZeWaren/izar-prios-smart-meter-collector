#ifndef __WMBUS_H
#define __WMBUS_H

uint8_t CheckWMBusFrame(uint8_t *frame, uint8_t len, uint8_t *LField, uint8_t *CField, uint16_t *Manufacturer, uint32_t *A_Id, uint8_t *A_Ver, uint8_t *A_Type);

#endif