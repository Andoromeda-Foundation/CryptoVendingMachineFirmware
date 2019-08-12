#include <stdio.h>
#include <atmel_start.h>
#include "motor_controller.h"

static volatile int motor_feedback_enabled;	// true if motor started for at least 100ms

static MotorStatus MotorStatusMapping[] = {
	[MOTOR_1] = MOTOR_OK,
	[MOTOR_2] = MOTOR_OK,
	[MOTOR_3] = MOTOR_OK,
};

static MotorId MotorRunning;

typedef void (*motor_event_callback_t)(MotorEvent);

static motor_event_callback_t motor_event_callback;

static void setup_motor_feedback_interrupt()
{
	//MFB_set_isc(PORT_ISC_BOTHEDGES_gc);
}

void motor_timeout()
{
    MotorStatusMapping[MotorRunning] = MOTOR_TIMEOUT;
    disable_motor();
    if (motor_event_callback) {
        motor_event_callback(MOTOR_EVENT_COMPLETE_TIMEOUT);
    }
}

void setup_motor_interrupt()
{
	setup_motor_feedback_interrupt();
    MOTOR_TIMER_set_timeout_callback(motor_timeout);
    // Timeout 7s, prescalar 1024, clock 5MHz
    // Ttick = 1s / (5Mhz * 1s) * 1024 = 204.8us
    // 7s / 204.8us = 34179.6875
    //TIMER_0_set_timeout(0x1000);
}

void init_motor_controller()
{
	setup_motor_interrupt();
}

static void start_motor_timer()
{
    MOTOR_TIMER_start();
}

static void stop_motor_timer()
{
    MOTOR_TIMER_stop();
}

void enable_motor(MotorId motorId)
{
	disable_motor();
	
	MotorRunning = motorId;
    
    switch (motorId) {
        case MOTOR_1:
            MCTR1_set_level(true);
            break;
        case MOTOR_2:
            MCTR2_set_level(true);
            break;
        case MOTOR_3:
            MCTR3_set_level(true);
            break;
    }
	motor_feedback_enabled = 1;
	start_motor_timer();
    printf("motor %d enabled\r\n", motorId);
}

void disable_motor()
{
	MCTR1_set_level(false);
	MCTR2_set_level(false);
	MCTR3_set_level(false);
	motor_feedback_enabled = 0;
	stop_motor_timer();
}

static volatile int waiting_for_falling_edge;
void process_port_mfb_interrupt()
{
	if (motor_feedback_enabled) {	// delayed 100ms to enable
		bool mfb = MFB_get_level();	// true for rising, false for falling, rising for complete, falling for trigger
		
		if (mfb) {	// rising edge
			waiting_for_falling_edge = 1;
		}
		
		if (!mfb) {	// falling
			if (waiting_for_falling_edge) {
				MotorStatusMapping[MotorRunning] = MOTOR_OK;
				disable_motor();
				if (motor_event_callback) {
					motor_event_callback(MOTOR_EVENT_COMPLETE_OK);
				}
			}
			waiting_for_falling_edge = 0;
		}
	}
}

