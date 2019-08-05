/*
* motor_controller.c
*
* Created: 7/15/2019 19:15:20
*  Author: Afa
*/

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "motor_controller.h"

static volatile int motor_feedback_enabled;	// true if motor started for at least 100ms

static uint8_t MotorPortMapping[] = {
	[MOTOR_1] = PORT_MCTR1,
	[MOTOR_2] = PORT_MCTR2,
	[MOTOR_3] = PORT_MCTR3,
};

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
	TCCR1A = _BV(COM1A0);	// Enable OC1A/OC1B Compare Match and disable others
	// Timeout 10s, prescalar 1024
	// Ttick = 1s / (8Mhz * 1s) * 1024 = 1us * 1024 = 128us
    // 128us * 60000 = 7.68s
	OCR1A = 60000;
	TIMSK |= _BV(OCIE1A);
}

static void setup_motor_feedback_interrupt()
{
	GIMSK |= _BV(PCIE0);
	PCMSK0 |= _BV(PCINT3);	// PORT_MFB
}

void setup_motor_interrupt()
{
	setup_motor_feedback_interrupt();
	setup_motor_timer_interrupt();
}

void init_motor_controller()
{
	DDRA |= _BV(PORT_MCTR1) | _BV(PORT_MCTR2) | _BV(PORT_MCTR3);
	DDRA &= ~_BV(PORT_MFB);
	
	setup_motor_interrupt();
}

static void start_motor_timer()
{
	TCNT1 = 0;
	TCCR1B |= _BV(CS12) | _BV(CS10);	// clk prescalar 1024
}

static void stop_motor_timer()
{
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

void enable_motor(MotorId motorId)
{
	disable_motor();
	
	MotorRunning = motorId;
	PORTA |= 1 << MotorPortMapping[motorId];
	motor_feedback_enabled = 1;
	start_motor_timer();
}

void disable_motor()
{
	uint8_t mask = 0;
	for (uint8_t i = 0; i < MOTOR_LAST; ++i)
	{
		mask |= _BV(MotorPortMapping[i]);
	}
	PORTA &= ~mask;
	motor_feedback_enabled = 0;
	stop_motor_timer();
}

static volatile int waiting_for_falling_edge;
ISR(PCINT0_vect)
{
	if (motor_feedback_enabled) {	// delayed 100ms to enable
		int mfb = PINA & _BV(PORT_MFB);	// true for rising, false for falling, rising for complete, falling for trigger
		
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

ISR(TIM1_COMPA_vect)	// motor timeout
{
	MotorStatusMapping[MotorRunning] = MOTOR_TIMEOUT;
	disable_motor();
	if (motor_event_callback) {
		motor_event_callback(MOTOR_EVENT_COMPLETE_TIMEOUT);
	}
}
