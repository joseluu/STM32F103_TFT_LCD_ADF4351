/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
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

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information

Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : TOUCH_Sample.c
Purpose     : Shows how to access a touch panel without using buttons
Requirements: WindowManager - ( )
              MemoryDevices - ( )
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "board_config.h"
#include "LCDConf_F103_24.h"
#include "ADF4350_V1.h"
#include "Board_Touch.h"
#include "FrameWinDLGSweeper.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Recommended memory to run the sample with adequate performance
//
#define RECOMMENDED_MEMORY (1024L * 10)

int TouchX ;
int TouchY ;

	

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
	GUI_PID_STATE TouchState;
	int           xPhys ;
	int           yPhys ;
	
	ADF4351_Init();
	SetCurrentFrequency(248);
	RF_OUT();

	GUI_Init();	

	Touch_Initialize();

	GUI_CURSOR_Show();
//	GUI_SetColor(GUI_BLUE);
	GUI_Clear();
//	GUI_DispString("Starting ...");
	WM_HWIN hWin;
	hWin = CreateFramewin();			// écran avec barre du haut " ADF 4351 PLL "

	//EDIT_SetCursorAtPixel(0,0);
	
	while (1) {
		GUI_Exec();
		GUI_Delay(10);
	}
}	
	
	/*

//	SynchroScope();          // modif cricri
  // Infinite loop //
	while (1) {
//		DisplayCurrentFrequency(hWin, GetCurrentFrequency());
//		k_TouchUpdate();          // modif cricri
//		SynchroScope();
		GUI_Exec();
		GUI_Delay(10);
	}
}
*/

/*************************** End of file ****************************/

