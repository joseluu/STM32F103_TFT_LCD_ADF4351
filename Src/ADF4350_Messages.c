
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
	SetCurrentFrequency(sweepParameters.current);
	RF_OUT();//���
}


int bStarted;

void DoStartSweep()
{
	unsigned long Start;
	unsigned long Stop;
	unsigned long Sweep_time;
	unsigned long SweepDeltaFrequency;

	if (!bStarted) {
		bStarted = 1;
	
		Start = sweepParameters.start;
		Stop = sweepParameters.stop;
		Sweep_time = sweepParameters.timeStep;
		SweepDeltaFrequency = sweepParameters.step;
		StartSweep(Start, Stop, Sweep_time, SweepDeltaFrequency);
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

