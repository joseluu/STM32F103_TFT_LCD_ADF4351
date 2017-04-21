#pragma once
#ifndef _FRAMEWINDLGSWEEPER_H
#define _FRAMEWINDLGSWEEPER_H

#include "WM.h"
WM_HWIN CreateSweeperDialog(void);


void DisplayCurrentFrequency(WM_HWIN hWin, int frequencyKHz);
void DisplaySweepProgress(int percentValue);

#endif
