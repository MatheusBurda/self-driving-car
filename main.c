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
#define MIN_DIST_TO_FORWARD 30

uint32_t SysClock = 0;
osMutexId_t bestDirMutex = 0;
osMutexId_t distanceMutex = 0;
float distance = 0;
MovementDirection bestDir = M_BACKWARDS;

LookingDirection looking_directions[] =            {L_LEFT,           L_DIAGONAL_LEFT, L_FORWARD,   L_DIAGONAL_RIGHT, L_RIGHT};
MovementDirection movement_directions[] =          {M_LEFT,           M_DIAGONAL_LEFT, M_FORWARD,   M_DIAGONAL_RIGHT, M_RIGHT};
MovementDirection reversed_movement_directions[] = {M_DIAGONAL_RIGHT, M_RIGHT,         M_BACKWARDS, M_LEFT,           M_DIAGONAL_LEFT};
char *movement_directions_str[] = {"M_LEFT", "M_DIAGONAL_LEFT", "M_FORWARD", "M_DIAGONAL_RIGHT", "M_RIGHT"};


void ThreadMeasureDistanceAvg(void *argument) {
    float average;
    float measurement = 0;
    
    while (1) {
        average = 0;
        for (int i = 0; i < 10; i++) {
            measurement = Measure_Echo();
            while (measurement < 2 || measurement > 400) { // sensor ranges between 2 and 400
                measurement = Measure_Echo();
            }

            average += measurement;
            osDelay(50);
        }
        average /= 10;

        osMutexAcquire(distanceMutex, osWaitForever);
        distance = average;
        char string[32];
        snprintf(string, sizeof(string), "Distance: %.2f cm\r\n", distance);
        UARTSendString(string);
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
    float current_distance = 0;
    float max_distance = 0;
    MovementDirection currentBestDir = M_STOP;

    while (1) {
        currentBestDir = M_STOP;

        turnServo(L_FORWARD);
        osMutexAcquire(bestDirMutex, osWaitForever);
        bestDir = M_FORWARD;
        osMutexRelease(bestDirMutex);

        do {
            osMutexAcquire(distanceMutex, osWaitForever);
            current_distance = distance;
            osMutexRelease(distanceMutex);

            osDelay(5000);
        } while (current_distance > MIN_DIST_TO_TURN);

        osMutexAcquire(bestDirMutex, osWaitForever);
        bestDir = M_STOP;
        osMutexRelease(bestDirMutex);

        osDelay(100);

        for (int i = 0; i < sizeof(looking_directions)/sizeof(looking_directions[0]); i++) {
            turnServo(looking_directions[i]);

            osMutexAcquire(distanceMutex, osWaitForever);

            // char string[32];
            // snprintf(string, sizeof(string), "Dir: %d, Distance: %.2f cm\r\n", i, distance);
            // UARTSendString(string);

            if (distance > max_distance) {
                max_distance = distance;
                currentBestDir = movement_directions[i];
            }

            osMutexRelease(distanceMutex);
        }

        if (max_distance < MIN_DIST_TO_TURN) {
            currentBestDir = M_RIGHT;
        }

        osMutexAcquire(bestDirMutex, osWaitForever);
        bestDir = currentBestDir;
        osMutexRelease(bestDirMutex);

        osDelay(200); 

        osMutexAcquire(bestDirMutex, osWaitForever);
        bestDir = M_STOP;
        osMutexRelease(bestDirMutex);

        // for (int i = sizeof(looking_directions) / sizeof(looking_directions[0]) - 1; i >= 0; i--) {
        //     turnServo(looking_directions[i]);

        //     osMutexAcquire(distanceMutex, osWaitForever);

        //     char string[32];
        //     snprintf(string, sizeof(string), "Dir: %d, Distance: %.2f cm\r\n", i, distance);
        //     UARTSendString(string);

        //     if (distance > min_distance) {
        //         min_distance = distance;
        //         // turns to the oposite side 
        //         currentBestDir = reversed_movement_directions[i];
        //     }

        //     osMutexRelease(distanceMutex);
        // }

        // osMutexAcquire(bestDirMutex, osWaitForever);
        // bestDir = currentBestDir;
        // osMutexRelease(bestDirMutex);

        // osDelay(100); 
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

    while (1) {}
}
