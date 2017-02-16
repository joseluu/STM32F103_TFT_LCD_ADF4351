#pragma once
#ifndef ADF4350_MESSAGES_H
#define ADF4350_MESSAGES_H

#include <WM.h>

int GetRefClk();
void OnEditClicked_RF_Out(WM_MESSAGE * pMsg);
void OnButtonClicked_RF_OUT(WM_MESSAGE * pMsg);
void OnButtonClicked_Start_F(WM_MESSAGE * pMsg);
void OnButtonClicked_Stop_F(WM_MESSAGE * pMsg);
void OnButtonClicked_Delta_F(WM_MESSAGE * pMsg);
void OnButtonClicked_Ramp_Clk(WM_MESSAGE * pMsg);
void OnButtonClicked_Start_SW(WM_MESSAGE * pMsg);
void OnButtonClicked_Stop_SW(WM_MESSAGE * pMsg);
void OnEditClicked_Start_F(WM_MESSAGE * pMsg);
void OnEditClicked_Stop_F(WM_MESSAGE * pMsg);
void OnEditClicked_Delta_F(WM_MESSAGE * pMsg);
void OnEditClicked_Ramp_Clk(WM_MESSAGE * pMsg);

#endif

