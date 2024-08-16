#ifndef HCSR04_H
#define HCSR04_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/hw_ints.h"
#include "cmsis_os2.h"

#define TRIGGER_PIN GPIO_PIN_4
#define ECHO_PIN    GPIO_PIN_5
#define TRIGGER_PORT GPIO_PORTA_BASE
#define ECHO_PORT    GPIO_PORTA_BASE

extern float measureDistanceAvg(void);

extern  void initHCSR04(void);

extern  void Trigger_Ultrasonic(void);

float Measure_Echo(void);

#endif