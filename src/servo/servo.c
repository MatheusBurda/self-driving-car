#include "servo.h"

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"

// initialize the PWM module for servo control
void servoInit(uint32_t SysClock) {
    // Habiplita PWM e o port do PWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PG1_M0PWM5);
    // Configura o clock do PWM 
    SysCtlPWMClockSet(SYSCTL_PWMDIV_2); 

    // Configura gerador PWM
    uint32_t pwmPeriod = (SysClock/1) / PWM_FREQUENCY;  // Adjust clock division here
    PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, pwmPeriod);

    PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
    PWMGenEnable(PWM0_BASE, PWM_GEN_2);

}

// sets the servo position (in degrees)
void servoSetPosition(uint8_t angle) {
    uint32_t dutyCyclePercentage = (angle * 100) / 180 ;
	if (dutyCyclePercentage == 0) {
		PWMGenDisable(PWM0_BASE, PWM_GEN_2); // Disable PWM
	}
	else {
		PWMGenEnable(PWM0_BASE, PWM_GEN_2); // Enable PWM
		dutyCyclePercentage = dutyCyclePercentage > 100 ? 100 : dutyCyclePercentage;

		uint32_t pwmPeriod = PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2);
		uint32_t pulseWidth = (pwmPeriod * dutyCyclePercentage) / 100;

		// Set the new pulse width
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, pulseWidth);
	}
}



// // initialize the PWM module for servo control
// void Servo_Init(void) {
//     SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
//     while ((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R4) == 0) {}

//     SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_0;
//     while ((SYSCTL_RCGCTIMER_R & SYSCTL_RCGCTIMER_0) == 0) {}

//     GPIOPinTypePWM(GPIO_PORTE_BASE, SERVO_PWM_PIN);
//     GPIOPinConfigure(GPIO_PORTE_BASE, SERVO_PWM_PIN, GPIO_PCC_PE5_M0PWM4);

//     TimerConfigure(SERVO_PWM_MODULE, TIMER_CFG_MODE_PWM);
//     TimerControl(SERVO_PWM_MODULE, TIMER_CTL_SRSYNC, TIMER_CTRL_SRSYNC_SYS_CLK);

//     TimerControl(SERVO_PWM_MODULE, TIMER_CTL_TAMR, TIMER_TAMR_TAMR_1SHOT | TIMER_TAMR_TACDIR);
//     TimerControl(SERVO_PWM_MODULE, TIMER_CTL_TCMR, TIMER_CTL_TCMR_ON_CASCADE);

//     TimerLoadSet(SERVO_PWM_MODULE, TIMER_A, (120000000 / 5000) - 1);
//     TimerIntDisable(SERVO_PWM_MODULE, TIMER_IMR_CAE);
//     TimerEnable(SERVO_PWM_MODULE, TIMER_CTL_TEn);

//     TimerControl(SERVO_PWM_MODULE, TIMER_CTL_CC2D, 0);
//     TimerMatchSet(SERVO_PWM_MODULE, TIMER_A, TIMER_MATCH_B, (TimerLoadValueGet(SERVO_PWM_MODULE, TIMER_A) / 2) + 1);

//     TimerControl(SERVO_PWM_MODULE, TIMER_CTL_OUTPUT, TIMER_CTL_OUTPUT_M0PWM4);
// }

// // sets the servo position (in degrees)
// void Servo_SetPosition(uint8_t angle) {
//     uint16_t pulse_width = SERVO_MIN_PULSE_WIDTH + (angle * (SERVO_MAX_PULSE_WIDTH - SERVO_MIN_PULSE_WIDTH)) / 180;

//     TimerMatchSet(SERVO_PWM_MODULE, TIMER_A, TIMER_MATCH_B, pulse_width);
// }
