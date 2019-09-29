/*
 * periph.h
 *
 * Created: 2017-04-02 15:25:31
 *  Author: Afa
 */ 


#ifndef PERIPH_H_
#define PERIPH_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

void TWI_Init(void);
bool TWI_Start(uint8_t address);
void TWI_Stop(void);
bool TWI_Write(uint8_t data);
uint8_t TWI_Read_Ack(void);
uint8_t TWI_Read_Nack(void);
uint16_t TWI_Transmit(uint8_t address, uint8_t* data, uint16_t length);
uint16_t TWI_Receive(uint8_t address, uint8_t* data, uint16_t length);
uint16_t TWI_WriteRegister(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);

uint16_t TWI_ReadRegister(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
bool TWI_ReadRegisterU16(uint8_t devaddr, uint8_t regaddr, uint16_t *value);
bool TWI_ReadRegisterS16(uint8_t devaddr, uint8_t regaddr, int16_t *value);

void USART_Init(void);
void USART_SendByte(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_H_ */