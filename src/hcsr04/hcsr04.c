#include "hcsr04.h"
#include "../global_variables.h"
#include "../uart/uart.h"

float measureDistanceAvg(void) {
    float average = 0;
    int i = 0;
    for (i = 0; i < 10; i++) {
        average += Measure_Echo();
        osDelay(1);
    }

    average /= 10;

    return average;
}

void initHCSR04(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    // Set Trigger pin as output
    GPIOPinTypeGPIOOutput(TRIGGER_PORT, TRIGGER_PIN);
    // Set Echo pin as input
    GPIOPinTypeGPIOInput(ECHO_PORT, ECHO_PIN);
    GPIOPadConfigSet(ECHO_PORT, ECHO_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC_UP);
    TimerLoadSet(TIMER1_BASE, TIMER_A,  0xFFFFFFFF);
    // IntEnable(INT_TIMER1A);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void Trigger_Ultrasonic(void) {
    GPIOPinWrite(TRIGGER_PORT, TRIGGER_PIN, 0);
    osDelay(2);
    GPIOPinWrite(TRIGGER_PORT, TRIGGER_PIN, TRIGGER_PIN);
    osDelay(10);
    GPIOPinWrite(TRIGGER_PORT, TRIGGER_PIN, 0);
}

float Measure_Echo(void) {
    Trigger_Ultrasonic();
    
    // reset timer
    TimerLoadSet(TIMER1_BASE, TIMER_A,  0xFFFFFFFF);

    uint32_t start_time, end_time;
    while (GPIOPinRead(ECHO_PORT, ECHO_PIN) == 0);
    start_time = TimerValueGet(TIMER1_BASE, TIMER_A);

    while (GPIOPinRead(ECHO_PORT, ECHO_PIN) != 0);
    end_time = TimerValueGet(TIMER1_BASE, TIMER_A);

    return  ((end_time - start_time) / 200.0) * 0.0343;
}
