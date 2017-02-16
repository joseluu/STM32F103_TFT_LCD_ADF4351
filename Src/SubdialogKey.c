
#include "DIALOG.h"

#define GUI_ID_BUTTON10   GUI_ID_USER+0
#define GUI_ID_BUTTON11   GUI_ID_USER+1
#define GUI_ID_BUTTON12   GUI_ID_USER+2
#define GUI_ID_BUTTON13   GUI_ID_USER+3
#define GUI_ID_BUTTON14   GUI_ID_USER+4

// globals
// from FrameDlg main dialog screen
extern unsigned char Key_Return_Flag;
extern unsigned char Register_Index;
extern unsigned long Register_Buf[11];

static unsigned char Number_Index;
static int bInitialized;
static unsigned long Temp_Value;
static unsigned int Save_Time;  //用于保存数据
static unsigned char Charge_Flag[2];//用于标记寄存器被改变，需要保存


//EventsFunctionList
void OnButtonClicked_Back_Key(WM_MESSAGE * pMsg);
void OnButtonClicked_Enter(WM_MESSAGE * pMsg);
void OnButtonClicked_Symbol(WM_MESSAGE * pMsg);
void OnButtonClicked_C(WM_MESSAGE * pMsg);
void OnButtonClicked_D(WM_MESSAGE * pMsg);
void OnButtonClicked_9(WM_MESSAGE * pMsg);
void OnButtonClicked_8(WM_MESSAGE * pMsg);
void OnButtonClicked_7(WM_MESSAGE * pMsg);
void OnButtonClicked_6(WM_MESSAGE * pMsg);
void OnButtonClicked_5(WM_MESSAGE * pMsg);
void OnButtonClicked_4(WM_MESSAGE * pMsg);
void OnButtonClicked_3(WM_MESSAGE * pMsg);
void OnButtonClicked_2(WM_MESSAGE * pMsg);
void OnButtonClicked_1(WM_MESSAGE * pMsg);
void OnButtonClicked_0(WM_MESSAGE * pMsg);
//EndofEventsFunctionList

// Button actions


