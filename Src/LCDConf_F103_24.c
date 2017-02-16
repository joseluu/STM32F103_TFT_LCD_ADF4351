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
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @file    LCDConf_stm3210c_eval.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-September-2016
  * @brief   Driver for STM3210C-EVAL board LCD
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "LCDConf_F103_24.h"
#include "stm32f1xx_hal.h"
#include "lcd_ctrl_initialize.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
#define LCD_MIRROR_X 1
//
// Physical display size
//
#define XSIZE_PHYS  240
#define YSIZE_PHYS  320



/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif

/*********************************************************************
*
*       Defines, sfrs
*
**********************************************************************
*/
#define START_BYTE         0x70
#define SET_INDEX          0x00
#define READ_STATUS        0x01
#define LCD_WRITE_REG      0x02
#define LCD_READ_REG       0x03

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
void MX_GPIO_Init(void);
int tftID;




void delay_clk_DWT(uint32_t nCycles)
{
	volatile uint32_t start = DWT->CYCCNT;
	do {
	} while (DWT->CYCCNT - start < nCycles );
}
void delay_us_DWT(int uSec)
{
	volatile uint32_t cycles = (SystemCoreClock / 1000000L)*uSec;
	volatile uint32_t start = DWT->CYCCNT;
	do {
	} while (DWT->CYCCNT - start < cycles);
}

void delay_ms_DWT(int mSec)
{
	uint32_t uSec = mSec * 1000;
	volatile uint32_t cycles = (SystemCoreClock / 1000000L)*uSec;
	volatile uint32_t start = DWT->CYCCNT;
	do {
	} while (DWT->CYCCNT - start < cycles);
}

/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
#ifdef SCATTERED_LCD_DATA_IO
static void 	ScatteredWrite8(U8 Data){
#define WRITE_BIT(PORT,PIN,BIT) \
	HAL_GPIO_WritePin(PORT, PIN, (Data &(1<<BIT) ? GPIO_PIN_SET : GPIO_PIN_RESET))
	WRITE_BIT(LCD_D0_PORT, LCD_D0_PIN, 0);
	WRITE_BIT(LCD_D1_PORT, LCD_D1_PIN, 1);
	WRITE_BIT(LCD_D2_PORT, LCD_D2_PIN, 2);
	WRITE_BIT(LCD_D3_PORT, LCD_D3_PIN, 3);
	WRITE_BIT(LCD_D4_PORT, LCD_D4_PIN, 4);
	WRITE_BIT(LCD_D5_PORT, LCD_D5_PIN, 5);
	WRITE_BIT(LCD_D6_PORT, LCD_D6_PIN, 6);
	WRITE_BIT(LCD_D7_PORT, LCD_D7_PIN, 7);
}

static U8 ScatteredRead8(void){
#define READ_PORT_PIN(PORT,PIN) \
	HAL_GPIO_ReadPin(PORT, PIN)
	U8 result;

	result=
		(READ_PORT_PIN(LCD_D0_PORT, LCD_D0_PIN)!=GPIO_PIN_RESET? 1 << 0:0) |
		(READ_PORT_PIN(LCD_D1_PORT, LCD_D1_PIN)!=GPIO_PIN_RESET? 1 << 1:0) |
		(READ_PORT_PIN(LCD_D2_PORT, LCD_D2_PIN)!=GPIO_PIN_RESET? 1 << 2:0) |
		(READ_PORT_PIN(LCD_D3_PORT, LCD_D3_PIN)!=GPIO_PIN_RESET? 1 << 3:0) |
		(READ_PORT_PIN(LCD_D4_PORT, LCD_D4_PIN)!=GPIO_PIN_RESET? 1 << 4:0) |
		(READ_PORT_PIN(LCD_D5_PORT, LCD_D5_PIN)!=GPIO_PIN_RESET? 1 << 5:0) |
		(READ_PORT_PIN(LCD_D6_PORT, LCD_D6_PIN)!=GPIO_PIN_RESET? 1 << 6:0) |
		(READ_PORT_PIN(LCD_D7_PORT, LCD_D7_PIN)!=GPIO_PIN_RESET? 1 << 7:0) ;

	return result;
}
#endif

void LcdWriteReg16(unsigned short Cmd)
{
//	Port_Data_output();
	_CS(1); // cancel previous command
	delay_clk_DWT(1);
	_CS(0);
	_DC(0); // 0=cmd
	_WR(0);
	GPIOA->ODR = (GPIOA->ODR & 0xFF00) | (Cmd >> 8);     // write MSB
	_LE(1);
	delay_clk_DWT(1);
	_LE(0);
	GPIOA->ODR = (GPIOA->ODR & 0xFF00) | (Cmd & 0xFF); //write LSB
	delay_clk_DWT(1);
	_WR(1);					// should allow 10ns min settling time
	_CS(1);
	_DC(1); // 1=data next
}

