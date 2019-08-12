/**
 * \file
 *
 * \brief USART basic driver.
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

/**
 * \defgroup doc_driver_usart_basic USART Basic
 * \ingroup doc_driver_usart
 *
 * \section doc_driver_usart_basic_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <compiler.h>
#include <clock_config.h>
#include <usart_basic.h>
#include <atomic.h>

#include <stdio.h>

#if defined(__GNUC__)

int DBG_USART_printCHAR(char character, FILE *stream)
{
	DBG_USART_write(character);
	return 0;
}

FILE DBG_USART_stream = FDEV_SETUP_STREAM(DBG_USART_printCHAR, NULL, _FDEV_SETUP_WRITE);

#elif defined(__ICCAVR__)

int putchar(int outChar)
{
	USART_0_write(outChar);
	return outChar;
}
#endif

/* Static Variables holding the ringbuffer used in IRQ mode */
static uint8_t          DBG_USART_rxbuf[DBG_USART_RX_BUFFER_SIZE];
static volatile uint8_t DBG_USART_rx_head;
static volatile uint8_t DBG_USART_rx_tail;
static volatile uint8_t DBG_USART_rx_elements;
static uint8_t          DBG_USART_txbuf[DBG_USART_TX_BUFFER_SIZE];
static volatile uint8_t DBG_USART_tx_head;
static volatile uint8_t DBG_USART_tx_tail;
static volatile uint8_t DBG_USART_tx_elements;

void DBG_USART_default_rx_isr_cb(void);
void (*DBG_USART_rx_isr_cb)(void) = &DBG_USART_default_rx_isr_cb;
void DBG_USART_default_udre_isr_cb(void);
void (*DBG_USART_udre_isr_cb)(void) = &DBG_USART_default_udre_isr_cb;

void DBG_USART_default_rx_isr_cb(void)
{
	uint8_t data;
	uint8_t tmphead;

	/* Read the received data */
	data = USART2.RXDATAL;
	/* Calculate buffer index */
	tmphead = (DBG_USART_rx_head + 1) & DBG_USART_RX_BUFFER_MASK;

	if (tmphead == DBG_USART_rx_tail) {
		/* ERROR! Receive buffer overflow */
	} else {
		/* Store new index */
		DBG_USART_rx_head = tmphead;

		/* Store received data in buffer */
		DBG_USART_rxbuf[tmphead] = data;
		DBG_USART_rx_elements++;
	}
}

void DBG_USART_default_udre_isr_cb(void)
{
	uint8_t tmptail;

	/* Check if all data is transmitted */
	if (DBG_USART_tx_elements != 0) {
		/* Calculate buffer index */
		tmptail = (DBG_USART_tx_tail + 1) & DBG_USART_TX_BUFFER_MASK;
		/* Store new index */
		DBG_USART_tx_tail = tmptail;
		/* Start transmission */
		USART2.TXDATAL = DBG_USART_txbuf[tmptail];
		DBG_USART_tx_elements--;
	}

	if (DBG_USART_tx_elements == 0) {
		/* Disable UDRE interrupt */
		USART2.CTRLA &= ~(1 << USART_DREIE_bp);
	}
}

/**
 * \brief Set call back function for DBG_USART
 *
 * \param[in] cb The call back function to set
 *
 * \param[in] type The type of ISR to be set
 *
 * \return Nothing
 */
void DBG_USART_set_ISR_cb(usart_cb_t cb, usart_cb_type_t type)
{
	switch (type) {
	case RX_CB:
		DBG_USART_rx_isr_cb = cb;
		break;
	case UDRE_CB:
		DBG_USART_udre_isr_cb = cb;
		break;
	default:
		// do nothing
		break;
	}
}

/* Interrupt service routine for RX complete */
ISR(USART2_RXC_vect)
{
	if (DBG_USART_rx_isr_cb != NULL)
		(*DBG_USART_rx_isr_cb)();
}

/* Interrupt service routine for Data Register Empty */
ISR(USART2_DRE_vect)
{
	if (DBG_USART_udre_isr_cb != NULL)
		(*DBG_USART_udre_isr_cb)();
}

bool DBG_USART_is_tx_ready()
{
	return (DBG_USART_tx_elements != DBG_USART_TX_BUFFER_SIZE);
}

bool DBG_USART_is_rx_ready()
{
	return (DBG_USART_rx_elements != 0);
}

bool DBG_USART_is_tx_busy()
{
	return (!(USART2.STATUS & USART_TXCIF_bm));
}

/**
 * \brief Read one character from DBG_USART
 *
 * Function will block if a character is not available.
 *
 * \return Data read from the DBG_USART module
 */
uint8_t DBG_USART_read(void)
{
	uint8_t tmptail;

	/* Wait for incoming data */
	while (DBG_USART_rx_elements == 0)
		;
	/* Calculate buffer index */
	tmptail = (DBG_USART_rx_tail + 1) & DBG_USART_RX_BUFFER_MASK;
	/* Store new index */
	DBG_USART_rx_tail = tmptail;
	ENTER_CRITICAL(R);
	DBG_USART_rx_elements--;
	EXIT_CRITICAL(R);

	/* Return data */
	return DBG_USART_rxbuf[tmptail];
}

