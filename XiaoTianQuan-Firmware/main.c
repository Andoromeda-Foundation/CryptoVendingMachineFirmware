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

	/* Replace with your application code */
	while (1) {
        enable_motor(MOTOR_3);
        _delay_ms(10000);
        disable_motor();
        _delay_ms(2000);
        printf("PWR: %f\r\n", get_battery_volt());
        start_battery_volt_conversion();
	}
}
