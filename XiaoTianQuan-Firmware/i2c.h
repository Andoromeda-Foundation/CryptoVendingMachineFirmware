/*
 * i2c.h
 *
 * Created: 8/15/2019 17:55:26
 *  Author: Afa
 */ 


#ifndef I2C_H_
#define I2C_H_

 enum 
{
    I2C_REG_RC_0 = 0x10,
    I2C_REG_RC_1 = 0x11,
    I2C_REG_RC_2 = 0x12,
    I2C_REG_RC_3 = 0x13,
    I2C_REG_RC_4 = 0x14,
    I2C_REG_RC_5 = 0x15,
    I2C_REG_RC_6 = 0x16,
    I2C_REG_RC_7 = 0x17,
    I2C_REG_RC_8 = 0x18,
    I2C_REG_RC_9 = 0x19,
    I2C_REG_RC_A = 0x1A,
    I2C_REG_RC_B = 0x1B,
    I2C_REG_RC_C = 0x1C,
    I2C_REG_RC_D = 0x1D,
    I2C_REG_RC_E = 0x1E,
    I2C_REG_RC_F = 0x1F,
    
    I2C_REG_RS_0 = 0x20,
    I2C_REG_RS_1 = 0x21,
    I2C_REG_RS_2 = 0x22,
    I2C_REG_RS_3 = 0x23,
    I2C_REG_RS_4 = 0x24,
    I2C_REG_RS_5 = 0x25,
    I2C_REG_RS_6 = 0x26,
    I2C_REG_RS_7 = 0x27,
    I2C_REG_RS_8 = 0x28,
    I2C_REG_RS_9 = 0x29,
    I2C_REG_RS_A = 0x2A,
    I2C_REG_RS_B = 0x2B,
    I2C_REG_RS_C = 0x2C,
    I2C_REG_RS_D = 0x2D,
    I2C_REG_RS_E = 0x2E,
    I2C_REG_RS_F = 0x2F,

    I2C_REG_RSS = 0x30,
    I2C_REG_RE = 0x31,

    I2C_REG_PWR = 0x80,
    I2C_REG_BAT = 0x81,
    I2C_REG_BATL = I2C_REG_BAT,
    I2C_REG_BATH = 0x82,
};

void initialize_i2c();

#endif /* I2C_H_ */