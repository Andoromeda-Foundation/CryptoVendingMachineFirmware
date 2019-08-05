/*! \file *********************************************************************
*
* \brief  AVR290: Avoiding TWI Clock Stretching in ATtiny AVRs
*
*
* \par Application note:
*      AVR290:Avoiding TWI Clock Stretching in ATtiny AVRs
*
*
* \author
*      Name : Marv Kausch
*      Atmel Corporation: http://www.atmel.com \n
*      Support email: touch@atmel.com
*
* $Revision: 1.0 $
* $Date: 2011_02_23  $  \n
* Copyright (c) 2011, Atmel Corporation All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of ATMEL may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
* SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "twi-no_stretch-tiny20-drvr.h"

#include "pin.h"


/* General TWI Master status codes */
#define TWI_DATA_INTERRUPT                TWDIF
#define TWI_ADDRESS_STOP_MATCH            TWASIF
#define TWI_BUS_COLLISION                 TWC
#define TWI_BUS_ERROR                     TWBE
#define TWI_SLAVE_STATUS                  TWSSRA
#define TWI_RD_WR_DIR                     TWDIR
#define TWI_SLAVE_CTRLA                   TWSCRA
#define TWI_SLAVE_CTRLB                   TWSCRB
#define TWI_SLAVE_CMD0                    TWCMD0
#define TWI_SLAVE_CMD1                    TWCMD1
#define TWI_SLAVE_DATA                    TWSD
#define TWI_INTERFACE_ENAB                TWEN
#define TWI_SDI_HOLD_EANB                 TWSHE
#define TWI_DATA_INT_ENAB                 TWDIE
#define TWI_ADDR_STOP_INT_ENAB            TWASIE
#define TWI_STOP_INT_ENAB                 TWSIE


void init_twi_slave(void)
{
    TWSA = TWI_ADDR << 1;     // Set TWI slave address.
    TWI_SLAVE_DATA = 0xFF;               // Default content = SDA released.
    TWI_SLAVE_CTRLA = _BV(TWI_INTERFACE_ENAB) | _BV(TWI_SDI_HOLD_EANB) |           // Enable TWI-interface and release TWI pins.
                      _BV(TWI_DATA_INT_ENAB) | _BV(TWI_ADDR_STOP_INT_ENAB) | _BV(TWI_STOP_INT_ENAB);      // Enable TWI interrupts.
}

ISR(TWI_SLAVE_vect) {
    uint8_t status = TWI_SLAVE_STATUS & (_BV(TWI_DATA_INTERRUPT) | _BV(TWI_ADDRESS_STOP_MATCH));
    enable_pin_ready();

    PORTB = _BV(PORTB0);    // Set SCL hi

    if (status & _BV(TWI_DATA_INTERRUPT)) {   // data received (TWDIF)
        if (TWI_SLAVE_STATUS & _BV(TWI_RD_WR_DIR)) {    // Master is reading
            PORTB |= _BV(PORTB2);   // WTH is this?
            TWI_SLAVE_DATA = twi_data_to_master();
            TWI_SLAVE_CTRLB = _BV(TWI_SLAVE_CMD1) | _BV(TWI_SLAVE_CMD0);    // ACK (TWDIR: 1, TWDIF: 1)
        } else {    // Master is writing
            TWI_SLAVE_CTRLB = _BV(TWI_SLAVE_CMD1) | _BV(TWI_SLAVE_CMD0);    // No action (TWDIR: 0, TWDIF: 1)
            twi_data_from_master(TWI_SLAVE_DATA);
        }
    } else if (status & _BV(TWI_ADDRESS_STOP_MATCH)) {   // address matched (TWASIF)
        if (TWI_SLAVE_STATUS & _BV(TWI_BUS_COLLISION)) { // There's a collision detected
            /* Clear the Collision Flag */
            TWI_SLAVE_STATUS = TWI_SLAVE_STATUS;
            /* Wait for any Start Condition */
            TWI_SLAVE_CTRLB = _BV(TWI_SLAVE_CMD1);
        } else {
            /* Address Match */
            if (TWI_SLAVE_STATUS & _BV(TWAS)) { // This is an address detection
                /* Execute ACK and then receive next byte or set TWDIF to send the data */
                TWI_SLAVE_CTRLB = _BV(TWI_SLAVE_CMD1) | _BV(TWI_SLAVE_CMD0);
            }  else {   // Stop condition
                /* Wait for any Start Condition */
                TWI_SLAVE_CTRLB = _BV(TWI_SLAVE_CMD1);
            }
        }
    }
    
    disable_pin_ready();
}

/*! \brief  TWI_bus_error_status function
*
*  Purpose: checks for any TWI bus error\n
*  the TWI Master to write or read the Slave
*  \note  The peripheral must be initialized in advance.
*  no inputs or outputs
*  A twi bus error will result in disabling TWI Interface and re-initializing the TWI Slave
*/

void twi_bus_error_check (void)
{
    if (TWI_SLAVE_STATUS & _BV(TWI_BUS_ERROR))
    {
        /* Clear all Flags */
        TWI_SLAVE_STATUS = TWI_SLAVE_STATUS;
        /* Disable TWI Interface */
        TWSCRA = 0x00u;

        /* Reinitialize TWI Slave */
        init_twi_slave();
    }
}
