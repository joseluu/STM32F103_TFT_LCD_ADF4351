/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

----------------------------------------------------------------------
File        : LCDConf.h
Purpose     : Display driver configuration file
---------------------------END-OF-HEADER------------------------------
*/
#ifndef LCDCONF_H
#define LCDCONF_H

#include "board_config.h"

#define INIT_PIN(PORT,PIN) GPIO_InitStruct.Pin = PIN; \
							HAL_GPIO_Init(PORT,&GPIO_InitStruct);

extern int tftID;
void LcdWriteReg8(unsigned char Cmd);
void LcdWriteData8(unsigned char Data);
void LcdWriteReg16(unsigned short Cmd);
void LcdWriteData16(unsigned short Data);
int LCD_SafeToInterrupt(void);
void delay_us_DWT(int uS);
void delay_ms_DWT(int ms);

//define PIN configuration below
#ifdef SWEEPER
/* connection: 
8 bit Data:  PortA, 
RD: PC_10,      read strobe
WR: PC_11,		write strobe
RS  DC: PB_1     register or data  data/command
CS: PB_0, chip select
reset: PB_15, 
*/
#define _LE(val) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _RD(val) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _WR(val) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _DC(val) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _CS(val) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _RESET(val) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#else
#ifdef NUCLEO
#define SCATTERED_LCD_DATA_IO
//   D0 connects to digital pin 8  PA9	(Notice these are
//   D1 connects to digital pin 9  PC7  NOT in order!)
//   D2 connects to digital pin 2  PA10
//   D3 connects to digital pin 3  PB3
//   D4 connects to digital pin 4  PB5
//   D5 connects to digital pin 5  PB4
//   D6 connects to digital pin 6  PB10
//   D7 connects to digital pin 7  PA8
// some pins are used for resistive touch sense
#define LCD_D0_PORT GPIOA
#define LCD_D0_PIN GPIO_PIN_9
#define LCD_D1_PORT GPIOC
#define LCD_D1_PIN GPIO_PIN_7
#define LCD_D2_PORT GPIOA
#define LCD_D2_PIN GPIO_PIN_10
#define LCD_D3_PORT GPIOB
#define LCD_D3_PIN GPIO_PIN_3
#define LCD_D4_PORT GPIOB
#define LCD_D4_PIN GPIO_PIN_5
#define LCD_D5_PORT GPIOB
#define LCD_D5_PIN GPIO_PIN_4
#define LCD_D6_PORT GPIOB
#define LCD_D6_PIN GPIO_PIN_10
#define LCD_D7_PORT GPIOA
#define LCD_D7_PIN GPIO_PIN_8

#define XP_Pin GPIO_PIN_9
#define XP_GPIO_Port GPIOA
#define YM_Pin GPIO_PIN_7
#define YM_GPIO_Port GPIOC
#define YP_Pin GPIO_PIN_0
#define YP_GPIO_Port GPIOB
#define XM_Pin GPIO_PIN_4
#define XM_GPIO_Port GPIOA

// ADC1 measures using YP  PB0
#define ADC1_CHANNEL ADC_CHANNEL_8
// ADC1 measures using XM  PA4
#define ADC2_CHANNEL ADC_CHANNEL_4


#define LCD_CS_PORT GPIOB
#define LCD_CS_PIN GPIO_PIN_0
#define LCD_DC_PORT GPIOA
#define LCD_DC_PIN GPIO_PIN_4
#define LCD_WR_PORT GPIOA
#define LCD_WR_PIN GPIO_PIN_1
#define LCD_RD_PORT GPIOA
#define LCD_RD_PIN GPIO_PIN_0
#define LCD_RESET_PORT GPIOC
#define LCD_RESET_PIN GPIO_PIN_1

#define _LE(val)
#define _CS(val) HAL_GPIO_WritePin(LCD_CS_PORT,LCD_CS_PIN,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _DC(val) HAL_GPIO_WritePin(LCD_DC_PORT,LCD_DC_PIN,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _WR(val) HAL_GPIO_WritePin(LCD_WR_PORT,LCD_WR_PIN,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _RD(val) HAL_GPIO_WritePin(LCD_RD_PORT,LCD_RD_PIN,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _RESET(val) HAL_GPIO_WritePin(LCD_RESET_PORT,LCD_RESET_PIN,(val?GPIO_PIN_SET:GPIO_PIN_RESET))

#else
/* connection: 
8 bit Data:  PortA, 
RD: PB_11,      read strobe
WR: PB_10,		write strobe
RS  DC: PB_1     register or data  data/command
CS: PB_0, chip select
reset: PB_15, 
*/
#define _RD(val) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _WR(val) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _DC(val) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _CS(val) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _RESET(val) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define _LE(val)

// some pins are used for resistive touch sense
#define XP_Pin GPIO_PIN_0
#define XP_GPIO_Port GPIOA
#define YM_Pin GPIO_PIN_1
#define YM_GPIO_Port GPIOA
#define YP_Pin GPIO_PIN_0
#define YP_GPIO_Port GPIOB
#define XM_Pin GPIO_PIN_1
#define XM_GPIO_Port GPIOB
#define ADC1_CHANNEL ADC_CHANNEL_8
#define ADC2_CHANNEL ADC_CHANNEL_9
#endif
#endif

#endif /* LCDCONF_H */

/*************************** End of file ****************************/
