
#include "ADF4350_Messages.h"
#include "ADF4350_V1.h"

unsigned long Register_Buf[12]; //to communicate with menu dialog

int GetRefClk(){
	return REF_CLK;
}

void OnEditClicked_RF_Out(WM_MESSAGE * pMsg)
{
}

void OnButtonClicked_RF_OUT(WM_MESSAGE * pMsg)
{
	unsigned long temp = Register_Buf[6], op;
	double n;
	op = temp % 100;
	temp -= op;
	n = temp;
	n /= 1000;
	SetCurrentFrequency(n);
	RF_OUT();//���
}

void OnButtonClicked_Start_F(WM_MESSAGE * pMsg)
{
}

void OnButtonClicked_Stop_F(WM_MESSAGE * pMsg)
{
}

void OnButtonClicked_Delta_F(WM_MESSAGE * pMsg)
{
}

void OnButtonClicked_Ramp_Clk(WM_MESSAGE * pMsg)
{
}

int bStarted;

void OnButtonClicked_Start_SW(WM_MESSAGE * pMsg)
{
	unsigned long temp = Register_Buf[9], op;
	double n;
	double Delta;
	unsigned long Start;
	unsigned long Stop;
	unsigned long Sweep_time;

	if (!bStarted) {
		bStarted = 1;
		op = temp % 100;
		temp -= op;
		n = temp;
		n /= 1000;
		Delta = n;
	
		Start = Register_Buf[7];
		Stop = Register_Buf[8];
		Sweep_time = Register_Buf[10];
		StartSweep(Start, Stop, Sweep_time, Delta);
	}
}

void OnButtonClicked_Stop_SW(WM_MESSAGE * pMsg)
{
	if (bStarted) {
		SweepTimerStop();	
		bStarted = 0;
	}
}

void OnEditClicked_Start_F(WM_MESSAGE * pMsg)
{
}

void OnEditClicked_Stop_F(WM_MESSAGE * pMsg)
{
}

void OnEditClicked_Delta_F(WM_MESSAGE * pMsg)
{
}

void OnEditClicked_Ramp_Clk(WM_MESSAGE * pMsg)
{
}

