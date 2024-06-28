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
#include "driverlib/adc.h"
#include "system_TM4C129.h"
#include <TM4C129.h>

#define SERVO_PWM_MODULE  TIMER0
#define SERVO_PWM_PIN     (1 << 5) // PE5

#define PERIOD_TIME 10000
#define PWM_FREQUENCY 12000

#define SERVO_MIN_PULSE_WIDTH  500
#define SERVO_MAX_PULSE_WIDTH  2500

extern void servoInit(uint32_t SysClock);
extern void servoSetPosition(uint8_t angle);

#endif
