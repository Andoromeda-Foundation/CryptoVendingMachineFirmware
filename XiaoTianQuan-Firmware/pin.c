/*
 * led.c
 *
 * Created: 7/16/2019 15:12:58
 *  Author: Afa
 */ 

#include "config.h"

#include <avr/io.h>

#include "pin.h"

void init_pins()
{
	DDRA |= _BV(PORT_STATUS);
	DDRA |= _BV(PORT_READY);
}

void enable_pin_status()
{
	PORTA |= _BV(PORT_STATUS);
}

void disable_pin_status()
{
	PORTA &= ~_BV(PORT_STATUS);
}

void enable_pin_ready()
{
	PORTA |= _BV(PORT_READY);
}

void disable_pin_ready()
{
	PORTA &= ~_BV(PORT_READY);
}
