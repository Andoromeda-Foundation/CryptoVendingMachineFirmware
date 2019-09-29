/*
* TWIBase.h
*
* Created: 2017-05-15 17:00:29
* Author: Afa
*/


#ifndef __TWIBASE_H__
#define __TWIBASE_H__

#include <stdint.h>

class TWIBase
{
public:
    TWIBase(uint8_t addr) : _address(addr) { }
    // Seems virtual destructor is not supported

protected:    
    bool readS16(uint8_t reg, int16_t *val);
    bool readU16(uint8_t reg, uint16_t *val);
    bool readU8(uint8_t reg, uint8_t *val);
    bool readS8(uint8_t reg, uint8_t *val);
    bool writeU8(uint8_t reg, uint8_t val);

    uint8_t _address;   // Address of the TWI device
};

#endif
