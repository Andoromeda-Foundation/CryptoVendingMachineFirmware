/**
 * \file
 *
 * \brief Driver ISR.
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

#include <driver_init.h>
#include <compiler.h>

#include "tca.h"

ISR(RTC_CNT_vect)
{
	/* Insert your RTC Overflow interrupt handling code */

	/* Insert your RTC Compare interrupt handling code */

	/* Overflow and Compare interrupt flags have to cleared manually */
	RTC.INTFLAGS = RTC_OVF_bm | RTC_CMP_bm;
}

void (*tca0_cmp0_cb)(void) = NULL;

void MOTOR_TIMER_set_timeout_callback(void (*cb)(void))
{
    tca0_cmp0_cb = cb;    
}

ISR(TCA0_CMP0_vect)
{
	/* Insert your TCA Compare 0 Interrupt handling code here */
    if (tca0_cmp0_cb) {
        tca0_cmp0_cb();
    }

	/* The interrupt flag has to be cleared manually */
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

ISR(PORTA_PORT_vect)
{
	/* Insert your PORTA interrupt handling code here */
    // bat m
    
	/* Clear interrupt flags */
	VPORTA_INTFLAGS = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);
}

void (*mfb_callback)(void) = NULL;

void MFB_set_change_callback(void (*cb)(void)) {
    mfb_callback = cb;
}

ISR(PORTC_PORT_vect)
{
	/* Insert your PORTC interrupt handling code here */
    if (VPORTC_INTFLAGS & (1 << 3)) {   // MFB
        if (mfb_callback) {
            mfb_callback();
        }
    }   
         
	/* Clear interrupt flags */
	VPORTC_INTFLAGS = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
}
