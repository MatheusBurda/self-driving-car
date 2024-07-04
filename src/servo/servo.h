#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

extern void servoSetup(void);
extern void servo0deg(void);
extern void servo45deg(void);
extern void servo90deg(void);
extern void servo135deg(void);
extern void servo180deg(void);
extern void disableServo(void);
extern void enableServo(void);

extern bool pwm_state;
extern uint32_t pulse_width;

#endif
