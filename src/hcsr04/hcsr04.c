#include "hcsr04.h"
#include "../global_variables.h"
#include "../uart/uart.h"

float measureDistanceAvg(void) {
    float average = 0;
    int i = 0;
    for (i = 0; i < 10; i++) {
        Trigger_Ultrasonic();
        average += Measure_Echo();
        osDelay(1);
    }

    average /= 10;

    // char string[32];
    // snprintf(string, sizeof(string), "Distance: %.2f cm\r\n", average);
    // UARTSendString(string);

    return average;
}

// void delayMicroseconds(uint32_t us) {
//     SysCtlDelay(SysClock / 3 / 1000000 * us);
// }

// void delayMilliseconds(uint32_t ms) {
//     SysCtlDelay(SysClock / 3 / 1000 * ms);
// }

// void delaySeconds(uint32_t s) {
//     SysCtlDelay(SysClock / 3 * s);
// }


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
    IntEnable(INT_TIMER1A);
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
    uint32_t start_time, end_time;
    while (GPIOPinRead(ECHO_PORT, ECHO_PIN) == 0);
    start_time = TimerValueGet(TIMER1_BASE, TIMER_A);

    while (GPIOPinRead(ECHO_PORT, ECHO_PIN) != 0);
    end_time = TimerValueGet(TIMER1_BASE, TIMER_A);

    // reset timer
    TimerLoadSet(TIMER1_BASE, TIMER_A,  0xFFFFFFFF);

    return  ((end_time - start_time) / 200.0) * 0.0343;
}
