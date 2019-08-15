/*
 * batadc.h
 *
 * Created: 8/15/2019 18:39:26
 *  Author: Afa
 */ 


#ifndef BATADC_H_
#define BATADC_H_

void initialize_batadc();
void start_battery_volt_conversion();
float get_battery_volt();


#endif /* BATADC_H_ */