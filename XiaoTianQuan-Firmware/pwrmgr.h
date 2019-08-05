/*
 * pwrmgr.h
 *
 * Created: 7/18/2019 15:22:38
 *  Author: Afa
 */ 


#ifndef PWRMGR_H_
#define PWRMGR_H_

void pm_wake_up();  // clkIO, clkADC
void pm_go_idle();  // clkADC
void pm_go_standby();       // no clock domains
void pm_go_power_down();    // no clock domains, no clock source

#endif /* PWRMGR_H_ */