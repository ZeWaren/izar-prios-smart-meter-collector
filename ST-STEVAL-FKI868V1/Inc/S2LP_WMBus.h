#ifndef S2LP_WMBUS_h
#define S2LP_WMBUS_h

#include <stdint.h>

void S2LP_ConfigureSlaveBoardLink(uint32_t *PinIRQ);
void S2LP_ConfigureForWMBusT1Receiver(void);
void S2LP_ConfigureEnableIrqs(void);
void S2LP_CompleteConfiguration(void);

void S2LP_HandleGPIOInterrupt(void);

#endif