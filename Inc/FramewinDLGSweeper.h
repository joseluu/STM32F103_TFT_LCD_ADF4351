#pragma once
#ifndef _FRAMEWINDLGSWEEPER_H
#define _FRAMEWINDLGSWEEPER_H

#include "WM.h"
WM_HWIN CreateFramewin(void);
void DisplayCurrentFrequency(WM_HWIN hWin, double frequency);

#endif