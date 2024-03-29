#include <stdio.h>
#include <atmel_start.h>
#include "motor_controller.h"

void MFB_set_change_callback(void (*cb)(void)); // defined in isr
void process_port_mfb_interrupt();

static volatile int motor_feedback_enabled;	// true if motor started for at least 100ms

static MotorStatus MotorStatusMapping[] = {
    [MOTOR_1] = MOTOR_OK,
    [MOTOR_2] = MOTOR_OK,
    [MOTOR_3] = MOTOR_OK,
};

static MotorId MotorRunning;

typedef void (*motor_event_callback_t)(MotorEvent);

static motor_event_callback_t motor_event_callback;

static void motor_event(MotorStatus s) 
{
    RDY_set_level(true);
    
    if (motor_event_callback) {
        motor_event_callback(s);
    }
}

static void setup_motor_feedback_interrupt()
{
    MFB_set_change_callback(process_port_mfb_interrupt);
}

void set_motor_event_callback(motor_event_callback_t cb)
{
    motor_event_callback = cb;
}

void motor_timeout()
{
    MotorStatusMapping[MotorRunning] = MOTOR_TIMEOUT;
    disable_motor();
    motor_event(MOTOR_EVENT_COMPLETE_TIMEOUT);
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

MotorStatus get_motor_status(MotorId motorId)
{
    return MotorStatusMapping[motorId];
}

void process_port_mfb_interrupt()
{
    if (motor_feedback_enabled) {	// when motor enabled
        MotorStatusMapping[MotorRunning] = MOTOR_OK;
        disable_motor();
        motor_event(MOTOR_EVENT_COMPLETE_OK);
    }
}

