#ifndef MOVEMENT_H
#define MOVEMENT_H

#define MOTOR_PIN1 GPIO_PIN_0 // K0
#define MOTOR_PIN2 GPIO_PIN_1 // K1
#define MOTOR_PIN3 GPIO_PIN_2 // K2
#define MOTOR_PIN4 GPIO_PIN_3 // K3

typedef enum MovementDirection {
    M_LEFT = 0,
    M_DIAGONAL_LEFT, 
    M_FORWARD ,
    M_DIAGONAL_RIGHT, 
    M_RIGHT,
    M_BACKWARDS,
    M_STOP
} MovementDirection;


extern void movementInit(void);
extern void moveForward(void);
extern void stop(void);
extern void turnRight(void);
extern void turnLeft(void);
extern void moveBackwards(void);

#endif