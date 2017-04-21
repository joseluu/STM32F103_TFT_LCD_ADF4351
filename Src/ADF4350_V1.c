/*==========================================================
//	function:	ADF4530/51 ²ÊÆÁÄ£¿é
//	date:		2015/09/18
//	E_Mail:		technicxie@126.com	
==========================================================*/


#include "stm32f1xx_hal.h"
#include <math.h>
#include <string.h>  // for memcpy
#include <stdbool.h>
#include "board_config.h"
#include "ADF4350_V1.h"
#include "ADF4350_Messages.h"


void WriteToADF4350(unsigned char count, unsigned char *buf);
void ReadFromADF4350(unsigned char count, unsigned char *buf);

static unsigned long Register_Buf[6];
static unsigned long Register_Previous[6];
static unsigned int Fraction, Integer;
static unsigned char Reg_Buf[6];
static int RF_Fre_Value;
static unsigned long Start_Fre_value;
static unsigned long Stop_Fre_value;
static unsigned long Delta_Fre_value;
static unsigned long Sweep_Time_value;
static unsigned long SweepCurrentFreq;


double percentSweep = 0;
double percentSweepIncrement = 0;


extern void SweepTimerStart(void); 
extern void SweepTimerStop(void);

int SetCurrentFrequency(int newFrequencyKHz)
{
	int value = RF_Fre_Value;
	RF_Fre_Value = newFrequencyKHz;
	return value;
}

/* Delay routine for SPI signaling */
/* does not work in optimized (release) mode because of compiler optimization */
/* in debug mode, the 100 multiplier leads to a 10us delay and a 2ms ADF4351 programming time */
/* in debug mode a 10 multiplier leads to a 2us delay and a 500us programming time */
/* in optimized mode, the delay is 300ns and th e ADF4351 program time is 50us*/
#define FAST 1
#ifdef FAST
#define DELAY(n)  
#else
#define DELAY(n) delay_dds(n*10)
#endif




void delay_dds(int uSec)
{
	volatile uint32_t cycles = (SystemCoreClock / 1000000L)*uSec;
	volatile uint32_t start = DWT->CYCCNT;
	do {
	} while (DWT->CYCCNT - start < cycles);
}


