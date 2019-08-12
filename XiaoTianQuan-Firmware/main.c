#define F_CPU 5000000

#include <atmel_start.h>
#include <util/delay.h>

#include "motor_controller.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
    init_motor_controller();

	/* Replace with your application code */
	while (1) {
        enable_motor(MOTOR_3);
        _delay_ms(10000);
        disable_motor();
        _delay_ms(2000);
	}
}
