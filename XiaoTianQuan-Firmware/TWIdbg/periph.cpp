/*
* periph.cpp
*
* Created: 2017-04-02 15:24:29
*  Author: Afa
*/

#include "config.h"

#include <stdio.h>

#include <avr/io.h>
#include <util/setbaud.h>
#include <util/twi.h>

#include "periph.h"

inline void loop_until_TWINT()
{
    loop_until_bit_is_set(TWCR, TWINT);
}

void TWI_Init(void)
{
    TWSR = 0;
    TWBR = 0x0A;

    TWCR = (1 << TWEN);
}

bool TWI_Start(uint8_t address)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    loop_until_TWINT();

    if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START) {
        return false;
    }

    TWDR = address;

    TWCR = (1 << TWINT) | (1 << TWEN);
    
    loop_until_TWINT();

    if ((TW_STATUS != TW_MT_SLA_ACK ) && (TW_STATUS != TW_MR_SLA_ACK)) {
        return false;
    }

    return true;
}

bool TWI_Write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    loop_until_TWINT();
    
    if (TW_STATUS != TW_MT_DATA_ACK) {
        return false;
    }
    
    return true;
}

uint8_t TWI_Read_Ack(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    loop_until_TWINT();

    return TWDR;
}

uint8_t TWI_Read_Nack(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    loop_until_TWINT();

    return TWDR;
}

void TWI_Stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

uint16_t TWI_Transmit(uint8_t address, uint8_t* data, uint16_t length)
{
    uint16_t written = 0;
    if (!TWI_Start(address | TW_WRITE)) {
        return 0;
    }

    for (uint16_t i = 0; i < length; ++i) {
        if (!TWI_Write(data[i])) {
            goto twi_transmit_cleanup;
        }
        ++written;
    }

    twi_transmit_cleanup:
    TWI_Stop();

    return written;
}

uint16_t TWI_Receive(uint8_t address, uint8_t* data, uint16_t length)
{
    if (!TWI_Start(address | TW_READ)) {
        return 0;
    }

    for (uint16_t i = 0; i < length - 1; ++i) {
        data[i] = TWI_Read_Ack();
    }

    data[length - 1] = TWI_Read_Nack();

    TWI_Stop();
    return length;
}

uint16_t TWI_WriteRegister(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
    uint16_t written = 0;
    if (!TWI_Start(devaddr | TW_WRITE)) {
        return 0;
    }

    TWI_Write(regaddr);

    for (uint16_t i = 0; i < length; ++i) {
        if (!TWI_Write(data[i])) {
            goto twi_writeregister_cleanup;
        }
        ++written;
    }

    twi_writeregister_cleanup:
    TWI_Stop();
    return written;
}

uint16_t TWI_ReadRegister(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
    if (!TWI_Start(devaddr | TW_WRITE)) {
        return 0;
    }

    TWI_Write(regaddr);

    if (!TWI_Start(devaddr | TW_READ)) {
        return 0;
    }

    for (uint16_t i = 0; i < length - 1; ++i) {
        data[i] = TWI_Read_Ack();
    }

    data[length - 1] = TWI_Read_Nack();

    TWI_Stop();
    return length;
}

bool TWI_ReadRegisterU16(uint8_t devaddr, uint8_t regaddr, uint16_t *value)
{
    uint8_t val[2];
    if (TWI_ReadRegister(devaddr, regaddr, val, 2) == 2) {
        *value = ((uint16_t)val[0] << 8) | val[1];
        return true;
    }

    return false;
}

bool TWI_ReadRegisterS16(uint8_t devaddr, uint8_t regaddr, int16_t *value)
{
    uint8_t val[2];
    if (TWI_ReadRegister(devaddr, regaddr, val, 2) == 2) {
        *value = (uint16_t)(val[0] << 8) | val[1];
        return true;
    }

    return false;
}

// USART

void USART_Init(void)
{
    UBRR0L = UBRRL_VALUE;
    UBRR0H = UBRRH_VALUE;

    #if USE_2X
    UCSR0A |= (1 << U2X0);
    #else
    UCSR0A &= ~(1 << U2X0);
    #endif

    UCSR0B = ((1 << TXEN0) | (1 << RXEN0));
}

void USART_SendByte(uint8_t data)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    //while((UCSRA &(1<<UDRE)) == 0) {
    //}

    UDR0 = data;
}