void initPorts(){
GPIO_InitTypeDef GPIO_InitStruct;

	  /* GPIO Ports Clock Enable */
__GPIOC_CLK_ENABLE();
__GPIOA_CLK_ENABLE();
__GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = (CE_PIN);
	HAL_GPIO_Init(CE_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = (MOSI_PIN);
	HAL_GPIO_Init(MOSI_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = (SCL_PIN);
	HAL_GPIO_Init(SCL_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = (LE_PIN);
	HAL_GPIO_Init(LE_PORT, &GPIO_InitStruct);
}


//---------------------------------
//void WriteToADF4350(unsigned char count,unsigned char *Reg_Buf);
//---------------------------------
//Function that writes to the ADF4350 via the SPI port. 
//--------------------------------------------------------------------------------
void WriteToADF4350(unsigned char count, unsigned char *buf)
{
	unsigned	char	ValueToWrite = 0;
    unsigned	char	i = 0;
	unsigned	char	j = 0;
	
	DELAY(2);
	CLR_SCL();
	CLR_LE();
	DELAY(2);

	for(i=count;i>0;i--)
 	{
	 	ValueToWrite = *(buf + i - 1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				SET_DATA();	  //Send one to SDO pin
			}
			else
			{
				CLR_DATA();	  //Send zero to SDO pin
			}
			DELAY(1);
			SET_SCL();
			DELAY(2);
			ValueToWrite <<= 1;	//Rotate data
			CLR_SCL();
			DELAY(1);
		}
	}
	CLR_DATA();
	DELAY(2);
	SET_LE();
	DELAY(2);
	CLR_LE();
}

void RF_OUT(void)
{
	unsigned int mod_v[10]={5,25,50,125,250,500,1000,2000,4000};
	unsigned char div,flag;
	unsigned int frac_value;
	unsigned int mod_value;
	double n_value,op,y;
	double ref_clk=REF_CLK;
	unsigned long temp;
	unsigned int i,x;
	
		if(RF_Fre_Value<69000)	{
				Register_Buf[4] &= 0x000FFFFF;
				Register_Buf[4] |= 0x00E00000;
			  div=64;
		}
		else if(RF_Fre_Value<138000)
		{
				Register_Buf[4] &= 0x000FFFFF;
				Register_Buf[4] |= 0x00D00000;
			  div=32;
		}
		else if(RF_Fre_Value<275000)
		{
				Register_Buf[4] &= 0x000FFFFF;
				Register_Buf[4] |= 0x00C00000;
			  div=16;
		}
		else if(RF_Fre_Value<550000)
		{
				Register_Buf[4] &= 0x000FFFFF;
				Register_Buf[4] |= 0x00B00000;
			  div=8;
		}
		else if(RF_Fre_Value<1100000)
		{
				Register_Buf[4] &= 0x000FFFFF;
				Register_Buf[4] |= 0x00A00000;
			  div=4;
		}
		else if(RF_Fre_Value<2200000)
		{
				Register_Buf[4] &= 0x000FFFFF;
				Register_Buf[4] |= 0x00900000;
			  div=2;
		}
		else
		{
				Register_Buf[4] &= 0x000FFFFF;
				Register_Buf[4] |= 0x00800000;
			  div=1;
		}
		
	unsigned long oscillatorFrequency = RF_Fre_Value * div;
	Register_Buf[0] = (oscillatorFrequency / 1000) << 15 | (oscillatorFrequency % 1000) << 3;
		
	_SYNC(1);
	for (int i=5;i>=0;i--){
		bool mustWrite = false;
		if (mustWrite || (Register_Buf[i] != Register_Previous[i])) {
			mustWrite = true; // we must finish up to register 0 once we start writing into registers
			memcpy(&Reg_Buf, &Register_Buf[i], 4);
			WriteToADF4350(4,Reg_Buf);
			Register_Previous[i] = Register_Buf[i];// remember value so that we can skip it next time
		}
	}
	_SYNC(0);
}

void reset_all_reg(int referenceFrequency)
{
	Register_Buf[5]=0x00580005;  // set digital lock detect 

//(DB23=1)The signal is taken from the VCO directly;
//(DB22-20:4H) the RF divider is 8;
//(DB19-12:50H) band select clock divider: 08
//(DB11=0)VCO powerd up;
//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	Register_Buf[4]=0x00c0803c;

#define CLOCK_DIVIDER 1000 // KHz resolution
	Register_Buf[3]=0x00000003 | CLOCK_DIVIDER << 3;
#define R_COUNTER referenceFrequency  // 1 MHz PFD
#define DOUBLE_BUFFER 1
//(DB6=1)set PD polarity is positive;
//(DB7=1)LDP is 6nS;
//(DB8=0)enable fractional-N digital lock detect;
//(DB12-9:7H)set Icp 2.50 mA;
	Register_Buf[2] = 0x000000E42 | DOUBLE_BUFFER << 13 | R_COUNTER << 14;

#define PRESCALER_8_9 1
#define PHASE_VALUE 1
#define MODULUS_VALUE 1000 // so that we get KHz resolution
	Register_Buf[1]=0x00000001 | PRESCALER_8_9 << 27 | PHASE_VALUE << 15 | MODULUS_VALUE << 3;

#define INTEGER_VALUE 100 // MHz
#define FRACTIONAL_VALUE 0 // KHz
	Register_Buf[0]= INTEGER_VALUE << 15 | FRACTIONAL_VALUE << 3;
	
	for (int i=0;i<6;i++){
		Register_Previous[i] = 0;
	}
}

void restoreSweepParameters()
{
	sweepParameters.current = 144500;
	sweepParameters.start = 144500;
	sweepParameters.stop = 145500;
	sweepParameters.step = 100;
	sweepParameters.timeStep = 100;
}

//*********************************************************************
void ADF4351_Init(int referenceFrequency)
{
	initPorts();
	SET_CE();
	reset_all_reg(referenceFrequency);

	SetCurrentFrequency(referenceFrequency * 10);
	RF_OUT();
}


static unsigned long Sweep_Time_Counter;
static unsigned char Sweep_DIR_Flag;

void StartSweep(unsigned long Start, 
				unsigned long  Stop, 
				unsigned long  SweepDeltaTime, 
				unsigned long  SweepDeltaFrequency)
{
	if (Start < Stop) {
// Initialize global variables
		Start_Fre_value = Start;
		Stop_Fre_value = Stop;
		Sweep_Time_value = SweepDeltaTime;
		Delta_Fre_value = SweepDeltaFrequency;
		Sweep_Time_Counter = 0;
		percentSweepIncrement = 100.0/((Stop_Fre_value-Start_Fre_value)/Delta_Fre_value);

		SweepCurrentFreq = Start_Fre_value;
		Sweep_DIR_Flag = 0;
		RF_Fre_Value = Start;
		RF_OUT();
		
		SweepTimerStart();
	}
}


void SweepTimerTick(void){ // interrupt processing routine
	unsigned long temp = sweepParameters.timeStep /50; // timer is every 100 us
	if (Sweep_Time_Counter++ >= temp) {
		Sweep_Time_Counter = 0;
		if (Sweep_DIR_Flag == 0) {
			SweepCurrentFreq += Delta_Fre_value;

			SweepProgress((int)percentSweep);
			percentSweep += percentSweepIncrement;
			if (percentSweep >= 100){
				percentSweep = 100;
			}   
			
			if (SweepCurrentFreq >= Stop_Fre_value) {
				SweepCurrentFreq = Stop_Fre_value;
				Sweep_DIR_Flag = 1;
			}
		} else {
			SweepCurrentFreq -= Delta_Fre_value;

			SweepProgress(percentSweep);
			percentSweep -= percentSweepIncrement;
			if (percentSweep <= 0) {
				percentSweep = 0;
			}		
			
			if (SweepCurrentFreq <= Start_Fre_value) {
				SweepCurrentFreq = Start_Fre_value;
				Sweep_DIR_Flag = 0;
			}
		}
		RF_Fre_Value = 	SweepCurrentFreq;
		RF_OUT();
	}
}
