/*
 * XiaoTianQuan-Firmware.c
 *
 * Created: 7/15/2019 18:19:23
 * Author : Afa
 */ 

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pin.h"
#include "motor_controller.h"
#include "twi-no_stretch-tiny20-drvr.h"

static void init_clock()
{
	CCP = 0xD8; // write access signature to Configuration Change Protection Register
	CLKPSR = 0x00; // Prescaler 1
}

int main(void)
{
	init_clock();

	init_pins();
	init_motor_controller();
	//init_twi_slave();

    //enable_pin_status();

	sei();
	//
    //DDRA &= ~(1 << PORTA7);
    //DDRB &= ~(1 << PORTB2);
    
    /* Replace with your application code */
    while (1) 
    {
        if (PINA & _BV(PINA7)) {
            enable_pin_status();
        } else {
            disable_pin_status();
        }
        
        if (PINB & _BV(PINB1)) {
            enable_pin_ready();
        } else {
            disable_pin_ready();
        }
        
        //twi_bus_error_check();
		//enable_motor(MOTOR_2);
		//_delay_ms(100);
	}
}

unsigned char twi_data_to_master(void)
{
    return 0;
}

static int on;
void twi_data_from_master(unsigned char data)
{
    //if (on) {
        //enable_pin_ready();
    //} else {
    //}
}