/**
 * \brief Write one character to DBG_USART
 *
 * Function will block until a character can be accepted.
 *
 * \param[in] data The character to write to the USART
 *
 * \return Nothing
 */
void DBG_USART_write(const uint8_t data)
{
	uint8_t tmphead;

	/* Calculate buffer index */
	tmphead = (DBG_USART_tx_head + 1) & DBG_USART_TX_BUFFER_MASK;
	/* Wait for free space in buffer */
	while (DBG_USART_tx_elements == DBG_USART_TX_BUFFER_SIZE)
		;
	/* Store data in buffer */
	DBG_USART_txbuf[tmphead] = data;
	/* Store new index */
	DBG_USART_tx_head = tmphead;
	ENTER_CRITICAL(W);
	DBG_USART_tx_elements++;
	EXIT_CRITICAL(W);
	/* Enable UDRE interrupt */
	USART2.CTRLA |= (1 << USART_DREIE_bp);
}

/**
 * \brief Initialize USART interface
 * If module is configured to disabled state, the clock to the USART is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 * \retval 0 the USART init was successful
 * \retval 1 the USART init was not successful
 */
int8_t DBG_USART_init()
{

	USART2.BAUD = (uint16_t)USART2_BAUD_RATE(9600); /* set baud rate register */

	USART2.CTRLA = 0 << USART_ABEIE_bp    /* Auto-baud Error Interrupt Enable: disabled */
	               | 0 << USART_DREIE_bp  /* Data Register Empty Interrupt Enable: disabled */
	               | 0 << USART_LBME_bp   /* Loop-back Mode Enable: disabled */
	               | USART_RS485_OFF_gc   /* RS485 Mode disabled */
	               | 1 << USART_RXCIE_bp  /* Receive Complete Interrupt Enable: enabled */
	               | 0 << USART_RXSIE_bp  /* Receiver Start Frame Interrupt Enable: disabled */
	               | 0 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: disabled */

	USART2.CTRLB = 0 << USART_MPCM_bp       /* Multi-processor Communication Mode: disabled */
	               | 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
	               | 1 << USART_RXEN_bp     /* Reciever enable: enabled */
	               | USART_RXMODE_NORMAL_gc /* Normal mode */
	               | 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
	               | 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */

	// USART2.CTRLC = USART_CMODE_ASYNCHRONOUS_gc /* Asynchronous Mode */
	//		 | USART_CHSIZE_8BIT_gc /* Character size: 8 bit */
	//		 | USART_PMODE_DISABLED_gc /* No Parity */
	//		 | USART_SBMODE_1BIT_gc; /* 1 stop bit */

	USART2.DBGCTRL = 1 << USART_DBGRUN_bp; /* Debug Run: enabled */

	// USART2.EVCTRL = 0 << USART_IREI_bp; /* IrDA Event Input Enable: disabled */

	// USART2.RXPLCTRL = 0x0 << USART_RXPL_gp; /* Receiver Pulse Length: 0x0 */

	// USART2.TXPLCTRL = 0x0 << USART_TXPL_gp; /* Transmit pulse length: 0x0 */

	uint8_t x;

	/* Initialize ringbuffers */
	x = 0;

	DBG_USART_rx_tail     = x;
	DBG_USART_rx_head     = x;
	DBG_USART_rx_elements = x;
	DBG_USART_tx_tail     = x;
	DBG_USART_tx_head     = x;
	DBG_USART_tx_elements = x;

#if defined(__GNUC__)
	stdout = &DBG_USART_stream;
#endif

	return 0;
}

/**
 * \brief Enable RX and TX in DBG_USART
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX and TX enable-bits in the USART control register
 *
 * \return Nothing
 */
void DBG_USART_enable()
{
	USART2.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}

/**
 * \brief Enable RX in DBG_USART
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX enable-bit in the USART control register
 *
 * \return Nothing
 */
void DBG_USART_enable_rx()
{
	USART2.CTRLB |= USART_RXEN_bm;
}

/**
 * \brief Enable TX in DBG_USART
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the TX enable-bit in the USART control register
 *
 * \return Nothing
 */
void DBG_USART_enable_tx()
{
	USART2.CTRLB |= USART_TXEN_bm;
}

/**
 * \brief Disable DBG_USART
 * 1. Disables the USART module by clearing the enable-bit(s) in the USART control register
 * 2. If supported by the clock system, disables the clock to the USART
 *
 * \return Nothing
 */
void DBG_USART_disable()
{
	USART2.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
}

/**
 * \brief Get recieved data from DBG_USART
 *
 * \return Data register from DBG_USART module
 */
uint8_t DBG_USART_get_data()
{
	return USART2.RXDATAL;
}
