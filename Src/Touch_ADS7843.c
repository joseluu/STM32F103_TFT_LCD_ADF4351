						 //=========================================================================================================================================
#include <string.h>
#include "Touch_ADS7843.h"
#include "stm32f1xx_hal.h"
//====================================================================================
void Delayus(int uSec)
{
	volatile uint32_t cycles = (SystemCoreClock / 1000000L)*uSec;
	volatile uint32_t start = DWT->CYCCNT;
	do {
	} while (DWT->CYCCNT - start < cycles);
}
//====================================================================================
extern TIM_HandleTypeDef htim3;
void TP_Init(void)
{
GPIO_InitTypeDef GPIO_InitStruct;

	  /* GPIO Ports Clock Enable */
__GPIOC_CLK_ENABLE();
__GPIOA_CLK_ENABLE();
__GPIOB_CLK_ENABLE();

GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;


	GPIO_InitStruct.Pin = (TP_DCLK_PIN);
	HAL_GPIO_Init(TP_DCLK_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = (TP_CS_PIN);
	HAL_GPIO_Init(TP_CS_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = (TP_DIN_PIN);
	HAL_GPIO_Init(TP_DIN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = (TP_PEN_PIN);
	HAL_GPIO_Init(TP_PEN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

	GPIO_InitStruct.Pin = (TP_DOUT_PIN);
	HAL_GPIO_Init(TP_DOUT_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = (TP_BUSY_PIN);
	HAL_GPIO_Init(TP_BUSY_PORT, &GPIO_InitStruct);

	// start timer interrupt
	HAL_TIM_Base_Start_IT(&htim3);
}


//====================================================================================
static void WR_CMD (unsigned char cmd) 
{
    unsigned char buf;
    unsigned char i;
    TP_CS(1);
    TP_DIN(0);
    TP_DCLK(0);
    TP_CS(0);
    for(i=0;i<8;i++) 
    {
        buf=(cmd>>(7-i))&0x1;
        TP_DIN(buf);
        Delayus(5);
        TP_DCLK(1);
        Delayus(5);
        TP_DCLK(0);
    }
}
//====================================================================================
static unsigned short RD_AD(void) 
{
    unsigned short buf=0,temp;
    unsigned char i;
    TP_DIN(0);
    TP_DCLK(1);
    for(i=0;i<12;i++) 
    {
        Delayus(5);
        TP_DCLK(0);         
        Delayus(5);   
        temp= (TP_DOUT) ? 1:0;
        buf|=(temp<<(11-i));
        
        Delayus(5);
        TP_DCLK(1);
    }
    TP_CS(1);
    buf&=0x0fff;
    return(buf);
}
//====================================================================================
int Read_X(void) 
{ 
    int i;
    WR_CMD(CHX);
   // while(TP_BUSY);
    Delayus(5);
	i=(320-(RD_AD()/12.8));							// modif cricri pour inverser sens lecture X
    return i;   
}
//====================================================================================
int Read_Y(void) 
{ 
    int i;
    WR_CMD(CHY);
    //while(TP_BUSY);
    Delayus(5);
    i=(RD_AD()/16.5);
    return i;    
}
//====================================================================================
void TP_GetAdXY(int *x,int *y, int *touchDetected) 
{
    int adx,ady;
    adx= Read_X();
    ady= Read_Y();
		*x=adx;
    *y=ady;
	*touchDetected = 0;
}
