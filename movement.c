#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// Definições dos pinos
#define MOTOR_PIN1 GPIO_PIN_0 // K0
#define MOTOR_PIN2 GPIO_PIN_1 // K1
#define MOTOR_PIN3 GPIO_PIN_2 // K2
#define MOTOR_PIN4 GPIO_PIN_3 // K3

// Inicializa o Port K
void PortK_Init(void) {
    // Habilita o port K
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    
    // Espera até que o port K esteja pronto
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK));
    
    // Configura os pinos K0-K3 como saídas
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4);
}

// Função para andar reto
void AndarReto(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, MOTOR_PIN1 | MOTOR_PIN3);
}

// Função para parar
void Parar(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, 0);
}

// Função para virar à direita
void VirarDireita(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, MOTOR_PIN1 | MOTOR_PIN4);
}

// Função para virar à esquerda
void VirarEsquerda(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, MOTOR_PIN2 | MOTOR_PIN3);
}

// Função para andar de ré
void AndarRe(void) {
    GPIOPinWrite(GPIO_PORTK_BASE, MOTOR_PIN1 | MOTOR_PIN2 | MOTOR_PIN3 | MOTOR_PIN4, MOTOR_PIN2 | MOTOR_PIN4);
}

int run(void) {
    // Configura a velocidade do sistema
    uint32_t SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);

    
    // Inicializa o port K
    PortK_Init();
    
    // Exemplo de uso das funções
    AndarReto();
    SysCtlDelay(SysClock/2); // Anda reto por 1 segundo
    Parar();
    SysCtlDelay(SysClock/2); // Para por 1 segundo
    VirarDireita();
    SysCtlDelay(SysClock/2); // Vira à direita por 1 segundo
	VirarEsquerda();
    SysCtlDelay(SysClock/2); // Vira à direita por 1 segundo
    Parar();
    SysCtlDelay(SysClock/2); // Para por 1 segundo
    AndarRe();
    SysCtlDelay(SysClock/2); // Anda de ré por 1 segundo
    Parar();

    while(1) {
        // Loop infinito para manter o programa em execução
    }

    return 0;
}
