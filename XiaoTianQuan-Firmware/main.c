#define F_CPU 5000000

#include <atmel_start.h>
#include <util/delay.h>
#include <stdio.h>

#include "motor_controller.h"
#include "batadc.h"
#include "i2c.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
    initialize_batadc();
    init_motor_controller();
    initialize_i2c();

    start_battery_volt_conversion();
        
    /* Replace with your application code */
	while (1) {
        if (TWI0.SSTATUS) {}
	}
}
