/*
 * batadc.c
 *
 * Created: 8/15/2019 19:12:14
 *  Author: Afa
 */ 

#include <atmel_start.h>

#include "batadc.h"

volatile adc_result_t _batadc_measurement;
volatile uint8_t      _batadc_measurement_normalized;
volatile float _batadc_voltage = 0.f;

static void handler_cb(void)
{
    _batadc_measurement = BATTERY_ADC_get_conversion_result();
    _batadc_measurement_normalized = _batadc_measurement >> (BATTERY_ADC_get_resolution() - 8);
    _batadc_voltage = (2.5f / 256.f * _batadc_measurement_normalized) * 7.2f;
}

void initialize_batadc()
{
    BATTERY_ADC_register_callback(handler_cb);
    BATTERY_ADC_start_conversion(0);
}

void start_battery_volt_conversion()
{
    BATTERY_ADC_start_conversion(0);
}


float get_battery_volt()
{
    return _batadc_voltage;
}



