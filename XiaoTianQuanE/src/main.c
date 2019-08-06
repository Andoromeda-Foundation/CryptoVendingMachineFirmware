/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <ioport.h>
#include <delay.h>

#define PWR IOPORT_CREATE_PIN(PORTD, 0)
#define SW IOPORT_CREATE_PIN(PORTD, 1)

#define F_CPU 16000000

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	
	board_init();


	/* Insert application code here, after the board has been initialized. */
	ioport_init();
	ioport_set_pin_dir(PWR, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(SW, IOPORT_DIR_OUTPUT);
	
	while (1) {
		ioport_set_pin_level(PWR, true);
		delay_ms(440);
		ioport_set_pin_level(SW, true);
		delay_ms(2240);
		ioport_set_pin_level(PWR, false);
		ioport_set_pin_level(SW, false);
		
		delay_s(2);
	}
}
