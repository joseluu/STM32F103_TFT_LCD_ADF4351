
#include "ADF4350_Messages.h"
#include "ADF4350_V1.h"
#include "FrameWinDLGSweeper.h"

int GetRefClk(){
	return REF_CLK;
}

void SweepProgress(int percentSweep){
	DisplaySweepProgress(percentSweep);
}

void DoRfOut()
{
	unsigned long temp = sweepParameters.current, op;
	double n;
	op = temp % 100;
	temp -= op;
	n = temp;
	SetCurrentFrequency(n);
	RF_OUT();//���
}


int bStarted;

void DoStartSweep()
{
	unsigned long temp = sweepParameters.step, op;
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
	
		Start = sweepParameters.start;
		Stop = sweepParameters.stop;
		Sweep_time = sweepParameters.timeStep;
		StartSweep(Start, Stop, Sweep_time, Delta);
	}
}

void DoStopSweep()
{
	if (bStarted) {
		SweepTimerStop();	
		bStarted = 0;
		DoRfOut(); // restore current frequency
	}
}

