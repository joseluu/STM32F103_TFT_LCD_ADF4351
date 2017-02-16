/*
* Touch driver for the adafruit 2.4 LCD
* resistive screen, uses the D0 and D1 lines for sensing
* and the CS and xxx line
*
* This file is used in conjonction with the GUI_X_Touch.c unchanged
* from the emWin/samples
*/

#include "stm32f1xx_hal.h"
#include <GUI.h>
#include "Board_Touch.h"
#include "Board_Config.h"

#include "LCDConf_F103_24.h"

#ifdef USING_TOUCH_ADS7843
#include "Touch_ADS7843.h"
void Touch_Update(void)
{
	static GUI_PID_STATE TS_State = { 0, 0, 0, 0 };
	uint16_t xDiff, yDiff;  
	int x, y, touchDetected;

	TP_GetAdXY(&x, &y, &touchDetected);

		if ((x >= 320) || (y >= 240 )) 
//	if ((x >= LCD_GetXSize()) || (y >= LCD_GetYSize()))   				// modif cricri
		{
		x = 0;
		y = 0;
		touchDetected = 0;
	} else if ((x <= 0) || (y <= 0)) {
		x = 0;
		y = 0;
		touchDetected = 0;
	} else {
		touchDetected = 1;
	}

	xDiff = (TS_State.x > x) ? (TS_State.x - x) : (x - TS_State.x);
	yDiff = (TS_State.y > y) ? (TS_State.y - y) : (y - TS_State.y);
  
  
	if ((TS_State.Pressed != touchDetected) ||
	    (xDiff > 30) ||
	    (yDiff > 30)) {
		TS_State.Pressed = touchDetected;
		TS_State.Layer = 0;//SelLayer;
		if (touchDetected) {
			TS_State.x = x;
			TS_State.y = y;
			GUI_TOUCH_StoreStateEx(&TS_State);
		} else {
			GUI_TOUCH_StoreStateEx(&TS_State);
			TS_State.x = 0;
			TS_State.y = 0;
		}
	}
}

void   Touch_Initialize(void)
{
/* initializes the touch controller */

	TP_Init();
}
#endif

#ifdef USING_TOUCH_ADC
#include "Touch_ADC.h"

void Touch_Update()
{
	if (LCD_SafeToInterrupt()) {
		GUI_TOUCH_Exec(); 
	}
}

#define USE_EMWIN_ANALOG_DRIVER
#ifdef USE_EMWIN_ANALOG_DRIVER


volatile static int pressure;
void GUI_TOUCH_X_ActivateY(void){ // for X measurements
	GPIO_Z_MeasurementSetup();
	pressure = GPIO_Z_Measurement();
}

int GUI_TOUCH_X_MeasureX(void){
	int xValue=-1;
	if (pressure > 100) {
		GPIO_X_MeasurementSetup();
		xValue = GPIO_X_Measurement();
	}
	return xValue;
}

void GUI_TOUCH_X_ActivateX(void) { //  for Y measurements
}

int GUI_TOUCH_X_MeasureY(void){
	int yValue = -1;
	if (pressure > 100) {
		GPIO_Y_MeasurementSetup();
		yValue = GPIO_Y_Measurement();
	}
	return yValue;
}





#define TOUCH_AD_LEFT 3500
#define TOUCH_AD_RIGHT 400
#define TOUCH_AD_TOP 3500
#define TOUCH_AD_BOTTOM 500


#define XSIZE_PHYS  240         // ces dmensions ne sont utilisees que dans le cas ou l'ADC interne du microcontrolleur
#define YSIZE_PHYS  320			// sert a mesurer la resistance


//#define XSIZE_PHYS  320								//  s'il y a cette modif l'ecran ne s'affiche que jusqu'aux rectangles
//#define YSIZE_PHYS  240								// modif cricri   mais s'affiche complet !! le cadre ...



void   Touch_Initialize(void)
{
/* initializes the ADC and GPIOs */
	GUI_TOUCH_Calibrate(GUI_COORD_X, 0, XSIZE_PHYS - 1, TOUCH_AD_LEFT, TOUCH_AD_RIGHT);    	
	/* Calibrate y */
	GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, YSIZE_PHYS - 1, TOUCH_AD_TOP, TOUCH_AD_BOTTOM); 

	LCD_Touch_ADC_Init();
}
#else

#endif
#endif