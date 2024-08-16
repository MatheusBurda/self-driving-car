#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/uart.h"

void UARTIntHandler(void);
void SetupUart(uint32_t SysClock);
void UARTSendString(const char *str);

#endif