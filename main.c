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

#include "src/movement/movement.h"
#include "src/uart/uart.h"
#include "src/servo/servo.h"
// Defini��es dos pinos
#define TRIG_PIN GPIO_PIN_0 // PM0
#define ECHO_PIN GPIO_PIN_1 // PM1

uint32_t SysClock;
float distance;

void PortM_Init(void) {
    // Habilita o port M
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    
    // Espera at� que o port M esteja pronto
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM));
    
    // Configura o pino TRIG como sa�da
    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, TRIG_PIN);
    
    // Configura o pino ECHO como entrada
    GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, ECHO_PIN);
}

void Timer0_Init(void) {
    // Habilita o Timer 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    
    // Configura o Timer 0 como temporizador de 32 bits em modo peri�dico
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
}

void delayMicroseconds(uint32_t us) {
    // Calcula o n�mero de ciclos de clock para o atraso desejado
    uint32_t delay = (SysClock) / us;
    SysCtlDelay(delay);
}

void MeasureDistance(void *argument) {
	while(true) {
    uint32_t duration;
    
    // Envia um pulso de 10us no pino TRIG
    GPIOPinWrite(GPIO_PORTM_BASE, TRIG_PIN, TRIG_PIN);
    delayMicroseconds(12000000);
    GPIOPinWrite(GPIO_PORTM_BASE, TRIG_PIN, 0);
    
    // Espera pelo in�cio do pulso no pino ECHO
    while(GPIOPinRead(GPIO_PORTM_BASE, ECHO_PIN) == 0);
    
    // Inicia o timer
    TimerLoadSet(TIMER0_BASE, TIMER_A, 0xFFFFFFFF);
    TimerEnable(TIMER0_BASE, TIMER_A);
    
    // Espera pelo fim do pulso no pino ECHO
    while(GPIOPinRead(GPIO_PORTM_BASE, ECHO_PIN) != 0);
    
    // Para o timer
    duration = TimerValueGet(TIMER0_BASE, TIMER_A);
    TimerDisable(TIMER0_BASE, TIMER_A);
    
    // Calcula a dist�ncia em cent�metros
    distance = duration;
    // Adiciona um pequeno atraso antes da pr�xima medi��o
    SysCtlDelay(SysClock/50); // 1 segundo
	}
}

// int main(void) {
//     uint32_t distance;
    
//     // Configura a velocidade do sistema
// 		SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);
    
//     // Inicializa o port M, o Timer 0 e a UART0
//     PortM_Init();
//     Timer0_Init();
// 		SetupUart(SysClock);
    
		
//     //osKernelInitialize();
//     char string[60];
// 		//osThreadNew(MeasureDistance, NULL, NULL);
	
// 		//osKernelStart();
//     while(1) {
//         // Escreve a dist�ncia na UART
//         snprintf(string, sizeof(string), "distance: %f \r\n", distance);
//         UARTSendString(string);
    

//     }
//   return 0;
// }

int main(void) {
    SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);
    
    servoInit(SysClock);
    servoSetPosition(90);
    while(1) {
          servoSetPosition(90);
        //  servoSetPosition(0);
        //  delayMicroseconds(1000);
        //  servoSetPosition(45);
        //  delayMicroseconds(1000);
        //  servoSetPosition(90);
        //  delayMicroseconds(1000);
        //  servoSetPosition(135);
        //  delayMicroseconds(1000);
        //  servoSetPosition(180);
        //  delayMicroseconds(1000);
    }

    return 0;
}        


