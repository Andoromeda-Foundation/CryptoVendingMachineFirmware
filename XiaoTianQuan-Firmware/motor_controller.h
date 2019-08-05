/*
 * motor_controller.h
 *
 * Created: 7/15/2019 19:14:33
 *  Author: Afa
 */ 


#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

typedef enum {
	MOTOR_1 = 0,
	MOTOR_2 = 1,
	MOTOR_3 = 2,
	MOTOR_LAST = MOTOR_3
} MotorId;

typedef enum {
	MOTOR_OK,
	MOTOR_TIMEOUT,
} MotorStatus;

typedef enum {
	MOTOR_EVENT_COMPLETE_OK,
	MOTOR_EVENT_COMPLETE_TIMEOUT,
} MotorEvent;

void init_motor_controller();
void enable_motor(MotorId motorId);
void disable_motor();

MotorStatus get_motor_status(MotorId motorId);

#endif /* MOTOR_CONTROLLER_H_ */