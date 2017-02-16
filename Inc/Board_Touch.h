#pragma once

typedef struct {
	int pressed;
	int x;
	int y;
} TOUCH_STATE;

void   Touch_Initialize(void);
void  Touch_Update(void);

void Touch_GetState(TOUCH_STATE *touch_state);