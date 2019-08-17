/*
* i2c.c
*
* Created: 8/15/2019 17:32:05
*  Author: Afa
*/

#include <atmel_start.h>

#include "batadc.h"
#include "motor_controller.h"
#include "i2c.h"

static volatile uint8_t _currentRegister = 0x00;
static volatile uint8_t volatile _writeArray[2] = {};
static volatile uint8_t _writeIndex = 0;
static volatile float _batvolt = 0.f;
static volatile bool _batvoltvalid = false;

static volatile uint8_t _regRSS;
static volatile uint8_t _regPWR;

static uint8_t r_prs(uint8_t offset)
{
    if (offset * 8 > MOTOR_LAST) {
        return 0;
    }
    
    uint8_t ret = 0;
    
    for (MotorId i = offset * 8; i <= MOTOR_LAST; ++i) {
        MotorStatus s = get_motor_status(i);
        if (s != MOTOR_OK) {
            ret |= 1 << i;
        }
    }
    return ret;
}

static uint8_t r_re()
{
    if (_regRSS > MOTOR_LAST) {
        return 0x00;
    }
    
    return get_motor_status(_regRSS);
}

static uint8_t r_bat(uint8_t idx)
{
    if (!_batvoltvalid) {
        _batvolt = get_battery_volt();
        _batvoltvalid = true;
    }
    
    if (idx == 0) {
        return ((uint8_t *)&_batvolt)[0];
    }
    
    if (idx == 1) {
        return ((uint8_t *)&_batvolt)[1];
    }
    
    return 0;
}

static void ctrl_rc(uint8_t offset, uint8_t value)
{
    if (offset * 8 > MOTOR_LAST)
        return;
        
    int lsb = __builtin_ffs(value);
    if (lsb == 0) {
        return;
    } 
    
    int motorId = offset * 8 + lsb - 1;
    enable_motor(motorId);
}

static void ctrl_pwr(uint8_t value)
{
    _regPWR = value & 0b00000001;
    if (_regPWR & 0b00000001) { // App power
        APP_POWER_set_level(true);
    } else {
        APP_POWER_set_level(false);
    }
}

static void process_write()
{
    switch (_currentRegister) {
        case I2C_REG_RC_0:
        case I2C_REG_RC_1:
        case I2C_REG_RC_2:
        case I2C_REG_RC_3:
        case I2C_REG_RC_4:
        case I2C_REG_RC_5:
        case I2C_REG_RC_6:
        case I2C_REG_RC_7:
        case I2C_REG_RC_8:
        case I2C_REG_RC_9:
        case I2C_REG_RC_A:
        case I2C_REG_RC_B:
        case I2C_REG_RC_C:
        case I2C_REG_RC_D:
        case I2C_REG_RC_E:
        case I2C_REG_RC_F:
        ctrl_rc(_currentRegister - I2C_REG_RC_0, _writeArray[0]);
        break;
        
        // RS: read only
        
        case I2C_REG_RSS:
        break;
        
        // RE: read only
                
        case I2C_REG_PWR:
        ctrl_pwr(_writeArray[0]);
        break;
        
        // BAT: read only
        
        default:
        break;
    }
}

static void i2c_read_handler()
{
    switch (_currentRegister) {
        case I2C_REG_RC_0:
        case I2C_REG_RC_1:
        case I2C_REG_RC_2:
        case I2C_REG_RC_3:
        case I2C_REG_RC_4:
        case I2C_REG_RC_5:
        case I2C_REG_RC_6:
        case I2C_REG_RC_7:
        case I2C_REG_RC_8:
        case I2C_REG_RC_9:
        case I2C_REG_RC_A:
        case I2C_REG_RC_B:
        case I2C_REG_RC_C:
        case I2C_REG_RC_D:
        case I2C_REG_RC_E:
        case I2C_REG_RC_F:
        I2C_SLAVE_write(0x00);  // Write only
        break;
        
        case I2C_REG_RS_0:
        case I2C_REG_RS_1:
        case I2C_REG_RS_2:
        case I2C_REG_RS_3:
        case I2C_REG_RS_4:
        case I2C_REG_RS_5:
        case I2C_REG_RS_6:
        case I2C_REG_RS_7:
        case I2C_REG_RS_8:
        case I2C_REG_RS_9:
        case I2C_REG_RS_A:
        case I2C_REG_RS_B:
        case I2C_REG_RS_C:
        case I2C_REG_RS_D:
        case I2C_REG_RS_E:
        case I2C_REG_RS_F:
        I2C_SLAVE_write(r_prs(_currentRegister - 0x10));
        break;
        
        case I2C_REG_RSS:
        I2C_SLAVE_write(_regRSS);
        break;
        
        case I2C_REG_RE:
        I2C_SLAVE_write(r_re());
        break;
        
        case I2C_REG_PWR:
        I2C_SLAVE_write(_regPWR);
        break;
        
        case I2C_REG_BATL:
        I2C_SLAVE_write(r_bat(0));
        break;
        
        case I2C_REG_BATH:
        I2C_SLAVE_write(r_bat(1));
        break;
        
        default:
        I2C_SLAVE_write(0x00);
        break;
    }
    ++_currentRegister;
}

static void i2c_write_handler()
{
    if (_writeIndex == 0) {
        _currentRegister = I2C_SLAVE_read();
        } else if (_writeIndex > 2) {
        I2C_SLAVE_send_nack();
        return;
        } else {
        _writeArray[_writeIndex] = I2C_SLAVE_read();
    }
    ++_writeIndex;
    I2C_SLAVE_send_ack();
}

static void i2c_stop_handler()
{
    if (_writeIndex > 1) {
        process_write();
    }
    _writeIndex = 0;
}

volatile int test;

static void i2c_address_handler()
{
    uint8_t addr = I2C_SLAVE_read() >> 1;
    if (addr == I2C_SLAVE_ADDR) {
        I2C_SLAVE_send_ack();
    } else {
        I2C_SLAVE_send_nack();
    }
}

static void i2c_error_handler()
{
	//while (1)
		//;
}

void initialize_i2c()
{
    I2C_SLAVE_set_read_callback(i2c_read_handler);
    I2C_SLAVE_set_write_callback(i2c_write_handler);
    I2C_SLAVE_set_address_callback(i2c_address_handler);
    I2C_SLAVE_set_stop_callback(i2c_stop_handler);
    I2C_SLAVE_set_collision_callback(i2c_error_handler);
	I2C_SLAVE_set_bus_error_callback(i2c_error_handler);
    I2C_SLAVE_open();
    //while (test  < 10);
}
