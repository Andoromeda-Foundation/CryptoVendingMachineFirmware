/* 
* TWIBase.cpp
*
* Created: 2017-05-15 17:00:28
* Author: Afa
*/

#include "TWIBase.h"

#include "periph.h"

bool TWIBase::readS16(uint8_t reg, int16_t *val)
{
    return TWI_ReadRegisterS16(_address, reg, val);
}

bool TWIBase::readU16(uint8_t reg, uint16_t *val)
{
    return TWI_ReadRegisterU16(_address, reg, val);
}

bool TWIBase::readU8(uint8_t reg, uint8_t *val)
{
    return TWI_ReadRegister(_address, reg, val, 1);
}

bool TWIBase::writeU8(uint8_t reg, uint8_t val)
{
    return TWI_WriteRegister(_address, reg, &val, 1);
}