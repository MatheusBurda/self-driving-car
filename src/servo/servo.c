#include "servo.h"
#include "../global_variables.h"

volatile uint32_t pulseWidth = 0;
volatile bool pwmState = false;

void Timer0IntHandler(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    uint32_t next_timer;

    next_timer = pwmState ? (SysClock / 50 - SysClock * pulseWidth / 5000) : (SysClock * pulseWidth / 5000);

    TimerLoadSet(TIMER0_BASE, TIMER_A, next_timer - 1);

    pwmState = !pwmState;

    if (pwmState) {
        GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, GPIO_PIN_1);
    } else {
        GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
    }
}

void servoSetup(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    uint32_t timerPeriod = SysClock / 50;
    TimerLoadSet(TIMER0_BASE, TIMER_A, timerPeriod - 1);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    TimerEnable(TIMER0_BASE, TIMER_A);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG));
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_1);
}

void servo0deg(void){
    pulseWidth = 2;
}

void servo45deg(void){
    pulseWidth = 4;
}

void servo90deg(void){
    pulseWidth = 6;
}

void servo135deg(void){
    pulseWidth = 8;
}

void servo180deg(void){
    pulseWidth = 10 ;
}

void disableServo(void) {
	TimerDisable(TIMER0_BASE, TIMER_A);
}

void enableServo(void) {
	TimerEnable(TIMER0_BASE, TIMER_A);
}
