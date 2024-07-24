#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "cmsis_os2.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "src/uart/uart.h"
#include "src/global_variables.h"
#include "src/hcsr04/hcsr04.h"
#include "src/movement/movement.h"
#include "src/servo/servo.h"


#define MIN_DIST_TO_TURN 10

uint32_t SysClock = 0;
int bestDirMutex = 0;
int distanceMutex = 0;
float distance = 0;
MovementDirection bestDir = M_BACKWARDS;

LookingDirection looking_directions[] = {L_LEFT, L_DIAGONAL_LEFT, L_FORWARD, L_DIAGONAL_RIGHT, L_RIGHT};
MovementDirection movement_directions[] = {M_LEFT, M_DIAGONAL_LEFT, M_FORWARD, M_DIAGONAL_RIGHT, M_RIGHT};
char *movement_directions_str[] = {"M_LEFT", "M_DIAGONAL_LEFT", "M_FORWARD", "M_DIAGONAL_RIGHT", "M_RIGHT"};


void ThreadMeasureDistanceAvg(void *argument) {
    float average;
    
    while (1) {
        average = 0;
        for (int i = 0; i < 10; i++) {
            Trigger_Ultrasonic();
            average += Measure_Echo();
            osDelay(10);
        }
        average /= 10;

        char string[32];
        snprintf(string, sizeof(string), "Distance: %.2f cm\r\n", average);
        UARTSendString(string);

        osMutexAcquire(distanceMutex, osWaitForever);
        distance = average;
        osMutexRelease(distanceMutex);

    }
}


void ThreadTurnAround(void *argument) {
    uint32_t delay = 0;
    while (1) {
        delay = 100;
        
        osMutexAcquire(bestDirMutex, osWaitForever);
        switch (bestDir) {
            case M_FORWARD:
                moveForward();
                delay += 500;
                break;
            case M_BACKWARDS:
                moveBackwards();
                delay += 500;
                break;
            case M_DIAGONAL_LEFT:
                turnLeft();
                delay += 250;
                break;
            case M_LEFT:
                turnLeft();
                delay += 500;
                break;
            case M_DIAGONAL_RIGHT:
                turnRight();
                delay += 250;
                break;
            case M_RIGHT:
                turnRight();
                delay += 500;
                break;
            default:
                stop();
                delay += 500;
                break;
        }
        osMutexRelease(bestDirMutex);

        osDelay(delay);

    }
}


void ThreadLookAround(void *argument) {
    float max_distance;
    MovementDirection currentBestDir = M_STOP;

    while (1) {
        currentBestDir = M_STOP;
        max_distance = 0;

        for (int i = 0; i < sizeof(looking_directions) / sizeof(looking_directions[0]); i++) {
            turnServo(looking_directions[i]);
            osDelay(200);
            osMutexAcquire(distanceMutex, osWaitForever);
            if (distance > max_distance) {
                max_distance = distance;
                currentBestDir = movement_directions[i];
            }
            osMutexRelease(distanceMutex);
        }

        if (max_distance <= MIN_DIST_TO_TURN) {
            currentBestDir = M_BACKWARDS;
        }

        osMutexAcquire(bestDirMutex, osWaitForever);
        bestDir = currentBestDir;
        osMutexRelease(bestDirMutex);

        osDelay(100); 

        for (int i = sizeof(looking_directions) / sizeof(looking_directions[0]) - 1; i >= 0; i--) {
            turnServo(looking_directions[i]);
            osDelay(200);
            osMutexAcquire(distanceMutex, osWaitForever);
            if (distance > max_distance) {
                max_distance = distance;
                currentBestDir = movement_directions[i];
            }
            osMutexRelease(distanceMutex);
        }

        if (max_distance <= MIN_DIST_TO_TURN) {
            currentBestDir = M_BACKWARDS;
        }

        osMutexAcquire(bestDirMutex, osWaitForever);
        bestDir = currentBestDir;
        osMutexRelease(bestDirMutex);

        osDelay(100); 
    }
}


int main(void) {
    SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);

    initHCSR04();
    SetupUart(SysClock);
    servoSetup();
    movementInit();
    
    UARTSendString("HELLO\r\n");

    osKernelInitialize();

    bestDirMutex = osMutexNew(NULL);
    distanceMutex = osMutexNew(NULL);

    osThreadNew(ThreadMeasureDistanceAvg, NULL, NULL);
    osThreadNew(ThreadLookAround, NULL, NULL);
    osThreadNew(ThreadTurnAround, NULL, NULL);

    osKernelStart();

    while (1) { }
}
