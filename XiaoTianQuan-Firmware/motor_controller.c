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

static void setup_motor_timer_interrupt()
{
	//TCCR1A = _BV(COM1A0);	// Enable OC1A/OC1B Compare Match and disable others
	//// Timeout 10s, prescalar 1024
	//// Ttick = 1s / (8Mhz * 1s) * 1024 = 1us * 1024 = 128us
    //// 128us * 60000 = 7.68s
	//OCR1A = 60000;
	//TIMSK |= _BV(OCIE1A);
}

static void setup_motor_feedback_interrupt()
{
	MFB_set_isc(PORT_ISC_BOTHEDGES_gc);
}

void setup_motor_interrupt()
{
	setup_motor_feedback_interrupt();
	setup_motor_timer_interrupt();
}

void init_motor_controller()
{
	setup_motor_interrupt();
}

static void start_motor_timer()
{
    //TIMER_0_init();
    //TIMER_0_start();
	//TCNT1 = 0;
	//TCCR1B |= _BV(CS12) | _BV(CS10);	// clk prescalar 1024
}

static void stop_motor_timer()
{
	//TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

void enable_motor(MotorId motorId)
{
    printf("enabling motor %d...\n", motorId);
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
    printf("motor enabled\n");
}

void disable_motor()
{
    printf("disabling motor...\n");
	MCTR1_set_level(false);
	MCTR2_set_level(false);
	MCTR3_set_level(false);
	motor_feedback_enabled = 0;
	stop_motor_timer();
    printf("motor disabled\n");
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

//ISR(TIM1_COMPA_vect)	// motor timeout
//{
	//MotorStatusMapping[MotorRunning] = MOTOR_TIMEOUT;
	//disable_motor();
	//if (motor_event_callback) {
		//motor_event_callback(MOTOR_EVENT_COMPLETE_TIMEOUT);
	//}
//}
