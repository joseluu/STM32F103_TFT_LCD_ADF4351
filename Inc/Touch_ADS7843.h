#ifndef _ADS7843DRV_H_
#define _ADS7843DRV_H_


#define	CHX 	0x90 	
#define	CHY 	0xd0	

#define TP_DCLK_PORT GPIOC
#define TP_DCLK_PIN GPIO_PIN_9
#define TP_CS_PORT GPIOC
#define TP_CS_PIN GPIO_PIN_7
#define TP_DIN_PORT GPIOB
#define TP_DIN_PIN GPIO_PIN_15
#define TP_DOUT_PORT GPIOB
#define TP_DOUT_PIN GPIO_PIN_12
#define TP_BUSY_PORT GPIOB
#define TP_BUSY_PIN GPIO_PIN_13
//
#define TP_PEN_PORT GPIOB
#define TP_PEN_PIN GPIO_PIN_11

#define TP_DCLK(val) HAL_GPIO_WritePin(TP_DCLK_PORT,TP_DCLK_PIN,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define TP_CS(val) HAL_GPIO_WritePin(TP_CS_PORT,TP_CS_PIN,(val?GPIO_PIN_SET:GPIO_PIN_RESET))
#define TP_DIN(val) HAL_GPIO_WritePin(TP_DIN_PORT,TP_DIN_PIN,(val?GPIO_PIN_SET:GPIO_PIN_RESET))

#define TP_DOUT HAL_GPIO_ReadPin(TP_DOUT_PORT,TP_DOUT_PIN)
#define TP_BUSY HAL_GPIO_ReadPin(TP_BUSY_PORT,TP_BUSY_PIN)

#define TP_PEN(val) HAL_GPIO_WritePin(TP_PEN_PORT,TP_PEN_PIN,(val?GPIO_PIN_SET:GPIO_PIN_RESET))

#define TP_INT_IN    //( IOPIN1 & MASK_INT )


void TP_Init(void);
void TP_GetAdXY(int *x,int *y, int *touchDetected);

#endif