void LcdWriteReg8(unsigned char Cmd) {
	_CS(1); // cancel previous command
	delay_clk_DWT(1);
	_CS(0);
	_DC(0); // 0=cmd
	_WR(0);
#ifdef SCATTERED_LCD_DATA_IO
	ScatteredWrite8(Cmd);
#else
	GPIOA->ODR = (GPIOA->ODR & 0xFF00) | (Cmd & 0xFF);  // write 8bit
#endif
	delay_clk_DWT(1);
	_WR(1);					// should allow 10ns min settling time
	_CS(1);
	_DC(1); // 1=data next
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
void LcdWriteData16(unsigned short Data)
{
//	Port_Data_output();
	_CS(0);
	_DC(1); // 1=data just to make sure
	GPIOA->ODR = (GPIOA->ODR & 0xFF00) | (Data >> 8);     // write MSB
	_LE(1);
	delay_clk_DWT(1);
	_LE(0);
	GPIOA->ODR = (GPIOA->ODR & 0xFF00) | (Data & 0xFF);     // write LSB
	delay_clk_DWT(1);
	_WR(0);
	_WR(1);					// should allow 10ns min settling time
	_CS(1);
}
void LcdWriteData8(unsigned char Data) {
	_CS(0);
	_DC(1); // 1=data just to make sure
#ifdef SCATTERED_LCD_DATA_IO
	ScatteredWrite8(Data);
#else
	GPIOA->ODR = (GPIOA->ODR & 0xFF00) | (Data & 0xFF);     // write 8bit
#endif
	_WR(0);
	_WR(1);					// should allow 10ns min settling time
	_CS(1);
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
#ifdef SWEEPER
static void LcdWriteDataMultiple16(U16 * pData, int NumItems)
{
	while (NumItems--) {
		LcdWriteData16(*pData++);
	} 
}
#else
static void LcdWriteDataMultiple8(U8 * pData, int NumItems) {
  while (NumItems--) {
	  LcdWriteData8(*pData++);
  } 
}
#endif
/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void Port_Data_input()
{
	GPIO_InitTypeDef GPIO_InitStruct;
#ifdef SCATTERED_LCD_DATA_IO
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	INIT_PIN(LCD_D0_PORT, LCD_D0_PIN)
	INIT_PIN(LCD_D1_PORT, LCD_D1_PIN)
	INIT_PIN(LCD_D2_PORT, LCD_D2_PIN)
	INIT_PIN(LCD_D3_PORT, LCD_D3_PIN)
	INIT_PIN(LCD_D4_PORT, LCD_D4_PIN)
	INIT_PIN(LCD_D5_PORT, LCD_D5_PIN)
	INIT_PIN(LCD_D6_PORT, LCD_D6_PIN)
	INIT_PIN(LCD_D7_PORT, LCD_D7_PIN)
#else
	/*Configure GPIO pins : PA0 PA1 PA2 PA3 
                           PA4 PA5 PA6 PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 
	                        | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
}

static void Port_Data_output()
{
	GPIO_InitTypeDef GPIO_InitStruct;
#ifdef SCATTERED_LCD_DATA_IO
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	INIT_PIN(LCD_D0_PORT, LCD_D0_PIN)
	INIT_PIN(LCD_D1_PORT, LCD_D1_PIN)
	INIT_PIN(LCD_D2_PORT, LCD_D2_PIN)
	INIT_PIN(LCD_D3_PORT, LCD_D3_PIN)
	INIT_PIN(LCD_D4_PORT, LCD_D4_PIN)
	INIT_PIN(LCD_D5_PORT, LCD_D5_PIN)
	INIT_PIN(LCD_D6_PORT, LCD_D6_PIN)
	INIT_PIN(LCD_D7_PORT, LCD_D7_PIN)
#else
	/*Configure GPIO pins : PA0 PA1 PA2 PA3 
                           PA4 PA5 PA6 PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 
	                        | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
}

static U8 LcdReadData8(void){
	U8 Data;
	Port_Data_input();
	_RD(0);
	delay_clk_DWT(1);   // fake read is necessary
	_RD(1);	
	_RD(0);
	delay_clk_DWT(1); 
#ifdef SCATTERED_LCD_DATA_IO
	Data = ScatteredRead8();
#else
	Data= GPIOA->IDR;     // read 8bit should allow 45ns settling time
	delay_clk_DWT(1); 
#endif
	_RD(1);	
	Port_Data_output();
	return Data;
}

static void LcdReadDataMultiple8(U8 * pData, int NumItems) {
	U8 Data;
	Port_Data_input();
	_RD(0);
	delay_clk_DWT(1);    // fake read is necessary
	_RD(1);	
	while (NumItems --) {
		_RD(0);
		delay_clk_DWT(1); 
#ifdef SCATTERED_LCD_DATA_IO
		*pData++ = ScatteredRead8();
#else
		*pData++ = GPIOA->IDR;     // read 8bit should allow 45ns settling time
		delay_clk_DWT(1); 
#endif
		_RD(1);	
	}
	Port_Data_output();
}

unsigned int rd_reg_data32(unsigned char reg)
{
	LcdWriteReg8(reg);
	unsigned int r = 0;
	U8 rb[4];

	LcdReadDataMultiple8(&rb[0], 4);
	r = rb[0] << 24 | rb[1] << 16 | rb[2] << 8 | rb[3];

	_CS(1); // force CS HIGH to cancel the cmd in case it was not supported
	return r;
}

void Board_LCD_Init(void) {
	MX_GPIO_Init();
	_RD(1);
	_WR(1);
	_DC(0);
	_CS(1);
	_RESET(1);
#ifdef SWEEPER
	_LE(0);
#endif
	delay_us_DWT(15);  // 10us min
	_RESET(0);  // reset is active
	delay_us_DWT(15); // 10us min
	_RESET(1);          // end reset
	delay_us_DWT(151370);		// 120 ms min		

//	tftID = rd_reg_data32(0x0);
	lcd_ctrl_initialize();

}


/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/

int LCD_SafeToInterrupt(void){
#ifdef USING_TOUCH_ADC
	if (HAL_GPIO_ReadPin(YP_GPIO_Port, YP_Pin) == GPIO_PIN_RESET) { // check if CS active
		return 1;
	}
#endif
	return 1;
}

void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
  //
  // Orientation
  //
#ifdef SWEEPER
	Config.Orientation= GUI_SWAP_XY | GUI_MIRROR_Y;   // pour ecran_cricri
#else
	Config.Orientation = GUI_SWAP_XY;
#endif
  GUIDRV_FlexColor_Config(pDevice, &Config);

#if (GUI_SUPPORT_TOUCH)
        /* Set the logical display orientation */
#if 0
// alternate implementation from emWin documentation
	Orientation = (GUI_MIRROR_X * LCD_GetMirrorXEx(0)) |
	              (GUI_MIRROR_Y * LCD_GetMirrorYEx(0)) |
	              (GUI_SWAP_XY  * LCD_GetSwapXYEx(0));
	GUI_TOUCH_SetOrientation(Orientation);
#else
	GUI_TOUCH_SetOrientation((GUI_MIRROR_X << 0) | (GUI_MIRROR_Y << 1) | (GUI_SWAP_XY << 2));      
#endif
#endif
  //
  // Set controller and operation mode
  //
#ifdef SWEEPER
	PortAPI.pfWrite16_A0  = LcdWriteReg16;
	PortAPI.pfWrite16_A1  = LcdWriteData16;
	PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple16;	
	PortAPI.pfRead16_A1 = LcdReadData8;
	PortAPI.pfReadM16_A1  = LcdReadDataMultiple8;
#else
  PortAPI.pfWrite8_A0  = LcdWriteReg8;
  PortAPI.pfWrite8_A1  = LcdWriteData8;
  PortAPI.pfWriteM8_A1 = LcdWriteDataMultiple8;
  PortAPI.pfRead8_A1 = LcdReadData8;
  PortAPI.pfReadM8_A1  = LcdReadDataMultiple8;
#endif
#ifdef SWEEPER
	GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66708, GUIDRV_FLEXCOLOR_M16C0B16);    //// modif cricri
#else
	GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B8);
#endif
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    Board_LCD_Init();
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	  /* GPIO Ports Clock Enable */
	__GPIOC_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

	Port_Data_output();

#ifdef SWEEPER
	  /*Configure GPIO pin : PB OUT */
	GPIO_InitStruct.Pin = (GPIO_PIN_8 | GPIO_PIN_14  | GPIO_PIN_15);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIOB->ODR = 1 << 14; // avoid spikes on control lines
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIOB->ODR = 1 << 14;

	  /*Configure GPIO pins : PC OUT */
	GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12| GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
				/*Configure GPIO pins : PA IN  pour touch panel */									// modif cricri
	GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 ;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	
	
#elif defined(NUCLEO)
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	INIT_PIN(LCD_CS_PORT, LCD_CS_PIN)
	INIT_PIN(LCD_DC_PORT, LCD_DC_PIN)
	INIT_PIN(LCD_WR_PORT, LCD_WR_PIN)
	INIT_PIN(LCD_RD_PORT, LCD_RD_PIN)
	INIT_PIN(LCD_RESET_PORT, LCD_RESET_PIN)

		  /*Configure GPIO pins : PB_8: SYNC */
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		  /*Configure GPIO pins : PC13: LED */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#else

  /*Configure GPIO pin : PA8 */									
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /*Configure GPIO pin : PB */
	GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_15);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIOB->ODR = 0x1F0; // avoid spikes on control lines
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIOB->ODR = 0x1F0;

	  /*Configure GPIO pins : PC13: LED */							
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	

	
#endif

}
void SynchroScope()
	{
		_SYNC(1);
		_SYNC(0);
	}
/*************************** End of file ****************************/

