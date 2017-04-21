#pragma once
#ifndef ADF4350_MESSAGES_H
#define ADF4350_MESSAGES_H

#include <WM.h>

struct {
	int refClk;
	int current;
	int start;
	int stop;
	int step;
	int timeStep;
} sweepParameters;

int GetRefClk();

void DoRfOut(void);
void DoStartSweep(void);
void DoStopSweep(void);
void SweepProgress(int percentSweep);

#endif

