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
        turnServo(RIGHT);
        SysCtlDelay(SysClock);
        turnServo(DIAGONAL_RIGHT)
        SysCtlDelay(SysClock);
        turnServo(FORWARD);
        SysCtlDelay(SysClock);
        turnServo(DIAGONAL_LEFT);
        SysCtlDelay(SysClock);
        servo180deg();
        SysCtlDelay(SysClock);
        servo135deg();
        SysCtlDelay(SysClock);
        servo90deg();
        SysCtlDelay(SysClock);
        servo45deg();
        SysCtlDelay(SysClock);
    }
}


