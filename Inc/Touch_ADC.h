#pragma once
#ifndef _TOUCH_ADC_H
#define _TOUCH_ADC_H


void LCD_Touch_ADC_Init(void);
unsigned int LCD_Touch_ADC_GetXY(unsigned short * xValueReturned, unsigned short  *yValueReturned);
void GPIO_X_MeasurementSetup(void);
void GPIO_Y_MeasurementSetup(void);
void GPIO_Z_MeasurementSetup(void);
unsigned short GPIO_X_Measurement(void);
unsigned short GPIO_Y_Measurement(void);
int GPIO_Z_Measurement(void);
void GPIO_Restore_Outputs(void);

#endif