void OnButtonClicked_0(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据OK
	if (temp_value && Number_Index < 9) {
		temp_value *= 10;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_1(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	if (Number_Index < 9) {
		temp_value *= 10;
		temp_value += 1;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_2(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	if (Number_Index < 9) {
		temp_value *= 10;
		temp_value += 2;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_3(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	if (Number_Index < 9) {
		temp_value *= 10;
		temp_value += 3;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_4(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	if (Number_Index < 9) {
		temp_value *= 10;
		temp_value += 4;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_5(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	if (Number_Index < 9) {
		temp_value *= 10;
		temp_value += 5;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_6(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	if (Number_Index < 9) {
		temp_value *= 10;
		temp_value += 6;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_7(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	if (Number_Index < 9) {
		temp_value *= 10;
		temp_value += 7;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_8(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	if (Number_Index < 9) {
		temp_value *= 10;
		temp_value += 8;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_9(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	if (Number_Index < 9) {
		temp_value *= 10;
		temp_value += 9;
		Number_Index++;
	}
	EDIT_SetValue(hEdit_Item, temp_value);
}


void OnButtonClicked_D(WM_MESSAGE * pMsg)
{
}

void OnButtonClicked_C(WM_MESSAGE * pMsg)
{
	long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	temp_value /= 10;
	Number_Index--;
	EDIT_SetValue(hEdit_Item, temp_value);
}

void OnButtonClicked_Symbol(WM_MESSAGE * pMsg)
{
}

void OnButtonClicked_Enter(WM_MESSAGE * pMsg)
{
	unsigned long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	switch (Register_Index) {
	case 1:
		Register_Buf[6] = (unsigned long)temp_value;
		Charge_Flag[0] = 1; 
		Save_Time = 0;
		break;
	case 2:
		Register_Buf[7] = (unsigned long)temp_value;
		Charge_Flag[0] = 1; 
		Save_Time = 0;
		break;
	case 3:
		Register_Buf[8] = (unsigned long)temp_value;
		Charge_Flag[0] = 1; 
		Save_Time = 0;
		break;
	case 4:
		Register_Buf[9] = (unsigned long)temp_value;
		Charge_Flag[0] = 1; 
		Save_Time = 0;
		break;
	case 5:
		Register_Buf[10] = (unsigned long)temp_value;
		Charge_Flag[0] = 1; 
		Save_Time = 0;
		break;		
	}

}

void OnButtonClicked_Back_Key(WM_MESSAGE * pMsg)
{
// nothing to do
}


static const GUI_WIDGET_CREATE_INFO _aDialogCreate_Key[] = {
    { FRAMEWIN_CreateIndirect,  "Key Board",         0,                       0+63,  0+38,  200,163,0,0},
    { EDIT_CreateIndirect,       "33",               GUI_ID_EDIT0,            18, 4,  149,30, 0,0},
    { BUTTON_CreateIndirect,    "0",                 GUI_ID_BUTTON0,          4,  40, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    "1",                 GUI_ID_BUTTON1,          41, 40, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    "2",                 GUI_ID_BUTTON2,          78, 40, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    "3",                 GUI_ID_BUTTON3,          115,40, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    "4",                 GUI_ID_BUTTON4,          152,40, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    "5",                 GUI_ID_BUTTON5,          4,  74, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    "6",                 GUI_ID_BUTTON6,          41, 74, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    "7",                 GUI_ID_BUTTON7,          78, 74, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    "8",                 GUI_ID_BUTTON8,          115,74, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    "9",                 GUI_ID_BUTTON9,          152,74, 31, 28, 0,0},
    { BUTTON_CreateIndirect,    ".",                 GUI_ID_BUTTON10,         4,  108,31, 28, 0,0},
    { BUTTON_CreateIndirect,    "C",                 GUI_ID_BUTTON11,         41, 108,31, 28, 0,0},
    { BUTTON_CreateIndirect,    "+/-",               GUI_ID_BUTTON12,         78, 108,31, 28, 0,0},
    { BUTTON_CreateIndirect,    "Enter",               GUI_ID_BUTTON13,         115,108,31, 28, 0,0},
    { BUTTON_CreateIndirect,    "Cancel",               GUI_ID_BUTTON14,         152,108,31, 28, 0,0}
};


static void InitDialog_Key(WM_MESSAGE * pMsg)
{
	long temp_value;
	WM_HWIN hDlg = pMsg->hWin;
	WM_HWIN	hEdit_Item = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	if (!bInitialized) {
		Number_Index = 0;
		int startValue = Register_Buf[Register_Index + 5];
		EDIT_SetDecMode(hEdit_Item, startValue, 0, 4400000, 0, GUI_EDIT_NORMAL);
		EDIT_SetTextAlign(hEdit_Item, GUI_TA_VCENTER | GUI_TA_RIGHT);
		EDIT_SetMaxLen(hEdit_Item, 7);
		bInitialized = 1;
	}
	temp_value = EDIT_GetValue(hEdit_Item);		//得到控件数据
	temp_value++;
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbDialog_Key(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
	WM_HWIN hEdit_Item= WM_GetDialogItem(hWin, GUI_ID_EDIT0);

    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            //PaintDialog_Key(pMsg);
			Number_Index=0;
			//EDIT_SetDecMode(hEdit_Item, 0,   0, 4400000, 0, 0);
            break;
        case WM_INIT_DIALOG:
            InitDialog_Key(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_BUTTON0:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_0(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_1(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON2:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_2(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON3:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_3(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON4:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_4(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON5:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_5(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON6:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_6(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON7:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_7(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON8:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_8(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON9:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_9(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON10:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_D(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON11:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_C(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON12:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_Symbol(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON13:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_Enter(pMsg);
							Key_Return_Flag=1;
							GUI_EndDialog(hWin, 0);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON14:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnButtonClicked_Back_Key(pMsg);
							GUI_EndDialog(hWin, 0);
					   break;
                    }
                    break;

            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

int CreateSubdialog(void) {
  int res;
	static bCreated = 0;
	if (! bCreated) {
		bCreated = 1;
		res = GUI_ExecDialogBox(_aDialogCreate_Key, GUI_COUNTOF(_aDialogCreate_Key), _cbDialog_Key, WM_HBKWIN, 20, 20);
		bCreated = 0;
	}
  return res;
}
