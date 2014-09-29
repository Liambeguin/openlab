#ifndef DATALINK_H
#define DATALINK_H

#include "uart.h"

void datalink_config (uart_t uart); 
void datalink_send (void *data, uint16_t size);

#endif /* DATALINK_H */
