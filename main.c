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

#include "src/uart/uart.h"
#include "src/global_variables.h"
#include "src/hcsr04/hcsr04.h"
#include "src/movement/movement.h"
#include "src/servo/servo.h"

uint32_t SysClock = 0;
#define MIN_DIST_TO_TURN 10

LookingDirection looking_directions[] = {L_LEFT, L_DIAGONAL_LEFT, L_FORWARD, L_DIAGONAL_RIGHT, L_RIGHT};
MovementDirection movement_directions[] = {M_LEFT, M_DIAGONAL_LEFT, M_FORWARD, M_DIAGONAL_RIGHT, M_RIGHT};
char *movement_directions_str[] = {"M_LEFT", "M_DIAGONAL_LEFT", "M_FORWARD", "M_DIAGONAL_RIGHT", "M_RIGHT"};

osMessageQueueId_t distanceQueue;
MovementDirection bestDir = M_BACKWARDS ;

void ThreadMeasureDistanceAvg(void *argument) {
    float average;
    while (1) {
        average = 0;
        for (int i = 0; i < 10; i++) {
            Trigger_Ultrasonic();
            average += Measure_Echo();
            osDelay(50);
        }
        average /= 10;

        // char string[32];
        // snprintf(string, sizeof(string), "Distance: %.2f cm\r\n", average);
        // UARTSendString(string);

        osMessageQueuePut(distanceQueue, &average, 0, osWaitForever);

        osDelay(100);
    }
}

void ThreadTurnAround(void *argument) {
    while (1) {
        // char string[32];
        // snprintf(string, sizeof(string), "MOVING %d %s\r\n", (int)bestDir, movement_directions_str[bestDir]);
        // UARTSendString(string);
        switch (bestDir) {
            case M_FORWARD:
                moveForward();
                osDelay(500);
                break;
            case M_BACKWARDS:
                moveBackwards();
                osDelay(500);
                break;
            case M_DIAGONAL_LEFT:
                turnLeft();
                osDelay(250);
                break;
            case M_LEFT:
                turnLeft();
                osDelay(500);
                break;
            case M_DIAGONAL_RIGHT:
                turnRight();
                osDelay(250);
                break;
            case M_RIGHT:
                turnRight();
                osDelay(500);
                break;
            default:
                stop();
                osDelay(500);
                break;
        }

        osDelay(100);
    }
}

void ThreadLookAround(void *argument) {
    float distance;
    float max_distance;

    while (1) {
        distance = 0;
        bestDir = M_STOP;
        max_distance = 0;

        for (int i = 0; i < sizeof(looking_directions) / sizeof(looking_directions[0]); i++) {
            turnServo(looking_directions[i]);
            osDelay(300);
            if (osMessageQueueGet(distanceQueue, &distance, NULL, osWaitForever) == osOK) {
                if (distance > max_distance) {
                    max_distance = distance;
                    bestDir = movement_directions[i];
                }
            }
        }

        if (max_distance <= MIN_DIST_TO_TURN) {
            bestDir = M_BACKWARDS;
        }

        osDelay(100); 

        for (int i = sizeof(looking_directions) / sizeof(looking_directions[0]) - 1; i >= 0; i--) {
            turnServo(looking_directions[i]);
            osDelay(300);
            if (osMessageQueueGet(distanceQueue, &distance, NULL, osWaitForever) == osOK) {
                if (distance > max_distance) {
                    max_distance = distance;
                    bestDir = movement_directions[i];
                }
            }
        }

        if (max_distance <= MIN_DIST_TO_TURN) {
            bestDir = M_BACKWARDS;
        }

        osDelay(100); 
    }
}

int main(void) {
    SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);

    initHCSR04();
    //SetupUart(SysClock);
    servoSetup();
    movementInit();
    
    //UARTSendString("HELLO\r\n");

    osKernelInitialize();

    distanceQueue = osMessageQueueNew(10, sizeof(float), NULL);

    osThreadNew(ThreadMeasureDistanceAvg, NULL, NULL);
    osThreadNew(ThreadLookAround, NULL, NULL);
    osThreadNew(ThreadTurnAround, NULL, NULL);

    osKernelStart();

    while (1) { }
}
