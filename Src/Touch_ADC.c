
#include <string.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "LCDConf_F103_24.h"
#ifdef USING_TOUCH_ADC
#include "Touch_ADC.h"


/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;


/* Private function prototypes -----------------------------------------------*/
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);

#define ELEM_SWAP(a,b) { unsigned short t=(a);(a)=(b);(b)=t; }
#define median(a,n) kth_smallest(a,n,(((n)&1)?((n)/2):(((n)/2)-1)))
#define NUMSAMPLES 5

static unsigned short kth_smallest(unsigned short a[], int n, int k);

extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim3;

void delay_us_DWT(unsigned int us);
void Error_Handler(void);

void LCD_Touch_ADC_Init(void){
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_ADC2_CLK_ENABLE();
	MX_ADC1_Init();
	MX_ADC2_Init();
	GPIO_Restore_Outputs();
	HAL_TIM_Base_Start_IT(&htim3);
}

// when user presses the screen, the returned value is non zero (usually between 1000 and 3000)
// the x and y coordinates of the touch are returned
// point (0,0) is at the top left corner assuming the display is held in portrait mode and the reset button is at the bottom
// xCoordinates varies from 900 to 3000
// yCoordinates varies from 300 to 900
unsigned int LCD_Touch_ADC_GetXY(unsigned short * xValueReturned, unsigned short * yValueReturned){
	volatile static  int g_ADCValue1 = 0;
	volatile static  int g_ADCValue2 = 0;
	unsigned short xValue;
	unsigned short yValue;
	int zValue;
	static unsigned short sValues[NUMSAMPLES];
	static unsigned short sValues2[NUMSAMPLES];

	GPIO_X_MeasurementSetup();
	xValue = GPIO_X_Measurement();

	GPIO_Y_MeasurementSetup();
	yValue=GPIO_Y_Measurement();

	GPIO_Z_MeasurementSetup();
	zValue = GPIO_Z_Measurement();  

	GPIO_Restore_Outputs();

	*xValueReturned = xValue;
	*yValueReturned = yValue;
	return (unsigned int)zValue;
}

GPIO_PinState P_PinState;
GPIO_PinState M_PinState;

