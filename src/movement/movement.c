#include "movement.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


void movementInit(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK));
    
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4);
}

void moveForward(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, MOTOR_PIN1 | MOTOR_PIN3);
}

void stop(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, 0);
}

void turnRight(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, MOTOR_PIN1 | MOTOR_PIN4);
}

void turnLeft(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, MOTOR_PIN2 | MOTOR_PIN3);
}

void moveBackwards(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, MOTOR_PIN2 | MOTOR_PIN4);
}
