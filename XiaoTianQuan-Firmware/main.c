#include <atmel_start.h>
#include <util/delay.h>
#include <stdio.h>

#include "motor_controller.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1) {
        printf("test\r\n");
        //_delay_ms(2000);
        printf("test\r\n");
        //_delay_ms(2000);
        printf("test\r\n");
        //_delay_ms(2000);
        printf("test\r\n");
        
        enable_motor(MOTOR_3);
        while (1);
        //_delay_ms(10000);
        //disable_motor();
        //_delay_ms(2000);
	}
}