void GPIO_X_MeasurementSetup(void)
{
	// Y axis YP input  YM disconnect
	GPIO_InitTypeDef GPIO_InitStruct;

	P_PinState = ((XP_GPIO_Port->ODR & XP_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	M_PinState = ((XM_GPIO_Port->ODR & XM_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = YP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(YP_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = YM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(YM_GPIO_Port, &GPIO_InitStruct);

    // X axis output
	GPIO_InitStruct.Pin = XM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(XM_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = XP_Pin;
	HAL_GPIO_Init(XP_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(XP_GPIO_Port, XP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(XM_GPIO_Port, XM_Pin, GPIO_PIN_RESET);
}

unsigned short GPIO_X_Measurement(void){
	static int g_ADCValue1;
	static unsigned short sValues[NUMSAMPLES];
	static unsigned short xValue;
	char szValue[50];

	delay_us_DWT(20);
	for (int i = 0; i < NUMSAMPLES; i++) {
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK) {
			g_ADCValue1 = HAL_ADC_GetValue(&hadc1) & 0xFFF; 
			sValues[i] = g_ADCValue1;
		}
	}
	HAL_GPIO_WritePin(YP_GPIO_Port, XP_Pin, P_PinState);
	HAL_GPIO_WritePin(YM_GPIO_Port, XM_Pin, M_PinState);
	GPIO_Restore_Outputs();
	xValue = median(sValues, NUMSAMPLES);
	sprintf(szValue, "X: %d\t", xValue);
	HAL_UART_Transmit(&huart1, szValue, strlen(szValue), 1000); 
	return xValue;
}
void GPIO_Y_MeasurementSetup(void)
{
	// X axis XM input  XP disconnect
	GPIO_InitTypeDef GPIO_InitStruct;

	P_PinState = ((YP_GPIO_Port->ODR & YP_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	M_PinState = ((YM_GPIO_Port->ODR & YM_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = XM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(XM_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = XP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(XP_GPIO_Port, &GPIO_InitStruct);

	// Y axis output
	GPIO_InitStruct.Pin = YP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(YP_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = YM_Pin;
	HAL_GPIO_Init(YM_GPIO_Port, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(YP_GPIO_Port, YP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(YM_GPIO_Port, YM_Pin, GPIO_PIN_RESET);
}

unsigned short GPIO_Y_Measurement(void){
	static int g_ADCValue2;
	static unsigned short sValues[NUMSAMPLES];
	unsigned short yValue;
	char szValue[50];

	delay_us_DWT(200);
	for (int i = 0; i < NUMSAMPLES; i++) {
		HAL_ADC_Start(&hadc2);
		if (HAL_ADC_PollForConversion(&hadc2, 1000000) == HAL_OK) {
			g_ADCValue2 = HAL_ADC_GetValue(&hadc2) & 0xFFF;
			sValues[i] = g_ADCValue2;
		}
	}
	HAL_GPIO_WritePin(YP_GPIO_Port, YP_Pin, P_PinState);
	HAL_GPIO_WritePin(YM_GPIO_Port, YM_Pin, M_PinState);
	GPIO_Restore_Outputs();
	yValue = median(sValues, NUMSAMPLES);
	sprintf(szValue, "Y: %d\t", yValue);
	HAL_UART_Transmit(&huart1, szValue, strlen(szValue), 1000); 
	return yValue;
}

void GPIO_Z_MeasurementSetup()
{
	// XM input YP input
	GPIO_InitTypeDef GPIO_InitStruct;

	P_PinState = ((XP_GPIO_Port->ODR & XP_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	M_PinState = ((YM_GPIO_Port->ODR & YM_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = XM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(XM_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = YP_Pin;
	HAL_GPIO_Init(YP_GPIO_Port, &GPIO_InitStruct);
	
	// XP output YM output
	GPIO_InitStruct.Pin = XP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(XP_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = YM_Pin;
	HAL_GPIO_Init(YM_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(XP_GPIO_Port, XP_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(YM_GPIO_Port, YM_Pin, GPIO_PIN_SET);
}

int GPIO_Z_Measurement(void){ 
	static int g_ADCValue3;
	static int g_ADCValue4;
	static unsigned short sValues[NUMSAMPLES];
	static unsigned short sValues2[NUMSAMPLES];
	char szValue[50];

	for (int i = 0; i < NUMSAMPLES; i++) {
		HAL_ADC_Start(&hadc1);
		HAL_ADC_Start(&hadc2);
		if ((HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK) &&
			(HAL_ADC_PollForConversion(&hadc2, 1000000) == HAL_OK)) {
			g_ADCValue3 = HAL_ADC_GetValue(&hadc1) & 0xFFF; 
			g_ADCValue4 = HAL_ADC_GetValue(&hadc2) & 0xFFF;
			sValues[i] = g_ADCValue3;
			sValues2[i] = g_ADCValue4;
		}
	}

	int z2 = median(sValues, NUMSAMPLES); 
	int z1 = median(sValues2, NUMSAMPLES);

	static float zValue1;
	static float zValue2;
	static float VT;
	static float pressure;

/* Touch resistance formula, see picture page 7 in http://www.cypress.com/file/131941/download
   in addition to resistors shown in this document, there are minimum resistances
   at each end of the resistive layers, the formula below is valid for a resistive 
   screen measured as 300 ohms on the X direction and 680 ohms on the Y direction
   300 ohms is composed by 40 ohms resistance at both ends with 220 ohms split
   on both side of the pressure point, that is: 40 + 220 X + 220 (1-X) + 40
   680 ohms is composed of 90 ohms on both ends, the rest split on the sides 
   of the pressure point: 90 + 500 Y + 500 (1-Y) + 90
	RT is the touch resistance
	X is the measured X position between 0 and 1
	Y ditto
	VT is the voltage difference measured in Z mode between 0 and 1 where 1 is the inactive value

	RT= (130 * VT + 220 * VT * X + 500 * VT  * Y)/(VT - 1)

	*/
    
	if (z2 > 4094) {// let's not divide by 0
		z2 = 4094;
	}
	// here we approximate using X=0.5 and Y=0.5
	VT = (z2 - z1) / 4095.0;
	zValue2 = 490 * VT / (1 - VT);
	//pressure varies as inverse of resistance
	pressure = (zValue2 > 2000? 0 : 2000 - zValue2);
	
	HAL_GPIO_WritePin(XP_GPIO_Port, XP_Pin, P_PinState);
	HAL_GPIO_WritePin(YM_GPIO_Port, YM_Pin, M_PinState);
	GPIO_Restore_Outputs();
	return (int)(pressure);
}

void GPIO_Restore_Outputs(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = XM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(XM_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = YP_Pin;
	HAL_GPIO_Init(YP_GPIO_Port, &GPIO_InitStruct);
	

	GPIO_InitStruct.Pin = XP_Pin;
	HAL_GPIO_Init(XP_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = YM_Pin;
	HAL_GPIO_Init(YM_GPIO_Port, &GPIO_InitStruct);
}

static unsigned short kth_smallest(unsigned short a[], int n, int k)
{
	unsigned int i, j, l, m;
	unsigned short x;

	l = 0; m = n - 1;
	while (l < m) {
		x = a[k];
		i = l;
		j = m;
		do {
			while (a[i] < x) i++;
			while (x < a[j]) j--;
			if (i <= j) {
				ELEM_SWAP(a[i], a[j]);
				i++; j--;
			}
		} while (i <= j) ;
		if (j < k) l = i;
		if (k < i) m = j;
	}
	return a[k] ;
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

	ADC_ChannelConfTypeDef sConfig;

	    /**Common config 
	    */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}

	    /**Configure Regular Channel 
	    */
	sConfig.Channel = ADC1_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}

}

/* ADC2 init function */
static void MX_ADC2_Init(void)
{

	ADC_ChannelConfTypeDef sConfig;

	    /**Common config 
	    */
	hadc2.Instance = ADC2;
	hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc2.Init.ContinuousConvMode = DISABLE;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc2) != HAL_OK) {
		Error_Handler();
	}

	    /**Configure Regular Channel 
	    */
	sConfig.Channel = ADC2_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
		Error_Handler();
	}

}

#endif // USING_TOUCH_ADC