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
/**
 * Arquivo para testes das funções de movimento
 */

uint32_t SysClock = 0;

int main(void) {
    SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);
    
    movementInit();

    while (1) {

        turnLeft();
        SysCtlDelay(SysClock);
        stop();
        SysCtlDelay(SysClock);

        moveForward();
        SysCtlDelay(SysClock);
        stop();
        SysCtlDelay(SysClock);

        turnRight();
        SysCtlDelay(SysClock);
        stop();
        SysCtlDelay(SysClock);

        moveBackwards();
        SysCtlDelay(SysClock);
        stop();
        SysCtlDelay(SysClock);

    }
}
