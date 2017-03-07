#pragma once
#ifndef _FRAMEWINDLG_2_H
#define _FRAMEWINDLG_2_H

#include "WM.h"
WM_HWIN CreateFramewin(void);

void DisplayCurrentFrequency(WM_HWIN hWin, int frequencyKHz);
void DisplaySweepProgress(int percentValue);

#endif
