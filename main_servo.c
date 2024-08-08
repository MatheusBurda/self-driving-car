#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "cmsis_os2.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "src/global_variables.h"
#include "src/movement/movement.h"
#include "src/uart/uart.h"
#include "src/servo/servo.h"

uint32_t SysClock = 0;

int main(void) {
    SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);
    
    servoSetup();

    while (1) {
        turnServo(L_RIGHT);
        SysCtlDelay(SysClock);
        turnServo(L_DIAGONAL_RIGHT);
        SysCtlDelay(SysClock);
        turnServo(L_FORWARD);
        SysCtlDelay(SysClock);
        turnServo(L_DIAGONAL_LEFT);
        SysCtlDelay(SysClock);
        turnServo(L_LEFT);
        SysCtlDelay(SysClock);
    }
}


