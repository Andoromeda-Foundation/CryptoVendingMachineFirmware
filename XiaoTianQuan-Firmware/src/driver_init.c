/**
 * \file
 *
 * \brief Driver initialization.
 *
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/*
 * Code generated by START.
 *
 * This file will be overwritten when reconfiguring your START project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <system.h>

/* Configure pins and initialize registers */
void ADC_0_initialization(void)
{

	// Disable digital input buffer
	VBAT_MONITOR_set_isc(PORT_ISC_INPUT_DISABLE_gc);
	// Disable pull-up resistor
	VBAT_MONITOR_set_pull_mode(PORT_PULL_OFF);

	ADC_0_init();
}

/* configure the pins and initialize the registers */
void USART_0_initialization(void)
{

	// Set pin direction to input
	APP_RXD_set_dir(PORT_DIR_IN);

	APP_RXD_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	// Set pin direction to output
	APP_TXD_set_dir(PORT_DIR_OUT);

	APP_TXD_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	USART_0_init();
}

/* configure the pins and initialize the registers */
void USART_1_initialization(void)
{

	// Set pin direction to input
	DBG_RXD_set_dir(PORT_DIR_IN);

	DBG_RXD_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	/* set the alternate pin mux */

	PORTMUX.USARTROUTEA |= PORTMUX_USART20_bm;

	// Set pin direction to output
	DBG_TXD_set_dir(PORT_DIR_OUT);

	DBG_TXD_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	/* set the alternate pin mux */

	PORTMUX.USARTROUTEA |= PORTMUX_USART20_bm;

	USART_1_init();
}

void TIMER_0_initialization(void)
{

	TIMER_0_init();
}

void TIMER_1_initialization(void)
{

	TIMER_1_init();
}

void EVENT_SYSTEM_0_initialization(void)
{

	EVENT_SYSTEM_0_init();
}

/* configure pins and initialize registers */
void I2C_0_initialization(void)
{

	SCL_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	SCL_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	SCL_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	SCL_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	SCL_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	/* set the alternate pin mux */

	PORTMUX.TWISPIROUTEA |= PORTMUX_TWI00_bm;

	SDA_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	SDA_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	SDA_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	SDA_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	SDA_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	/* set the alternate pin mux */

	PORTMUX.TWISPIROUTEA |= PORTMUX_TWI00_bm;

	I2C_0_init();
}

/**
 * \brief System initialization
 */
void system_init()
{
	mcu_init();

	/* PORT setting on PA2 */

	// Set pin direction to output
	APP_PWR_set_dir(PORT_DIR_OUT);

	APP_PWR_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    true);

	/* PORT setting on PA3 */

	// Set pin direction to output
	RDY_set_dir(PORT_DIR_OUT);

	RDY_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	/* PORT setting on PA4 */

	// Set pin direction to output
	STATUS_set_dir(PORT_DIR_OUT);

	STATUS_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    true);

	/* PORT setting on PA5 */

	BTN_APP_PWR_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	BTN_APP_PWR_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	BTN_APP_PWR_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	BTN_APP_PWR_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	BTN_APP_PWR_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_FALLING_gc);

	/* PORT setting on PA6 */

	BTN_ROTATE_M_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	BTN_ROTATE_M_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	BTN_ROTATE_M_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	BTN_ROTATE_M_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	BTN_ROTATE_M_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_FALLING_gc);

	/* PORT setting on PC0 */

	// Set pin direction to output
	MCTR1_set_dir(PORT_DIR_OUT);

	MCTR1_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	/* PORT setting on PC1 */

	// Set pin direction to output
	MCTR2_set_dir(PORT_DIR_OUT);

	MCTR2_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	/* PORT setting on PC2 */

	// Set pin direction to output
	MCTR3_set_dir(PORT_DIR_OUT);

	MCTR3_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	/* PORT setting on PC3 */

	MFB_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	MFB_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	MFB_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	MFB_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	MFB_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_FALLING_gc);

	CLKCTRL_init();

	RTC_0_init();

	VREF_0_init();

	ADC_0_initialization();

	USART_0_initialization();

	USART_1_initialization();

	TIMER_0_initialization();

	TIMER_1_initialization();

	EVENT_SYSTEM_0_initialization();

	CPUINT_init();

	SLPCTRL_init();

	I2C_0_initialization();

	BOD_init();
}
