#pragma once
#ifndef _board_config_h
#define _board_config_h


// next 3 lines: choose which board you have
//#define NUCLEO
//#define SWEEPER
//#define GD32_CLOCK

// TODO: GD32_CLOCK should be used to initialize the HSE clock to 12MHz in stm32f1xx_hal_conf.h



#ifdef SWEEPER
#define ILI9320
#define USING_TOUCH_ADS7843
#else
#define USING_TOUCH_ADC
#define ILI9341
#endif

#ifdef NUCLEO
#define NUCLEO_CLOCKS
#endif

#define _SYNC(val)  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,(val?GPIO_PIN_SET:GPIO_PIN_RESET))  // synchronize oscilloscope

#endif