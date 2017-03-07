#pragma once
#ifndef __ADF4350_H
#define __ADF4350_H

#include <WM.h>   // for WM_MESSAGE
/*
* MASTER CLOCK
*/

#ifdef SWEEPER
#define REF_CLK   25
#else
#define REF_CLK   10
#endif


/*
 * CONNECTION PINS DEFINITION
*/

// chip enable: must be high
#define CE_PORT GPIOC
#define CE_PIN  GPIO_PIN_13
#define SET_CE()		HAL_GPIO_WritePin(CE_PORT,CE_PIN,GPIO_PIN_SET)
#define CLR_CE()		HAL_GPIO_WritePin(CE_PORT,CE_PIN,GPIO_PIN_RESET)

// serial data
#define MOSI_PORT GPIOC
#define MOSI_PIN  GPIO_PIN_15
#define SET_DATA()		HAL_GPIO_WritePin(MOSI_PORT, MOSI_PIN,GPIO_PIN_SET)
#define CLR_DATA()		HAL_GPIO_WritePin(MOSI_PORT, MOSI_PIN,GPIO_PIN_RESET)

// load serial data on rising edge, data must be present 10ns before and after
// min clock half period: 25ns
#define SCL_PORT GPIOC
#define SCL_PIN  GPIO_PIN_0
#define SET_SCL()		HAL_GPIO_WritePin(SCL_PORT, SCL_PIN,GPIO_PIN_SET)
#define CLR_SCL()		HAL_GPIO_WritePin(SCL_PORT, SCL_PIN,GPIO_PIN_RESET)


// load 32 bit register on rising edge
#define LE_PORT GPIOC
#define LE_PIN  GPIO_PIN_14
#define SET_LE()		HAL_GPIO_WritePin(LE_PORT, LE_PIN,GPIO_PIN_SET)
#define CLR_LE()		HAL_GPIO_WritePin(LE_PORT, LE_PIN,GPIO_PIN_RESET)



void ADF4351_Init(int initialFrequency);
void RF_OUT(void);
void SweepTimerTick(void);
int SetCurrentFrequency(int newFrequencyKHz);
void reset_all_reg(int initialFrequency);
void StartSweep(unsigned long Start, 
	unsigned long  Stop, 
	unsigned long  Sweep_Tim, 
	double  Delta);
void SweepTimerStop(void);

void ProgBar(void);


#endif
