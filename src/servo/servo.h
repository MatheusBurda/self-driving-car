#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "cmsis_os2.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "../uart/uart.h"

typedef enum LookingDirection {
    L_LEFT = 10,
    L_DIAGONAL_LEFT = 8, 
    L_FORWARD = 6,
    L_DIAGONAL_RIGHT = 4, 
    L_RIGHT = 2
}LookingDirection;

extern void turnServo(enum LookingDirection direction);

extern void servoSetup(void);
extern void disableServo(void);
extern void enableServo(void);

extern bool pwm_state;
extern uint32_t pulse_width;

#endif
